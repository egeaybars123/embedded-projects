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
void int_to_array(int _input, int _input1, char _arr[16]);
int iterate_e(void);
int iterate_d(void);

int main(void){
	char lookup[]= {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
	char display_val[16]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	
	int input[8];
	int input1[4];
	int input2[4];
	int inp1, inp2;
	long long enc1, enc2;
	int e, d;
	uint32_t key;
	
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
		//while(keypad_getkey() == 0) {}
			Delay(200000);
			key = keypad_getkey();
			//ENCRYPTION
			if(key == 4) {
				Delay(300000);
				LCD_data(lookup[key-1]);
				Delay(300000);
				
				separate_blocks(input, 8, input1, input2);			
				inp1 = array_to_int4(input1);
				inp2 = array_to_int4(input2);
				//inp1 = 2210;
				//inp2 = 2162;
				e = iterate_e();
				enc1 = mod_pow(inp1, e, 3599);
				enc2 = mod_pow(inp2, e, 3599);
				int_to_array((int) enc1, (int) enc2, display_val);
				Delay(300000);
				
				LCD_command(0x01);
				LCD_command(0x80);
				print_fnc("ENC. MESSAGE:");
				
				LCD_command(0xC0);
				Delay(300000);
				
				for(int i=0; i<16; i++) {
					LCD_data(display_val[i]);
					Delay(100000);
				}
				
			}
			
			//DECRYPTION
			if(key == 16) {
				Delay(300000);
				LCD_data(lookup[key-1]);
				Delay(300000);
				
				separate_blocks(input, 8, input1, input2);			
				inp1 = array_to_int4(input1);
				inp2 = array_to_int4(input2);
				//inp1 = 2210;
				//inp2 = 2162;
				d = iterate_d();
				enc1 = mod_pow(inp1, d, 3599);
				enc2 = mod_pow(inp2, d, 3599);
				int_to_array((int) enc1, (int) enc2, display_val);
				Delay(300000);
				
				LCD_command(0x01);
				LCD_command(0x80);
				print_fnc("DEC. MESSAGE:");
				
				LCD_command(0xC0);
				//print_fnc("AA");
				Delay(300000);
				
				for(int i=0; i<16; i++) {
					LCD_data(display_val[i]);
					Delay(100000);
				}
				
				
			}
	}
}

//*************** LCD PRINT UTILS ******************
void int_to_array(int _input, int _input1, char _arr[16]) {
    int digitVal;
    int _input1Count=0;
    int _inputCount=0;

    int temp = _input;
    while (temp != 0) {
        temp /= 10;
        _inputCount++;
    }

    temp = _input1;
    while (temp != 0) {
        temp /= 10;
        _input1Count++;
    }

    for (int i=_input1Count-1; i >= 0; i--) {
        digitVal = _input1 % 10;
        _input1 /= 10;
        _arr[_inputCount+i] = (char) digitVal + '0';
    }

    for (int i=_inputCount-1; i >=0; i--) {
        digitVal = _input % 10;
        _input /= 10;
        _arr[i] = (char) digitVal + '0';
    }
}

//***************DATA STRUCTURE UTILS***************

int array_to_int4(int input[4]) {
    int result = 0;
    for (int i=0; i<4; i++) {
        result *= 10;
        result += input[i];
    }
    return result;
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

int iterate_e(void) {
    int x, y;
    int gcd = 0;
    int tryNumber = 1;
    while (gcd != 1) {
        tryNumber++;
        extendedEuclidean(tryNumber, 3480, &x, &y, &gcd);
    }
    return tryNumber;
}

int iterate_d(void) {
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
