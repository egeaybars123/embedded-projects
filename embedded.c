#include <MKL25Z4.H>
#include <stdio.h>
#include <math.h>
#include "utils.h"

void LCD_command(unsigned char command);
void LCD_init(void);
void keypad_init(void);
uint32_t keypad_getkey(void);
void Delay(volatile unsigned int time_del);
void print_fnc(unsigned char *data);
void LCD_data(unsigned char data);
int array_to_int4(int _input[4]);
void separate_blocks(int original[], int length, int firstHalf[], int secondHalf[]);
void extendedEuclidean(int a, int b, int* x, int* y, int* gcd);
int gcdExtended(int a, int b, int* x, int* y);
int generatePrivateKey(int e, int m);
int modInverse(int A, int M);
int encryptMessage(int message, int e, int n);
long long mod_pow(long long base, long long exponent, long long modulus);

int main(void){
	char lookup[]= {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
	int input[8];
	int input1[4];
	int input2[4];
	uint32_t key;
	//unsigned char display_val;
	
	int privKey = generatePrivateKey(31, 3599);
	
	LCD_init();
	LCD_command(0x01);
	LCD_command(0x80);   //set cursor at 1st line
	
	keypad_init();
	
	print_fnc("ENTER MESSAGE:");
	LCD_command(0xC0); //set cursor at 2nd line
	for(int i=0; i<8; i++){
		while(keypad_getkey() == 0){ }
		Delay(200000);
		key = keypad_getkey();
		LCD_data(lookup[key-1]);
		input[i] = (int)(lookup[key-1])-'0';
		//display_val = (char)lookup[key-1];
		//print_fnc(&display_val);
		
		Delay(400000);
		
	}
	
	Delay(500000);
	LCD_command(0x01);
	
	print_fnc("SELECT MODE:");
	Delay(1000000);
	while(1) {
		while(keypad_getkey() == 0) {}
			Delay(200000);
			key = keypad_getkey();
			LCD_data(lookup[key-1]);
			Delay(400000);
			//ENCRYPTION
			if(key == 4) {
				Delay(1000000);
				LCD_command(0x01);
				
				//Show Encrypted
				LCD_command(0x80);
				print_fnc("ENC. MESSAGE:");
				LCD_command(0xC0);
				//separate_blocks(input, 8, input1, input2);
				
			}
			
			//DECRYPTION
			else if(key == 15) {
				Delay(1000000);
				LCD_command(0x01);
				
			}
	}
	
	
}

//***************DATA STRUCTURE UTILS***************

int array_to_int4(int _input[4]) {
    int sum = 0;
    for (int i=0; i<4; i++) {
        sum += _input[i] * pow(10, (3-i));
    }
    return sum;
}

void separate_blocks(int original[], int length, int firstHalf[], int secondHalf[]) {
    int halfLength = length / 2;

    // Copy the first half of the original array to the firstHalf array
    for (int i = 0; i < halfLength; i++) {
        firstHalf[i] = original[i];
    }

    // Copy the second half of the original array to the secondHalf array
    for (int i = 0; i < halfLength; i++) {
        secondHalf[i] = original[halfLength + i];
    }
}

//****************CRYPTOGRAPHY SECTION**********************

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
		return 0;
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

    *x = old_s;
    *y = t;
    *gcd = old_r;
}
