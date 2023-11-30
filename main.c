#include <stdio.h>

void extendedEuclidean(int a, int b, int* x, int* y);
int gcdExtended(int a, int b, int* x, int* y);

int main() {
    int x, y;
    extendedEuclidean(59, 1012, &x, &y);
    printf("Bezout Coefficients: (x: %d)..(y: %d)", x, y);

    int gcd = gcdExtended(59, 1012, &x, &y);
    printf("Bezout Coefficients: (x: %d)..(y: %d)", x, y);

    return 0;
}

void extendedEuclidean(int a, int b, int* x, int* y) {
    int s, old_t = 0;
    int old_s, t = 1;
    int r = b;
    int old_r = a;
    int temp;

    while (r != 0) {
        int quotient = old_r/r;
        temp = r;
        r = old_r - (r*quotient);
        old_r = temp;
        temp = s;
        s = old_s - (s*quotient);
        old_s = temp;
        temp = t;
        t = old_t - (t*quotient);
        old_t = temp;
    }

    *x = old_s;
    *y = old_t;
}

//With recursion
int gcdExtended(int a, int b, int* x, int* y)
{
    // Base Case
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
 
    int x1, y1; // To store results of recursive call
    int gcd = gcdExtended(b % a, a, &x1, &y1);
 
    // Update x and y using results of recursive
    // call
    *x = y1 - (b / a) * x1;
    *y = x1;
 
    return gcd;
}