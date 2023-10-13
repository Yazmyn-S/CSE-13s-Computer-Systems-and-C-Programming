//implementation of wallis formula to find pi and return the number of computed factos
#include "mathlib.h"

#include <stdio.h>

static int count = 0;

double pi_wallis() {
    count = 0;
    double factor = 0.0;
    double wallis = 1.0;
    double wallis_result = 0.0;

    for (int k = 1; absolute(1.0 - factor) > EPSILON; k++) {
        factor = (4.0 * k * k) / (4.0 * k * k - 1.0);
        wallis = factor * wallis;
        count += 1;
    }
    wallis_result = 2.0 * wallis;

    return wallis_result;
}

int pi_wallis_factors() {
    return count;
}
