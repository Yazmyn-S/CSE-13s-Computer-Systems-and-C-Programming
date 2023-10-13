#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "benwvahsmr"
bool is_b = false;
bool is_e = false;
bool is_n = false;
bool is_w = false;
bool is_v = false;
bool is_a = false;
bool is_h = false;
bool is_s = false;
bool is_m = false;
bool is_r = false;

double k = 0;

int main(int argc, char **argv) {
    double diff;
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b':
            is_b = true;
            diff = M_PI - pi_bbp();
            break;
        case 'e':
            is_e = true;
            diff = M_E - e();
            break;
        case 'n':
            is_n = true;
            diff = sqrt(k) - sqrt_newton(k);
            break;
        case 'r':
            is_r = true;
            diff = M_PI - pi_euler();
            break;
        case 'm':
            is_m = true;
            diff = M_PI - pi_madhava();
            break;
        case 'v':
            is_v = true;
            diff = M_PI - pi_viete();
            break;
        case 'w':
            is_w = true;
            diff = M_PI - pi_wallis();
            break;
        case 'a': is_a = true; break;
        case 'h': is_h = true; break;
        case 's': is_s = true; break;
        }
    }

    if (is_h == false && is_s == true && is_b == false && is_a == false && is_w == false
        && is_v == false && is_e == false && is_r == false && is_n == false && is_m == false) {
        printf("After typing out ./mathlib-test you must select one or more of the following "
               "letters:\n");
        printf(
            "a (runs all tests)\ne (runs e test)\ns(displays how terms each test has)\nb,w,v,m,r "
            "(all run PI tests)\nn (runs square root test)\nh (displays this help message).\n");
        printf("Example: ./mathlib -b -w -s\n");
    }

    if (is_h == false && is_s == false && is_b == false && is_a == false && is_w == false
        && is_v == false && is_e == false && is_r == false && is_n == false && is_m == false) {
        printf("After typing out ./mathlib-test you must select one or more of the following "
               "letters:\n");
        printf(
            "a (runs all tests)\ne (runs e test)\ns(displays how terms each test has)\nb,w,v,m,r "
            "(all run PI tests)\nn (runs square root test)\nh (displays this help message).\n");
        printf("Example: ./mathlib -b -w -s\n");
    }

    if (is_h == true) {
        is_a = false;
        is_b = false;
        is_r = false;
        is_e = false;
        is_n = false;
        is_w = false;
        is_v = false;
        is_m = false;
        is_s = false;

        printf("After typing out ./mathlib-test you must select one or more of the following "
               "letters:\n");
        printf(
            "a (runs all tests)\ne (runs e test)\ns(displays how terms each test has)\nb,w,v,m,r "
            "(all run PI tests)\nn (runs square root test)\nh (displays this help message).\n");
        printf("Example: ./mathlib -b -w -s\n");

        is_h = false;
    }
    if (is_a == true) {
        diff = M_E - e();
        printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", e(), M_E, diff);
        is_e = false;
        if (is_s == true) {
            printf("e() terms = %d\n", e_terms());
        }

        diff = M_PI - pi_bbp();
        printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_bbp(), M_PI, diff);
        if (is_s == true) {
            printf("pi_bbp() terms = %d\n", pi_bbp_terms());
        }
        is_b = false;

        diff = M_PI - pi_madhava();
        printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_madhava(), M_PI,
            diff);
        if (is_s == true) {
            printf("pi_madhava() terms = %d\n", pi_madhava_terms());
        }
        is_m = false;

        diff = M_PI - pi_euler();
        printf("pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_euler(), M_PI, diff);
        if (is_s == true) {
            printf("pi_euler() terms = %d\n", pi_euler_terms());
        }
        is_r = false;

        diff = M_PI - pi_viete();
        printf("pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_viete(), M_PI, diff);
        if (is_s == true) {
            printf("pi_viete() terms = %d\n", pi_viete_factors());
        }
        is_v = false;

        is_w = false;
        diff = M_PI - pi_wallis();
        printf(
            "pi_wallis() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_wallis(), M_PI, diff);
        if (is_s == true) {
            printf("pi_wallis() terms = %d\n", pi_wallis_factors());
        }
        is_n = false;
        for (k = 0; k <= 9.95; k += 0.1) {
            sqrt(k);
            sqrt_newton(k);
            diff = sqrt(k) - sqrt_newton(k);
            printf("sqrt_newton(%.2lf) = %16.15lf, sqrt(%.2lf) = %16.15lf, diff = %16.15lf\n", k,
                sqrt_newton(k), k, sqrt(k), diff);
            if (is_s == true) {
                printf("sqrt_newton() terms = %d\n", sqrt_newton_iters());
            }
        }
    }

    if (is_e == true) {
        diff = M_E - e();
        printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", e(), M_E, diff);
        is_e = false;

        if (is_s == true) {
            printf("e() terms = %d\n", e_terms());
            //is_s = false;
        }
    }

    if (is_b == true) {
        diff = M_PI - pi_bbp();
        printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_bbp(), M_PI, diff);
        is_b = false;

        if (is_s == true) {
            printf("pi_bbp() terms = %d\n", pi_bbp_terms());
            //is_s = false;
        }
    }

    if (is_m == true) {
        diff = M_PI - pi_madhava();
        printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_madhava(), M_PI,
            diff);
        is_m = false;

        if (is_s == true) {
            printf("pi_madhava() terms = %d\n", pi_madhava_terms());
        }
    }

    if (is_r == true) {
        diff = M_PI - pi_euler();
        printf("pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_euler(), M_PI, diff);
        is_r = false;

        if (is_s == true) {
            printf("pi_euler() terms = %d\n", pi_euler_terms());
        }
    }

    if (is_v == true) {
        is_v = false;
        diff = M_PI - pi_viete();
        printf("pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_viete(), M_PI, diff);

        if (is_s == true) {
            printf("pi_viete() terms = %d\n", pi_viete_factors());
        }
    }

    if (is_w == true) {
        is_w = false;
        diff = M_PI - pi_wallis();
        printf(
            "pi_wallis() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_wallis(), M_PI, diff);

        if (is_s == true) {
            printf("pi_wallis() terms = %d\n", pi_wallis_factors());
        }
    }

    if (is_n == true) {
        is_n = false;

        for (double k = 0; k <= 9.95; k += 0.1) {
            sqrt(k);
            sqrt_newton(k);
            diff = sqrt(k) - sqrt_newton(k);
            printf("sqrt_newton(%.2lf) = %16.15lf, sqrt(%.2lf) = %16.15lf, diff = %16.15lf\n", k,
                sqrt_newton(k), k, sqrt(k), diff);

            if (is_s == true) {
                printf("sqrt_newton() terms = %d\n", sqrt_newton_iters());
            }
        }
    }

    return 0;
}
