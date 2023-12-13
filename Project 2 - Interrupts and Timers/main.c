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

volatile unsigned count = 0;
volatile unsigned led_count = 0;
volatile unsigned flag_t = 0;
volatile unsigned flag_green = 0;
volatile unsigned flag_halt = 0;
volatile unsigned timer_count = 0;

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
//PORT D 4 - Button 3 Interrupt Pin (ISR2) - Increase LED Counter <<<<------>>>> PORT E 2,3,4,5 chosen for the LEDs.

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
	
	PORTB->PCR[19] &= ~PORT_PCR_MUX_MASK; //Green Cathode
	PORTB->PCR[19] |= PORT_PCR_MUX(1);
}

void Init_SysTick(void) {
    // Initialize SysTick for your required T time
		SysTick->CTRL = 0; 
    SysTick->LOAD = 41000; //(uint32_t)(0.7 * SystemCoreClock) - 1;
    SysTick->VAL = 0;
		NVIC_SetPriority(SysTick_IRQn, 2);
    SysTick->CTRL |=  SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

//***************** SYSTICK HANDLER *****************

void SysTick_Handler(void) {
	++timer_count;
	if ((timer_count > 500) && !flag_t) {
		PTB->PDDR = MASK(18);		
		PTB->PTOR = MASK(18);
		timer_count = 0;
	}
	
	else if ((timer_count > 1400) && !flag_green && !flag_halt && flag_t) {
		PTB->PDDR = MASK(18);		
		PTB->PTOR = MASK(18);
		timer_count = 0;
	}
	
	else if ((timer_count > 300) && flag_green && !flag_halt) {
		PTB->PDDR = MASK(19);
		PTB->PTOR = MASK(19);
		
		timer_count = 0;
		led_count++;
		
		if (led_count > 10) {
			flag_green = 0;
			led_count = 0;
		}
	}
	
	else if ((timer_count > 1000) && flag_halt) {
		flag_halt = 0;
		timer_count = 0;
	}
}

//***************** INTERRUPT HANDLERS *****************

void PORTA_IRQHandler(void) {
	//clear pending interrupts
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	
	if(PORTA->ISFR & MASK(1)) {
		flag_green = 1;
		//blink_green();
	}
	else if(PORTA->ISFR & MASK(2)) {
		flag_halt = 1;
	}
	flag_t = 1;
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
	flag_t = 1;
	//clear status flags.
	PORTD->ISFR = 0xffffffff;
}
