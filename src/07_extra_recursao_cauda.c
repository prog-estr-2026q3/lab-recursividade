#include <stdio.h>
#include <stdlib.h>

static unsigned long pot_tail_aux(unsigned long base, unsigned int exp, unsigned long acc) {
    if (exp == 0) return acc;

    if (exp % 2 == 1) {
        return pot_tail_aux(base * base, exp / 2, acc * base);
    } else {
        return pot_tail_aux(base * base, exp / 2, acc);
    }
}

unsigned long pot_rapida_tail(unsigned long base, unsigned int exp) {
    return pot_tail_aux(base, exp, 1UL);
}

unsigned long pot_binaria_iter(unsigned long base, unsigned int exp) {
    unsigned long res = 1UL;
    while (exp > 0) {
        if (exp % 2 == 1) {
            res *= base;
        }
        base *= base;
        exp /= 2;
    }
    return res;
}

int main(int argc, char *argv[]) {
    unsigned int exp = (argc >= 2) ? (unsigned int)strtoul(argv[1], NULL, 10) : 1000000000;
    unsigned long base = (argc >= 3) ? strtoul(argv[2], NULL, 10) : 1UL;

    printf("base = %lu, exp = %u\n", base, exp);
    printf("tail (TCO): %lu\n", pot_rapida_tail(base, exp));
    printf("iter (bin): %lu\n", pot_binaria_iter(base, exp));

    return 0;
}

