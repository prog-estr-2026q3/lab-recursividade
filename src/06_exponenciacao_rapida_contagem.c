#include <stdio.h>
#include <stdlib.h>

static unsigned long g_chamadas = 0;

unsigned long pot_rapida_contada(unsigned long base, unsigned int exp) {
    g_chamadas++;

    if (exp == 0) return 1UL;

    unsigned long temp = pot_rapida_contada(base, exp / 2);

    if (exp % 2 == 0) {
        return temp * temp;
    } else {
        return base * temp * temp;
    }
}

static void imprimir_tabela(void) {
    unsigned int testes[] = {4, 16, 1024, 1000000000};
    const char *ingenua[] = {"7", "31", "2047", "> 1 bilhao"};
    int total = sizeof(testes) / sizeof(testes[0]);

    printf("%-12s %-16s %-15s\n", "n", "rapida (log n)", "ingenua (Theta(n))");
    for (int i = 0; i < total; i++) {
        g_chamadas = 0;
        pot_rapida_contada(1UL, testes[i]);
        printf("%-12u %-16lu %-15s\n", testes[i], g_chamadas, ingenua[i]);
    }
}

int main(int argc, char *argv[]) {
    unsigned int exp = (argc >= 2) ? (unsigned int)strtoul(argv[1], NULL, 10) : 1000000000;
    unsigned long base = (argc >= 3) ? strtoul(argv[2], NULL, 10) : 1UL;

    g_chamadas = 0;
    unsigned long res = pot_rapida_contada(base, exp);

    printf("base = %lu, exp = %u -> res = %lu (chamadas = %lu)\n\n", base, exp, res, g_chamadas);

    imprimir_tabela();
    return 0;
}

