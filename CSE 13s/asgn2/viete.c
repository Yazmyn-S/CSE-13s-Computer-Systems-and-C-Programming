//imple,ent viete's formula to find pi and reutn the number of computed factors
#include "mathlib.h"

#include <stdio.h>

static int count = 0;

double pi_viete() {
    count = 0;
    double factor = 0.0;
    double viete = 1.0;
    double viete_result = 0.0;
    double Dfactor = 2.0;
    double Nfactor = 0.0;

    for (int k = 1; absolute(1.0 - factor) > EPSILON; k++) {
        if (k == 1) {
            Nfactor = sqrt_newton(2.0);
            factor = Nfactor / Dfactor;
            count += 1;
            viete = factor * viete;
        } else {
            Nfactor = sqrt_newton(2.0 + Nfactor);
            factor = Nfactor / Dfactor;
            count += 1;
            viete = factor * viete;
        }
    }

    viete_result = 2.0 / viete;
    return viete_result;
}

int pi_viete_factors() {
    return count;
}
