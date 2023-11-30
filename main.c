#include <stdio.h>

void extendedEuclidean(int a, int b, int* x, int* y, int* gcd);
int gcdExtended(int a, int b, int* x, int* y);
int generatePrivateKey();
int modInverse(int A, int M);
int encryptMessage(int message, int e, int n);
long long mod_pow(long long base, long long exponent, long long modulus);

int main() {
    int x, y, gcd;
    extendedEuclidean(31, 3480, &x, &y, &gcd);
    int priv = generatePrivateKey(31, 3599);
    printf("Private key: (%d, %d)\n", priv, 3480);
    int encrypted = mod_pow(2207, 15, 4897);
    printf("Encrypted text: %lld \n", encrypted);
    int decrypted = mod_pow(2040, 4439, 4897);
    printf("Decrypted text: %lld \n", decrypted);

    return 0;
}

//Encryption ---- p = 59, q = 61.
//M = 58*60=3480
//When (31, 3480), (-449, 4).

// Function to calculate (base^exponent) % modulus using the square-and-multiply algorithm
long long mod_pow(long long base, long long exponent, long long modulus) {
    long long result = 1;

    // Ensure exponent is non-negative
    if (exponent < 0) {
        fprintf(stderr, "Error: Exponent should be non-negative.\n");
        return -1;  // Error code
    }

    // Iterate through the bits of the exponent
    while (exponent > 0) {
        // If the current bit is 1, multiply by base
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }

        // Square the base for the next bit
        base = (base * base) % modulus;

        // Move to the next bit of the exponent
        exponent /= 2;
    }

    return result;
}


//e=31 and mod m = 3599
//Private key: ( modInverse(e,m), 3480(n) )
int generatePrivateKey(int e, int m) {
    return modInverse(e, m);
}


int modInverse(int A, int M) { 
    for (int X = 1; X < M; X++) 
        if (((A % M) * (X % M)) % M == 1) 
            return X; 
} 


void extendedEuclidean(int a, int b, int* x, int* y, int* gcd) {
    int s, t = 0;
    int old_s = 1;
    int r = b;
    int old_r = a;
    int temp = 0;

    while (r != 0) {
        int quotient = old_r/r;
        
        temp = r;
        r = old_r - r*quotient;
        old_r = temp;

        temp = s;
        s = old_s - s*quotient;
        old_s = temp;

        //temp = t;
        //t = old_t - t*quotient;
        //old_t = temp;
    }

    if (b != 0)
    {
        t = (old_r - old_s * a) / b;
    }

    printf("Bezout Coefficients: (x: %d)..(y: %d)..(gcd: %d) \n", old_s, t, old_r);
    *x = old_s;
    *y = t;
    *gcd = old_r;
}