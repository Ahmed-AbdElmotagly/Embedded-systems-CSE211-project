#include <stdio.h>
#include <tm4c123gh6pm.h>
void SystemInit () {} ;

void port_Init (void) {                     // this function is to activate the ports which will be used inshaa allah
	SYSCTL_RCGCGPIO_R = 0x20 ;                // to activate port F 
	while ((SYSCTL_PRGPIO_R & 0x20)==0){}     // waiting for the activation 
	GPIO_PORTF_LOCK_R = 0x4C4F434B ;          // UNLOCKING PORT F 
}
// ##################################################################################################################################
void LED_Init (void) {                    	// this function is to unlock the pins and enable thhem as digital output
	GPIO_PORTF_DATA_R   = 0x02 ;                 // on 
	GPIO_PORTF_PUR_R		= 0x02 ;                 // use pull up resistance
	GPIO_PORTF_CR_R  		= 0x02 ;                 // unlock pin F1 (the red led )
	GPIO_PORTF_DIR_R 		= 0x02 ;                 // pin F1 as output
	GPIO_PORTF_DEN_R 		= 0x02 ;                 // to enable digital
	GPIO_PORTF_AFSEL_R 	= ~(0x02);               // disable Alternate function select 
	GPIO_PORTF_AMSEL_R 	= ~(0x02);               // disable Analog
	GPIO_PORTF_PCTL_R 	= 0x00000000; 					 // regular digital function	
}
// ####################################################################################################################################
int main() {
	
}


/*#define GPIO_PORTF_DATA_R         (*((volatile unsigned long*)0x400253FC))
#define GPIO_PORTF_DIR_R          (*((volatile unsigned long*)0x40025400))
#define GPIO_PORTF_AFSEL_R        (*((volatile unsigned long*)0x40025420))
#define GPIO_PORTF_PUR_R          (*((volatile unsigned long*)0x40025510))
#define GPIO_PORTF_DEN_R          (*((volatile unsigned long*)0x4002551C))
#define GPIO_PORTF_CR_R           (*((volatile unsigned long*)0x40025524))
#define GPIO_PORTF_AMSEL_R        (*((volatile unsigned long*)0x40025528))
#define GPIO_PORTF_PCTL_R         (*((volatile unsigned long*)0x4002552C))
#define GPIO_PORTF_LOCK_R         (*((volatile unsigned long*)0x40025520))
#define SYSCTL_RCGCGPIO_R         (*((volatile unsigned long*)0x400FE608))
#define SYSCTL_PRGPIO_R           (*((volatile unsigned long*)0x400FEA08)) */

