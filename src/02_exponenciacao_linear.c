#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned long pot_iter_linear(unsigned long base, unsigned int exp) {
    unsigned long res = 1UL;
    for (unsigned int i = 0; i < exp; i++) {
        res *= base;
    }
    return res;
}

unsigned long pot_rec_linear(unsigned long base, unsigned int exp) {
    if (exp == 0) {
        return 1UL;
    }
    return base * pot_rec_linear(base, exp - 1);
}

int main(int argc, char *argv[]) {
    unsigned int exp = (argc >= 2) ? (unsigned int)strtoul(argv[1], NULL, 10) : 10;
    unsigned long base = (argc >= 3) ? strtoul(argv[2], NULL, 10) : 2UL;

    printf("base = %lu, exp = %u\n", base, exp);

    clock_t t0 = clock();
    unsigned long res_iter = pot_iter_linear(base, exp);
    clock_t t1 = clock();
    printf("iter: %lu (%.6f s)\n", res_iter, (double)(t1 - t0) / CLOCKS_PER_SEC);

    clock_t t2 = clock();
    unsigned long res_rec = pot_rec_linear(base, exp);
    clock_t t3 = clock();
    printf("rec:  %lu (%.6f s)\n", res_rec, (double)(t3 - t2) / CLOCKS_PER_SEC);

    return 0;
}

