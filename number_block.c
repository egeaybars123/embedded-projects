#include <stdio.h>
#include <math.h>

int main() {
    int input[8] = {2,2,1,0,2,1,6,2};
    int input_whole;
    int input1[4]; 
    int input2[4];  
    array_to_int8(input);
    separate_blocks(input, 8, input1, input2);
    for (int i=0; i < 4; i++) {
        printf("%d,", input1[i]);
    }
    for (int i=0; i < 4; i++) {
        printf("%d,", input2[i]);
    }

    array_to_int4(input1);
    array_to_int4(input2);
    return 0;
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

