#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned long pot_rapida(unsigned long base, unsigned int exp) {
    if (exp == 0) return 1UL;

    unsigned long temp = pot_rapida(base, exp / 2);

    if (exp % 2 == 0) {
        return temp * temp;
    } else {
        return base * temp * temp;
    }
}

int main(int argc, char *argv[]) {
    unsigned int exp = (argc >= 2) ? (unsigned int)strtoul(argv[1], NULL, 10) : 1000000000;
    unsigned long base = (argc >= 3) ? strtoul(argv[2], NULL, 10) : 1UL;

    clock_t t0 = clock();
    unsigned long res = pot_rapida(base, exp);
    clock_t t1 = clock();

    printf("base = %lu, exp = %u -> res = %lu\n", base, exp, res);
    printf("tempo: %.6f s\n", (double)(t1 - t0) / CLOCKS_PER_SEC);

    return 0;
}

