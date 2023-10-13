//implement sqaure root approximation with Newton's method
//funtion returns number of computer iterations
#include "mathlib.h"

#include <stdio.h>

static int count = 0;

double sqrt_newton(double x) {
    count = 0;
    double next_y = 1.0; //initial guess for sqrt(x)
    double y = 0.0; //force while condition to be true initially
    while (absolute(next_y - y) > EPSILON) {
        y = next_y;
        next_y = 0.5 * (y + x / y);
        count += 1;
    }
    return next_y;
}

int sqrt_newton_iters() {
    return count;
}
