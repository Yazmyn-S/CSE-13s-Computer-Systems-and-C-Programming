//implementation of the madhava series to find pi and return thenumber of the computed terms
#include "mathlib.h"

#include <stdio.h>

static int count = 0;

double pi_madhava() {
    count = 0;
    double exponent = 1.0;
    double term = 1.0;
    double madhava = 0.0;
    double madhava_result = 0.0;

    for (int k = 0; absolute(term) > EPSILON; k++) {
        double Nterm;
        double Dterm = 2.0 * k + 1.0;

        if (k == 0) {
            Nterm = 1;
            term = Nterm / Dterm;

            count += 1;
            madhava = term + madhava;
        }

        else {
            Nterm = (1.0 / -3.0) * exponent;
            term = Nterm / Dterm;

            exponent = Nterm;
            madhava = term + madhava;
            count += 1;
        }
        madhava_result = sqrt_newton(12) * madhava;
    }
    return madhava_result;
}

int pi_madhava_terms() {
    return count;
}
