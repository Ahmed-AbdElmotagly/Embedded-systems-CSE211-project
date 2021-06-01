#include <stdio.h>
#include "tm4c123gh6pm.h"

void port_Init (void) {                  // this function is to activate the ports which will be used inshaa allah
SYSCTL_RCGCGPIO_R = 0x20 ;               // to activate port F bs mtl3 error w na l sra7a 3wz anam
	while ((SYSCTL_PRGPIO_R & 0x20)==0){}  // waiting for the activation 
 
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
