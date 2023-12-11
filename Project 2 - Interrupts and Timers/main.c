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
void init_LEDs(void);
void PORTA_IRQHandler(void);
void PORTD_IRQHandler(void);

static volatile unsigned count = 0;

int main(void){
	//Enable clock on multiple ports
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	__enable_irq();
	init_button();
	init_counter();
	init_LEDs();
	
	
	while(1){
		blink_red();
		__WFI(); //wait for interrupt
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
	
	NVIC_SetPriority(PORTA_IRQn, 0x80);
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);
}

void init_LEDs(void) {
	PORTB->PCR[0] &= ~PORT_PCR_MUX_MASK;
	PORTB-> PCR[0] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTB-> PCR[1] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[2] &= ~PORT_PCR_MUX_MASK;
	PORTB-> PCR[2] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[3] &= ~PORT_PCR_MUX_MASK;
	PORTB-> PCR[3] |= PORT_PCR_MUX(1);
	
	PTB->PDDR |= MASK(0) | MASK(1) | MASK(2)| MASK(3); //set as output
}

void init_counter(void) {
	PORTD->PCR[4] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);
	PTD->PDDR &= ~MASK(4);
	
	NVIC_SetPriority(PORTD_IRQn, 0xC0);
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn);
}

void blink_red(void) {
	PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK; //Red Cathode
	PORTB->PCR[18] |= PORT_PCR_MUX(1);
	
	PTB->PDDR |= MASK(18);
	while(1) {
		PTB->PTOR |= MASK(18);
		Delay(1500000);
	}
}

void blink_green(void) {
	PORTB->PCR[19] &= ~PORT_PCR_MUX_MASK; //Green Cathode
	PORTB->PCR[19] |= PORT_PCR_MUX(1);
	
	PTB->PDDR |= MASK(19);
	
	int count = 0;
	while(count < 10) {
		PTB->PTOR |= MASK(19);
		Delay(1500000);
		count++;
	}
	PTB->PSOR = MASK(19);
}

//***************** INTERRUPT HANDLERS *****************

void PORTA_IRQHandler(void) {
	//clear pending interrupts
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	
	if(PORTA->ISFR & MASK(1)) {
		blink_green();
	}
	else if(PORTA->ISFR & MASK(2)) {
		Delay(5500000);
	}
	
	//clear status flags.
	PORTA->ISFR = 0xffffffff;
}

void PORTD_IRQHandler(void) {
	//clear pending interrupts
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	
	if(PORTD->ISFR & MASK(4)) {
		count++;
		PTB->PDOR = (count << 0);
	}
	if (count == 16) {
		count = 0;
	}
	
	//clear status flags.
	PORTD->ISFR = 0xffffffff;
}
