#include "mathlib.h"

#include <stdio.h>

int term;
static int count = 0;

double pi_bbp(void) {
    count = 0;
    double PI = 0;
    int k = 0;
    double exponent = 1.0;
    double term = 1;
    for (k = 0; absolute(term) > EPSILON; k++) //EPSILON = 1e-14
    {
        term = exponent
               * (4.0 / (8.0 * k + 1.0) - 2.0 / (8.0 * k + 4.0) - 1.0 / (8.0 * k + 5.0)
                   - 1.0 / (8.0 * k + 6.0));
        double new_exponent = exponent * (1.0 / 16.0);
        exponent = new_exponent;
        PI = PI + term;
        count += 1;
    }
    return PI;
}

int pi_bbp_terms() {
    return count;
}
