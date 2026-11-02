CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic
DEBUG_FLAGS = -g
ASAN_FLAGS = -fsanitize=address,undefined -g
LDLIBS =

SRC_DIR = src
BUILD_DIR = build

SRCS = $(SRC_DIR)/01_bench_linear.c \
       $(SRC_DIR)/02_exponenciacao_linear.c \
       $(SRC_DIR)/03_armadilha_bifurcacao.c \
       $(SRC_DIR)/04_armadilha_contagem.c \
       $(SRC_DIR)/05_exponenciacao_rapida.c \
       $(SRC_DIR)/06_exponenciacao_rapida_contagem.c \
       $(SRC_DIR)/07_extra_recursao_cauda.c

BINS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%,$(SRCS))
ASAN_BINS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%_asan,$(SRCS))

all: $(BINS)

asan: $(ASAN_BINS)

$(BUILD_DIR)/%_asan: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(ASAN_FLAGS) $< -o $@ $(LDLIBS)

$(BUILD_DIR)/%: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $< -o $@ $(LDLIBS)

$(BUILD_DIR):
	mkdir -m 755 -p $(BUILD_DIR)

# Demonstracao pedagogica de medicao empírica de escala linear O(n)
bench: $(BUILD_DIR)/01_bench_linear
	@echo "=== Executando medicao empírica O(n) ==="
	./$(BUILD_DIR)/01_bench_linear

# Demonstracao pedagogica de estouro de pilha (espera-se SIGSEGV / stack overflow)
estouro-pilha: $(BUILD_DIR)/02_exponenciacao_linear
	@echo "=== Executando 02_exponenciacao_linear com n=265000 (espera-se falha de segmentacao) ==="
	-./$(BUILD_DIR)/02_exponenciacao_linear 265000 1

# Demonstracao pedagogica com relatorio detalhado do AddressSanitizer
estouro-pilha-asan: $(BUILD_DIR)/02_exponenciacao_linear_asan
	@echo "=== Executando 02_exponenciacao_linear_asan sob AddressSanitizer (espera-se stack-overflow) ==="
	-./$(BUILD_DIR)/02_exponenciacao_linear_asan 265000 1

# Alvo para inspecionar a eliminacao de cauda (Tail Call Elimination) no assembly
asm: $(SRC_DIR)/07_extra_recursao_cauda.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -O2 -S -fverbose-asm $< -o $(BUILD_DIR)/07_extra_recursao_cauda.s
	@echo "Assembly gerado em $(BUILD_DIR)/07_extra_recursao_cauda.s (-O2)."
	@echo "Verifique que a instrucao 'call' foi eliminada dentro do laco principal!"

test: all asan
	@echo "=== Executando suite de testes do aquecimento guiado ==="
	@echo "--- Teste 1: bench linear ---"
	./$(BUILD_DIR)/01_bench_linear
	@echo ""
	@echo "--- Teste 2: exponenciacao linear ---"
	./$(BUILD_DIR)/02_exponenciacao_linear 15 2
	./$(BUILD_DIR)/02_exponenciacao_linear_asan 15 2
	@echo ""
	@echo "--- Teste 3: armadilha da bifurcacao (puro) ---"
	./$(BUILD_DIR)/03_armadilha_bifurcacao 16 2
	./$(BUILD_DIR)/03_armadilha_bifurcacao_asan 16 2
	@echo ""
	@echo "--- Teste 4: armadilha da contagem (arvore redundante) ---"
	./$(BUILD_DIR)/04_armadilha_contagem 16 2
	./$(BUILD_DIR)/04_armadilha_contagem_asan 16 2
	@echo ""
	@echo "--- Teste 5: exponenciacao rapida O(log n) (puro) ---"
	./$(BUILD_DIR)/05_exponenciacao_rapida 1000000000 1
	./$(BUILD_DIR)/05_exponenciacao_rapida_asan 1000000000 1
	@echo ""
	@echo "--- Teste 6: tabela comparativa de chamadas ---"
	./$(BUILD_DIR)/06_exponenciacao_rapida_contagem 1000000000 1
	./$(BUILD_DIR)/06_exponenciacao_rapida_contagem_asan 1000000000 1
	@echo ""
	@echo "--- Teste 7: recursao de cauda e versao iterativa ---"
	./$(BUILD_DIR)/07_extra_recursao_cauda 1000000000 1
	./$(BUILD_DIR)/07_extra_recursao_cauda_asan 1000000000 1
	@echo ""
	@echo "Todos os testes do aquecimento executaram com sucesso sob compilacao padrao e AddressSanitizer!"

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all asan clean bench estouro-pilha estouro-pilha-asan asm test
