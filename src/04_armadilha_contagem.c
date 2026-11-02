#include <stdio.h>
#include <stdlib.h>

static unsigned long g_chamadas = 0;

unsigned long pot_bifurcacao_contada(unsigned long base, unsigned int exp) {
    g_chamadas++;

    if (exp == 0) return 1UL;
    if (exp == 1) return base;

    if (exp % 2 == 0) {
        return pot_bifurcacao_contada(base, exp / 2) *
               pot_bifurcacao_contada(base, exp / 2);
    } else {
        return base *
               pot_bifurcacao_contada(base, exp / 2) *
               pot_bifurcacao_contada(base, exp / 2);
    }
}

int main(int argc, char *argv[]) {
    unsigned int exp = (argc >= 2) ? (unsigned int)strtoul(argv[1], NULL, 10) : 1024;
    unsigned long base = (argc >= 3) ? strtoul(argv[2], NULL, 10) : 1UL;

    g_chamadas = 0;
    unsigned long res = pot_bifurcacao_contada(base, exp);

    printf("base = %lu, exp = %u -> res = %lu\n", base, exp, res);
    printf("chamadas recursivas: %lu\n", g_chamadas);

    return 0;
}

