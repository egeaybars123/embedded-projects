#include <stdio.h>
#include <math.h>
void int_to_array(int _input, int _input1, char _arr[16]);
void array_to_int4(int _input[4]);
void separate_blocks(int original[], int length, int firstHalf[], int secondHalf[]);
int array_to_int(int input[4]);

int main() {
    int input[8] = {2,2,1,0,2,1,6,2};
    int input1[4]; 
    int input2[4];  
    char display[16];
    int inp1_sum;
    int inp2_sum;
    //array_to_int8(input);
    separate_blocks(input, 8, input1, input2);
   /*  for (int i=0; i < 4; i++) {
        printf("%d,", input1[i]);
    }
    for (int i=0; i < 4; i++) {
        printf("%d,", input2[i]);
    } */

    inp1_sum = array_to_int(input1);
    inp2_sum = array_to_int(input2);
    printf("Inp1 sum: %d\n", inp1_sum);
    printf("Inp2 sum: %d\n", inp2_sum);

    //array_to_int4(input1);
    //array_to_int4(input2);

    /* int_to_array(220, 2162, display);
    for (int i=0; i < 16; i++) {
        printf("%c,", display[i]);
    } */
    return 0;
}

int array_to_int(int input[4]) {
    int result = 0;
    for (int i=0; i<4; i++) {
        result *= 10;
        result += input[i];
    }
    return result;
}

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

void array_to_int8(int _input[8]) {
    int sum = 0;
    for (int i=0; i<8; i++) {
        sum += _input[i] * pow(10, (7-i));
    }
    printf("%d\n", sum);
}

void array_to_int4(int _input[4]) {
    int sum = 0;
    for (int i=0; i<4; i++) {
        sum += _input[i] * pow(10, (3-i));
    }
    printf("%d\n", sum);
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

//input1 - first 4 number block <---> input2 - last 4 number block
//DOES NOT WORK! WHY?!
/* void separate_blocks(int _input[8], int* _input1[4], int* _input2[4]) {
    for (int i=0; i<8; i++) {
        if (i < 5) {
            *_input1[i] = _input[i];
        }
        *_input2[i-4] = _input[i];
    }
} */

