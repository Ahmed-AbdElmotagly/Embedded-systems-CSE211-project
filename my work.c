#include <stdio.h>
#include <tm4c123gh6pm.h>
#include <stdint.h>
#include <stdlib.h>
#define RED_LED  0x02
#define BLUE_LED 0x04
#define GREEN_LED 0x08
#define ALL 0xFF
#define LCD_SPECIAL 0x01 // for the write and data configuration
#define CLEAR 0x01 
#define LINE1 0x80
#define LINE2 0xC0
#define CURSOR_STEP 0x06
#define WAKEUP 0x30
#define ENABLE 0x04

void SystemInit () {} ;

void port_Init (void) {                     // this function is to activate the ports which will be used inshaa allah
	SYSCTL_RCGCGPIO_R = 0x20 ;                // to activate port F 
	while ((SYSCTL_PRGPIO_R & 0x20)==0){}     // waiting for the activation 
	GPIO_PORTF_LOCK_R = 0x4C4F434B ;          // UNLOCKING PORT F 
		
}
// ##################################################################################################################################
void  LED_Init (char data) {                   	// this function is to turn on Led using char for 8 bits
	GPIO_PORTF_PUR_R		|= data ;                 // use pull up resistance
	GPIO_PORTF_CR_R  		|= data ;                 // unlock pin F1 (the red led )
	GPIO_PORTF_DIR_R 		|= data ;                 // pin F1 as output
	GPIO_PORTF_DEN_R 		|= data ;                 // to enable digital
	GPIO_PORTF_AFSEL_R 	&= ~(data);               // disable Alternate function select 
	GPIO_PORTF_AMSEL_R 	&= ~(data);               // disable Analog
	GPIO_PORTF_PCTL_R 	&= 0x00000000;            // disable special functions
	GPIO_PORTF_DATA_R   |= data ;                 // on 
}
// ####################################################################################################################################
	
void	LCD_DATA (char DUAL){													// this function for the data to be placed on  pins of port B to connect the LCD
	GPIO_PORTB_DATA_R   |= DUAL ;                 // connect the pins of the lcd to port B pins in the sequence PB0 to D0 ..
	GPIO_PORTB_DIR_R 		|= ALL ;									// the 8 pins of port B as OUTPUT
	GPIO_PORTB_DEN_R 		|= ALL ;                  // to enable digital
	GPIO_PORTB_CR_R  		|= ALL ;                 // unlock ALL pins of port B to be available for the LCD 
	GPIO_PORTB_AMSEL_R 	&= 0x00000000;            // disable Analog
	GPIO_PORTB_PCTL_R 	&= 0x00000000;            // disable special functions
	GPIO_PORTB_AFSEL_R 	&= 0x00000000;            // disable Alternate function select
	}
void LCD_SETUP (char DUAL){													// this function is to setup write and data configuration with port E
	// CONNECT RS of LCD to PE3 ,, CONNECT R/W to PE1 ,, CONNECT ENABLE to PE2
	GPIO_PORTE_CR_R  		|= LCD_SPECIAL ;          // unlock pin E0 
	GPIO_PORTE_DIR_R 		|= LCD_SPECIAL ;          // pin E0 as output
	GPIO_PORTE_DEN_R 		|= LCD_SPECIAL ;          // to enable digital
	GPIO_PORTE_AFSEL_R 	&= 0x00000000;               // disable Alternate function select 
	GPIO_PORTE_AMSEL_R 	&= 0x00000000;               // disable Analog
	GPIO_PORTE_PCTL_R 	&= 0x00000000;            // disable special functions
	GPIO_PORTE_DATA_R   |= DUAL ;                 //
}
void delay (void){
	int t=1067;
while (t) {t--;}
       //for ( d = 0; d <= 10000; d++)
       //{}
			 
GPIO_PORTE_DATA_R   &= ~ENABLE ;
}

void LCD_WORK (char LCD1){//,char LCD2,char LCD3){
// LCD_SETUP (0x04);  // started command and write
// LCD_DATA (WAKEUP);
// LCD_DATA (CLEAR);  // cleared screen
LCD_DATA (0x38);    // 2 lines 8-bits
LCD_DATA (LINE2);  // moved cursor to line 2
LCD_DATA (0x0F);    //to blink cursor
LCD_SETUP (0x08);  // started data and write
LCD_DATA (LCD1);			// write 
LCD_SETUP (ENABLE);  // enable
delay ();
/*LCD_SETUP (0x00);  // started command and write
LCD_DATA (CURSOR_STEP);  // move cursor to right one step
LCD_SETUP (0x08);  // started data and write
LCD_DATA (LCD2);			// write 
LCD_SETUP (ENABLE);  // enable
delay();
LCD_SETUP (0x00);  // started command and write
LCD_DATA (CURSOR_STEP);  // move cursor to right one step
LCD_SETUP (0x08);  // started data and write
LCD_DATA (LCD3);			// write 
LCD_SETUP (ENABLE);  // enable
delay();
LCD_SETUP (0x00);  // started command and write
LCD_DATA (LINE1);  // move to line 1
LCD_SETUP (0x08);  // started data and write
LCD_DATA ('D');			// write 
LCD_SETUP (ENABLE);  // enable
delay();
LCD_SETUP (0x00);  // started command and write
LCD_DATA (CURSOR_STEP);  // move cursor to right one step
LCD_SETUP (0x08);  // started data and write
LCD_DATA ('I');			// write 
LCD_SETUP (ENABLE);  // enable
delay();
LCD_SETUP (0x00);  // started command and write
LCD_DATA (CURSOR_STEP);  // move cursor to right one step
LCD_SETUP (0x08);  // started data and write
LCD_DATA ('S');			// write 
LCD_SETUP (ENABLE);  // enable
delay();
LCD_SETUP (0x00);  // started command and write
LCD_DATA (CURSOR_STEP);  // move cursor to right one step
LCD_SETUP (0x08);  // started data and write
LCD_DATA ('T');			// write 
LCD_SETUP (ENABLE);  // enable
delay();
LCD_SETUP (0x00);  // started command and write
LCD_DATA (CURSOR_STEP);  // move cursor to right one step
LCD_SETUP (0x08);  // started data and write
LCD_DATA ('A');			// write 
LCD_SETUP (ENABLE);  // enable
delay();
LCD_SETUP (0x00);  // started command and write
LCD_DATA (CURSOR_STEP);  // move cursor to right one step
LCD_SETUP (0x08);  // started data and write
LCD_DATA ('N');			// write 
LCD_SETUP (ENABLE);  // enable
delay();
LCD_SETUP (0x00);  // started command and write
LCD_DATA (CURSOR_STEP);  // move cursor to right one step
LCD_SETUP (0x08);  // started data and write
LCD_DATA ('C');			// write 
LCD_SETUP (ENABLE);  // enable
delay();
LCD_SETUP (0x00);  // started command and write
LCD_DATA (CURSOR_STEP);  // move cursor to right one step
LCD_SETUP (0x08);  // started data and write
LCD_DATA ('E');			// write 
LCD_SETUP (ENABLE);  // enable
delay();*/
}
// ####################################################################################################################################
int main() {
	port_Init();
LED_Init (RED_LED);
//LCD_WORK (1);
	
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
