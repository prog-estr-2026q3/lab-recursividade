# Laboratório 05 — recursividade (aquecimento guiado)

Este repositório contém o código-fonte base utilizado no aquecimento guiado do **Laboratório 05: Recursividade** da disciplina de Programação Estruturada (MCTA028-15) da UFABC.

O objetivo desta atividade prática é investigar no terminal o comportamento em tempo de execução de funções recursivas em linguagem C, analisando a anatomia da pilha de execução (*call stack*), os limites físicos de memória (*stack overflow*), a armadilha de complexidade da bifurcação ingênua ($\Theta(n)$ chamadas redundantes) e o salto de desempenho proporcionado pelo paradigma de divisão e conquista ($O(\log n)$).

---

## Estrutura do projeto

```
.
├── Makefile
├── README.md
├── .gitignore
├── .devcontainer/   # Configuração de ambiente para dev container / GitHub Codespaces
│   ├── Dockerfile
│   └── devcontainer.json
├── src/             # Códigos-fonte (.c) dos experimentos
│   ├── 01_bench_linear.c
│   ├── 02_exponenciacao_linear.c
│   ├── 03_armadilha_bifurcacao.c
│   ├── 04_armadilha_contagem.c
│   ├── 05_exponenciacao_rapida.c
│   ├── 06_exponenciacao_rapida_contagem.c
│   └── 07_extra_recursao_cauda.c
└── build/           # Executáveis compilados (ignorado pelo git)
```

---

## Como compilar e executar

### 1. Compilar todos os programas

Para compilar todos os programas em modo de depuração padrão (`-g -Wall -Wextra -pedantic`):

```bash
make
```

Os binários correspondentes serão gerados no diretório `build/`.

### 2. Executar a suíte completa de testes

Para compilar todas as versões (padrão e com sanitizadores) e disparar a execução de cada experimento:

```bash
make test
```

### 3. Medição empírica de escala linear

Para medir o tempo de execução do laço `for` linear iterativo escalando de $10^7$ a $10^9$ iterações:

```bash
make bench
```

### 4. Demonstração de estouro de pilha (stack overflow)

Para observar a exaustão da pilha de execução do sistema operacional ao disparar $265.000$ quadros de ativação na recursão linear simples:

```bash
# Execução direta (gera Segmentation fault / SIGSEGV ao colidir com a página de guarda da MMU):
make estouro-pilha

# Execução instrumentada sob AddressSanitizer (exibe relatório detalhado de stack-overflow):
make estouro-pilha-asan
```

### 5. Compilação com AddressSanitizer (ASan)

Para compilar todas as fontes instrumentadas com checagem de limites de memória e comportamentos indefinidos (`-fsanitize=address,undefined -g`):

```bash
make asan
```

Os executáveis instrumentados receberão o sufixo `_asan` no diretório `build/` (por exemplo, `build/05_exponenciacao_rapida_asan`).

### 6. Inspeção de eliminação de cauda no assembly

Para compilar a versão recursiva em cauda com otimização `-O2` e inspecionar o código assembly gerado, confirmando a conversão da chamada de função recursiva em salto incondicional (laço no nível de máquina sem crescimento da pilha):

```bash
make asm
```

O arquivo correspondente será gerado em `build/07_extra_recursao_cauda.s`.

### 7. Execução manual de cada programa

Você pode rodar qualquer binário individualmente a partir da raiz do projeto, passando parâmetros via linha de comando:

```bash
# Exponenciação linear iterativa vs recursiva (base 2, expoente 15)
./build/02_exponenciacao_linear 15 2

# Armadilha da bifurcação ingênua pura
./build/03_armadilha_bifurcacao 16 2

# Armadilha da bifurcação com diagnóstico de chamadas totais
./build/04_armadilha_contagem 16 2
./build/04_armadilha_contagem 1024 1

# Exponenciação rápida por divisão e conquista O(log n)
./build/05_exponenciacao_rapida 1000000000 1

# Tabela comparativa de chamadas recursivas
./build/06_exponenciacao_rapida_contagem

# Otimização em cauda e algoritmo binário iterativo
./build/07_extra_recursao_cauda 1000000000 1
```

### 8. Limpar os arquivos compilados

Para remover o diretório `build/` e todos os arquivos compilados ou intermediários:

```bash
make clean
```

---

## Descrição dos exemplos em `src/`

- `01_bench_linear.c`: Medição empírica de tempo do laço iterativo $O(n)$ com `<time.h>` e `clock()`, demonstrando que para $n = 10^9$ operações a computação exige quase 1 segundo de CPU no terminal.
- `02_exponenciacao_linear.c`: Confronto direto entre laço `for` ($O(1)$ espaço auxiliar) e a recursão linear simples ($x \cdot x^{n-1}$, exigindo $O(n)$ quadros de ativação na pilha), culminando em estouro de pilha para $n \ge 265.000$.
- `03_armadilha_bifurcacao.c`: A bifurcação ingênua pura em C (`pot(x, n/2) * pot(x, n/2)`), evidenciando a duplicação desnecessária de subproblemas.
- `04_armadilha_contagem.c`: Diagnóstico instrumentado com contador de chamadas, provando que a bifurcação ingênua gera uma árvore binária completa de chamadas redundantes ($2047$ chamadas para $n = 1024$).
- `05_exponenciacao_rapida.c`: A exponenciação rápida correta por divisão e conquista, utilizando a variável local `temp` para reaproveitar o subproblema resolvido e calcular potências com expoente de 1 bilhão em microssegundos.
- `06_exponenciacao_rapida_contagem.c`: Instrumentação empírica com geração de tabela comparativa de chamadas recursivas contra a versão ingênua ($4, 16, 1024, 10^9$).
- `07_extra_recursao_cauda.c`: Implementação de recursão em cauda (*tail recursion*) com acumulador e versão iterativa clássica (*Square-and-Multiply*).

---

## Roteiro de experimentação em sala

| Janela | Experimento | Comando sugerido | Conceito investigado |
| :---: | :--- | :--- | :--- |
| **00--08 min** | Escala linear e benchmark | `make bench` | Tempo escalando proporcionalmente a $n$ no laço iterativo. |
| **08--16 min** | Limites físicos da pilha | `./build/02_exponenciacao_linear 265000 1` | Esgotamento do limite de pilha do SO (`SIGSEGV` / ASan). |
| **16--26 min** | Armadilha da bisseção | `./build/04_armadilha_contagem 1024 1` | Árvore redundante de $2047$ chamadas ($\Theta(n)$). |
| **26--36 min** | Exponenciação rápida | `./build/06_exponenciacao_rapida_contagem` | Redução para apenas $31$ chamadas para $n = 10^9$ ($O(\log n)$). |
| **36--40 min** | Otimização de cauda | `make asm` | Eliminação de quadro de chamada (*tail call optimization*). |
