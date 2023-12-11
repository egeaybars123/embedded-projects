#include <stdio.h>

int main() {
    unsigned int count = 0;
    increment(&count);
    increment(&count);
    printf("Count: %d \n", count);

    unsigned int test = 0b10011;
    test |= (3 << 1);
    printf("Test: %d", test);
    
    return 0;
}

void increment(unsigned int* count) {
    ++*count;
}


