// Include header file
#include <stdio.h>
//  C program for
//  Extended euclidean algorithm implementation

// Find GCD of given two given number
void  extended_gcd (int a, int b)
{
    // Declare some auxiliary variable
    int s = 0;
    int r = b;
    int old_s = 1;
    int old_r = a;
    int temp = 0;
    int bezout_t = 0;
    // When r not equal to zero
    while(r != 0)
    {
        int quotient = old_r / r;
        
        temp = r;
        r = old_r - quotient * r;
        old_r = temp;

        temp = s;
        s = old_s - quotient * s;
        old_s = temp;
    }
    if (b != 0)
    {
        bezout_t = (old_r - old_s * a) / b;
    }
    // Display given number
    printf("Given (a,b) : %d,%d\n" , a , b);
    // Displaying the value calculate result
    printf("Bezout coefficients : %d,%d\n\n" , 
        old_s, bezout_t);
    printf("greatest common divisor : %d\n\n" , old_r);
}

int main()
{
    extended_gcd(59, 1012);
    return 0; 
}