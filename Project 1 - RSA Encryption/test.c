#include <stdio.h>

void extendedEuclidean(int a, int b, int* x, int* y, int* gcd);
int generatePrivateKey(int e, int m);
int modInverse(int A, int M);
int encryptMessage(int message, int e, int n);
long long mod_pow(long long base, long long exponent, long long modulus);

int main() {
    int x, y, gcd;
    int out;
    //extendedEuclidean(15, 4756, &x, &y, &gcd);
    extendedEuclidean(7, 3480, &x, &y, &gcd);
    printf("GCD: %d \n", gcd);
    out = iterate_e();
    printf("Iterated E: %d \n", out);
    int priv = generatePrivateKey(7, 3480);
    printf("Private key: (%d, %d)\n", priv, 3480);
    int iterated = iterate_d();
    printf("Private key: (%d, %d)\n", priv, 3480);

    int encrypted = mod_pow(1112, out, 3599);
    printf("Encrypted text: %lld \n", encrypted);
    int decrypted = mod_pow(encrypted, iterated, 3599);
    printf("Decrypted text: %lld \n", decrypted);

    encrypted = mod_pow(2021, out, 3599);
    printf("Encrypted text: %lld \n", encrypted);
    decrypted = mod_pow(encrypted, iterated, 3599);
    printf("Decrypted text: %lld \n", decrypted);

    return 0;
}

int iterate_e() {
    int x, y;
    int gcd = 0;
    int tryNumber = 1;
    while (gcd != 1) {
        tryNumber++;
        extendedEuclidean(tryNumber, 3480, &x, &y, &gcd);
    }
    return tryNumber;
}

int iterate_d() {
    int x, y;
    int privKey;
    int gcd = 0;
    int tryNumber = 1;
    while (gcd != 1) {
        tryNumber++;
        extendedEuclidean(tryNumber, 3480, &x, &y, &gcd);
    }
    privKey = generatePrivateKey(tryNumber, 3480);
    return privKey;
}


//Encryption ---- p = 59, q = 61.
//M = 58*60=3480
//When (31, 3480), (-449, 4).
long long mod_pow(long long base, long long exponent, long long modulus) {
    long long result = 1;

    if (exponent < 0) {
        return -1;  // Error code
    }

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
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

void  extendedEuclidean(int a, int b, int* x, int* y, int* gcd) {
    int s = 0;
    int temp = 0;
    int t = 0;
    int r = b;
    int old_s = 1;
    int old_r = a;
    
    while(r != 0) {
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
        t = (old_r - old_s * a) / b;
    }

    *x = old_s;
    *y = t;
    *gcd = old_r;
}