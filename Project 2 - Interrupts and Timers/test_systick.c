#include <MKL25Z4.H>
#include <stdio.h>
#include <math.h>
#include "utils.h"

#define MASK(x) (1UL << (x))

void Delay(volatile unsigned int time_del);
void blink_green(void);
void blink_red(void);
void init_button(void);
void init_counter(void);
void Init_SysTick(void);
void init_LEDs(void);
void init_RGB(void);
void PORTA_IRQHandler(void);
void PORTD_IRQHandler(void);
void SysTick_Handler(void);
void delay(unsigned milliseconds);

static volatile unsigned count = 0;
volatile unsigned led_count = 0;
volatile unsigned flag = 0;
volatile unsigned flag_green = 0;

int main(void){
	//Enable clock on multiple ports
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	__disable_irq();
	init_button();
	init_RGB();
	init_counter();
	init_LEDs();
	Init_SysTick();
	__enable_irq();
	
	
	while(1){
		//blink_red();
		//delay(500);
		//__WFI(); //wait for interrupt
	}
	
}

//PORT A 1 - Button 1 Interrupt Pin (ISR1) - Green LED Blinking
//PORT A 2 - Button 2 Interrupt Pin (ISR1) - Halt blinking for 2 seconds
//PORT A 4 - Button 3 Interrupt Pin (ISR2) - Increase LED Counter <<<<------>>>> PORT E 0,1,2,3 chosen for the LEDs.

void init_button(void) {
	PORTA->PCR[1] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);
	PORTA->PCR[2] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);
	PTA->PDDR &= ~MASK(1);
	PTA->PDDR &= ~MASK(2);
	
	NVIC_SetPriority(PORTA_IRQn, 1);
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);
}

void init_LEDs(void) {
	PORTE->PCR[2] &= ~PORT_PCR_MUX_MASK;
	PORTE-> PCR[2] |= PORT_PCR_MUX(1);
	
	PORTE->PCR[3] &= ~PORT_PCR_MUX_MASK;
	PORTE-> PCR[3] |= PORT_PCR_MUX(1);
	
	PORTE->PCR[4] &= ~PORT_PCR_MUX_MASK;
	PORTE-> PCR[4] |= PORT_PCR_MUX(1);
	
	PORTE->PCR[5] &= ~PORT_PCR_MUX_MASK;
	PORTE-> PCR[5] |= PORT_PCR_MUX(1);
	
	PTE->PDDR |= MASK(2) | MASK(3) | MASK(4)| MASK(5); //set as output
}

void init_counter(void) {
	PORTD->PCR[4] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);
	PTD->PDDR &= ~MASK(4);
	
	NVIC_SetPriority(PORTD_IRQn, 0);
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn);
}

void init_RGB(void) {
	PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK; //Red Cathode
	PORTB->PCR[18] |= PORT_PCR_MUX(1);
	PTB->PDDR |= MASK(18);
	
//	PORTB->PCR[19] &= ~PORT_PCR_MUX_MASK; //Green Cathode
//	PORTB->PCR[19] |= PORT_PCR_MUX(1);
//	PTB->PDDR |= MASK(19);
}

void blink_red(void) {
	PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK; //Red Cathode
	PORTB->PCR[18] |= PORT_PCR_MUX(1);
	PTB->PDDR |= MASK(18);
	
	while(1) {
		PTB->PTOR |= MASK(18);
		delay(500);
	}
}

void blink_green(void) {
	PORTB->PCR[19] &= ~PORT_PCR_MUX_MASK; //Green Cathode
	PORTB->PCR[19] |= PORT_PCR_MUX(1);
	PTB->PDDR |= MASK(19);
	
	//int count = 0;
	while(led_count < 10) {
		PTB->PTOR = MASK(19);
		Delay(2500000);//delay(500);
		led_count++;
	}
	PTB->PSOR = MASK(19);
	led_count = 0;
}


void Init_SysTick(void) {
    // Initialize SysTick for your required T time
		SysTick->CTRL = 0; 
    SysTick->LOAD = (uint32_t)(0.7 * SystemCoreClock) - 1;
    SysTick->VAL = 0;
		NVIC_SetPriority(SysTick_IRQn, 2);
    SysTick->CTRL |=  SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void delay(unsigned milliseconds) {
    // Initialize SysTick with appropriate settings for delays
		SysTick->LOAD = (SystemCoreClock / 1000) * milliseconds - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    // Wait until the COUNTFLAG is set
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));

    // Disable SysTick
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	
}

//***************** SYSTICK HANDLER *****************

void SysTick_Handler(void) {
//    if (!flag) {
//        PTB->PTOR = MASK(18); 
//    } else {
//       PTB->PTOR = MASK(18);	
//    }
		
	PTB->PTOR = MASK(18);
}

//***************** INTERRUPT HANDLERS *****************

void PORTA_IRQHandler(void) {
	//clear pending interrupts
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	
	if(PORTA->ISFR & MASK(1)) {
		blink_green();
	}
	else if(PORTA->ISFR & MASK(2)) {
		Delay(2500000);//delay(2000);
	}
	flag = 1;
	//clear status flags.
	PORTA->ISFR = 0xffffffff;
}

void PORTD_IRQHandler(void) {
	//clear pending interrupts
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	
	if(PORTD->ISFR & MASK(4)) {
		count++;
		PTE->PDOR = (count << 2);
	}
	if (count == 16) {
		count = 0;
	}
	
	//clear status flags.
	PORTD->ISFR = 0xffffffff;
}
