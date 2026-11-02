#include <stdio.h>
#include <stdlib.h>

unsigned long pot_bifurcacao_ingenua(unsigned long base, unsigned int exp) {
    if (exp == 0) return 1UL;
    if (exp == 1) return base;

    if (exp % 2 == 0) {
        return pot_bifurcacao_ingenua(base, exp / 2) *
               pot_bifurcacao_ingenua(base, exp / 2);
    } else {
        return base *
               pot_bifurcacao_ingenua(base, exp / 2) *
               pot_bifurcacao_ingenua(base, exp / 2);
    }
}

int main(int argc, char *argv[]) {
    unsigned int exp = (argc >= 2) ? (unsigned int)strtoul(argv[1], NULL, 10) : 16;
    unsigned long base = (argc >= 3) ? strtoul(argv[2], NULL, 10) : 2UL;

    unsigned long res = pot_bifurcacao_ingenua(base, exp);
    printf("base = %lu, exp = %u -> res = %lu\n", base, exp, res);

    return 0;
}
