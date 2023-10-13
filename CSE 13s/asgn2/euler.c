//implement Euler's solution used to approicmate pi anf reutn the number of computed terms
#include "mathlib.h"

#include <stdio.h>

static int count = 0;

double pi_euler() {
    count = 0;
    double term = 2;
    double euler = 0;
    double euler_result = 0;
    for (int k = 1; absolute(term) > EPSILON; k++) {
        term = 1.0 / k * 1.0 / k;
        count += 1;
        euler = term + euler;
        //printf("%16.15f\n", term);
    }
    euler_result = sqrt_newton(6 * euler);
    return euler_result;
}

int pi_euler_terms() {
    return count;
}
