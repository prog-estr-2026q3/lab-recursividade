#include <stdio.h>
#include <time.h>

unsigned long pot_iter_linear(unsigned long base, unsigned int exp) {
    unsigned long res = 1UL;
    for (unsigned int i = 0; i < exp; i++) {
        res *= base;
    }
    return res;
}

int main(void) {
    unsigned int testes[] = {10000000, 100000000, 1000000000};
    int total = sizeof(testes) / sizeof(testes[0]);

    for (int i = 0; i < total; i++) {
        clock_t t0 = clock();
        pot_iter_linear(1UL, testes[i]);
        clock_t t1 = clock();
        printf("n = %10u: %.4f s\n", testes[i], (double)(t1 - t0) / CLOCKS_PER_SEC);
    }
    return 0;
}

