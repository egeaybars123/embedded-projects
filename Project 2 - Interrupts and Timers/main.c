#include <MKL25Z4.H>
#include <stdio.h>
#include <math.h>
#include "utils.h"

#define MASK(x) (1UL << (x))

void Delay(volatile unsigned int time_del);
void blink_green(void);
void blink_red(void);
void init_button(void);
void PORTA_IRQHandler(void);

int main(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
	init_button();
	__enable_irq();
	
	while(1){
		blink_red();
		__WFI(); //wait for interrupt
	}
	
}

//PORT A 1 - Button 1 Interrupt Pin - Green LED Blinking
//PORT A 2 - Button 2 Interrupt Pin - Halt blinking for 2 seconds

void init_button(void) {
	PORTA->PCR[1] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);
	PORTA->PCR[2] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);
	PTA->PDDR &= ~MASK(1);
	PTA->PDDR &= ~MASK(2);
	
	NVIC_SetPriority(PORTA_IRQn, 128);
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);
}

void blink_red(void) {
	PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK; //Red Cathode
	PORTB->PCR[18] |= PORT_PCR_MUX(1);
	
	PTB->PDDR |= MASK(18);
	while(1) {
		PTB->PTOR = MASK(18);
		Delay(1500000);
	}
}

void blink_green(void) {
	PORTB->PCR[19] &= ~PORT_PCR_MUX_MASK; //Green Cathode
	PORTB->PCR[19] |= PORT_PCR_MUX(1);
	
	PTB->PDDR |= MASK(19);
	
	int count = 0;
	while(count < 10) {
		PTB->PTOR = MASK(19);
		Delay(1500000);
		count++;
	}
	PTB->PSOR = MASK(19);
}

//***************** INTERRUPT HANDLERS *****************

void PORTA_IRQHandler(void) {
	//clear pending interrupts
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	
	if(PORTA->ISFR & MASK(1)) {
		blink_green();
	}
	else if(PORTA->ISFR & MASK(2)) {
		Delay(5500000);
	}
	
	//clear status flags.
	PORTA->ISFR = 0xffffffff;
}
