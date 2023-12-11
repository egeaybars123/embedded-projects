#include <MKL25Z4.H>
#include <stdio.h>
#include <math.h>
#include "utils.h"

#define MASK(x) (1UL << (x))

void Delay(volatile unsigned int time_del);
void blink_RGBLED(void);

int main(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	blink_RGBLED();
	
//	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
//	
//	PORTA->PCR[1] &= ~PORT_PCR_MUX_MASK;
//	PORTA->PCR[1] |= PORT_PCR_MUX(1);
//	
//	PTA->PDDR |= MASK(1);
//	while(1) {
//		PTA->PTOR = MASK(1);
//		//Delay(5000000);
//		//PTA->PDOR = MASK(1);
//		Delay(5000000);
//	}
}

void blink_RGBLED(void) {
	//PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK; //Red Cathode
	//PORTB->PCR[18] |= PORT_PCR_MUX(1);
	PORTB->PCR[19] &= ~PORT_PCR_MUX_MASK; //Green Cathode
	PORTB->PCR[19] |= PORT_PCR_MUX(1);
	
	PTB->PDDR |= MASK(19);
	
	while(1) {
		PTB->PTOR = MASK(19);
		Delay(2500000);
	}
}
