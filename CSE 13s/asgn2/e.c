#include "mathlib.h"

#include <stdio.h>

static int count = 0;

double e(void) {
    count = 0;
    double E = 0;
    int k = 0;
    int factorial = 1;
    double term = 1;

    for (k = 0; absolute(term) > EPSILON; k++) {
        if (k == 0 || k == 1) {
            factorial = 1;
            term = 1.0 / factorial++;
            E = term + E;
            count += 1;
        } else {
            term = term / factorial++;
            E = term + E;
            count += 1; //track number of implimentations
        }
    }
    return E;
}

int e_terms() {
    return count;
}
