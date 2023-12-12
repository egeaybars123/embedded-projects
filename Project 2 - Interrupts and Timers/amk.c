#include <MKL25Z4.h>

#define T 0.5f + (LastDigitOfID * 0.1f)

volatile uint8_t binaryCounter = 0;
volatile uint8_t blinkFlag = 0;

void delay(uint32_t milliseconds) {
    // Initialize SysTick with appropriate settings for delays
    SysTick->LOAD = (SystemCoreClock / 1000) * milliseconds - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    // Wait until the COUNTFLAG is set
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
        ;

    // Disable SysTick
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Init(void) {
    // Initialize SysTick for your required T time
    SysTick->LOAD = (uint32_t)(T * SystemCoreClock) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void RGB_LED_Init(void) {
    // Initialize onboard RGB LED
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // Enable clock for Port B
    PORTB->PCR[18] |= PORT_PCR_MUX(1);   // Set PTB18 as GPIO
    PORTB->PCR[19] |= PORT_PCR_MUX(1);   // Set PTB19 as GPIO

    // Set PTB18 and PTB19 as outputs
    GPIOB->PDDR |= (1 << 18) | (1 << 19);
}

void External_LEDs_Init(void) {
    // Initialize external LEDs
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; // Enable clock for Port C
    PORTC->PCR[0] |= PORT_PCR_MUX(1);    // Set PTC0 as GPIO
    PORTC->PCR[1] |= PORT_PCR_MUX(1);    // Set PTC1 as GPIO
    PORTC->PCR[2] |= PORT_PCR_MUX(1);    // Set PTC2 as GPIO
    PORTC->PCR[3] |= PORT_PCR_MUX(1);    // Set PTC3 as GPIO

    // Set PTC0-PTC3 as outputs
    GPIOC->PDDR |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
}

void Buttons_Init(void) {
    // Initialize buttons
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; // Enable clock for Port A
    PORTA->PCR[1] |= PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; // Set PTA1 as GPIO with pull-up resistor
    PORTA->PCR[2] |= PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; // Set PTA2 as GPIO with pull-up resistor
    PORTA->PCR[3] |= PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; // Set PTA3 as GPIO with pull-up resistor

    // Set PTA1-PTA3 as inputs
    GPIOA->PDDR &= ~((1 << 1) | (1 << 2) | (1 << 3));
}

void ISR1(void) {
    // Implement ISR1 to control onboard RGB LED based on button presses
    if (GPIOA->PDIR & (1 << 1)) {
        // Button 1 pressed: Turn onboard LED to green, blink 5 times
        for (int i = 0; i < 5; i++) {
            GPIOB->PDOR |= (1 << 18); // Turn LED to green
            delay(500);               // 500 ms delay
            GPIOB->PDOR &= ~(1 << 18); // Turn off LED
            delay(500);               // 500 ms delay
        }
    } else if (GPIOA->PDIR & (1 << 2)) {
        // Button 2 pressed: Halt blinking for 2 seconds
        delay(2000); // 2000 ms delay
    }

    blinkFlag = 1; // Set the flag to change the LED blinking behavior
}

void ISR2(void) {
    // Implement ISR2 to drive external LEDs with a 4-bit binary counter
    binaryCounter++;

    if (binaryCounter > 15) {
        binaryCounter = 0;
    }

    // Display binaryCounter on external LEDs
    GPIOC->PDOR = (binaryCounter & 0x0F);
}

void SysTick_Handler(void) {
    // Implement SysTick handl"er for blinking the red LED
    if (!blinkFlag) {
        // Red LED blinks with 1s period and 50% duty cycle
        GPIOB->PTOR = (1 << 19); // Toggle red LED
    } else {
        // Red LED blinks with 2Ts period and 50% duty cycle
        GPIOB->PTOR = (1 << 19); // Toggle red LED
        delay((uint32_t)(2 * T * 1000)); // Delay for 2Ts
    }
}

int main(void) {
    SysTick_Init();
    RGB_LED_Init();
    External_LEDs_Init();
    Buttons_Init();

    while (1) {
        // Main loop - your application logic goes here
    }
}