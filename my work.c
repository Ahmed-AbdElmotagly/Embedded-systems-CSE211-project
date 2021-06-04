#include <stdio.h>
#include <tm4c123gh6pm.h>
#include <stdlib.h>
#include <math.h>

#define RED_LED  0x02
#define BLUE_LED 0x04
#define ALL 0xFF
#define LCD_SPECIAL 0x01 // for the write and data configuration
#define CLEAR 0x01 
#define LINE1 0x80
#define LINE2 0xC0
#define CURSOR_STEP 0x06
#define WAKEUP 0x30
#define ENABLE 0x04
#define pi 3.14159265358979323846
#define R 6371000 // raduis of Earth

void SystemInit () {} ;

void port_Init (void) {                    	 // this function is to activate the ports which will be used inshaa allah
	SYSCTL_RCGCGPIO_R = 0x20 ;                // to activate port F 
	while ((SYSCTL_PRGPIO_R & 0x20)==0){}     // waiting for the activation 
	GPIO_PORTF_LOCK_R = 0x4C4F434B ;          // UNLOCKING PORT F 
}
// ##################################################################################################################################
void  LED_Init (char data) {                   		    	  // this function is to turn on Led using char for 8 bits
	GPIO_PORTF_DATA_R   		|= data ;                 // on  
	GPIO_PORTF_PUR_R		|= data ;                 // use pull up resistance
	GPIO_PORTF_CR_R  		|= data ;                 // unlock pin F1 (the red led )
	GPIO_PORTF_DIR_R 		|= data ;                 // pin F1 as output
	GPIO_PORTF_DEN_R 		|= data ;                 // to enable digital
	GPIO_PORTF_AFSEL_R 		&= ~(data);               // disable Alternate function select 
	GPIO_PORTF_AMSEL_R 		&= ~(data);               // disable Analog
	GPIO_PORTF_PCTL_R 		&= 0x00000000;            // disable special functions
}
// ####################################################################################################################################
void	LCD_DATA (char DUAL){				// this function for the data to be placed on  pins of port B to connect the LCD
	GPIO_PORTB_DATA_R   		|= DUAL ;       // connect the pins of the lcd to port B pins in the sequence PB0 to D0 ..
	GPIO_PORTB_DIR_R 		|= ALL ;	// the 8 pins of port B as OUTPUT
	GPIO_PORTB_DEN_R 		|= ALL ;        // to enable digital
	GPIO_PORTB_CR_R  		|= ALL ;        // unlock ALL pins of port B to be available for the LCD 
	GPIO_PORTB_AMSEL_R 	&= 0x00000000;            // disable Analog
	GPIO_PORTB_PCTL_R 	&= 0x00000000;            // disable special functions
	GPIO_PORTB_AFSEL_R 	&= 0x00000000;            // disable Alternate function select
	}
//#######################################################################################################################################

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::/
/:                       Function prototypes                        :/
/:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double deg_to_rad(double deg)
{
    return deg * (pi / 180);
}
double rad_to_deg(double rad)
{
    return rad * (180 / pi);
}

double distance(double lat1, double longt1, double lat2, double longt2)
{
    double dist, angleOfLat, angleOfLongt;
    if ((lat1 == lat2) && (longt1 == longt2))
    {
        return 0;
    }
	 // from formoula sheet "Haversine formula" to calculate the distance from here
	
    else
	{

		// from formoula sheet "HaverSine" to calculate the distance
		// declare some of parameters
		angleOfLat = (lat2 - lat1) / 2;
		angleOfLongt = (longt2 - longt1) / 2;

		dist = (2 * R * asin(sqrt((sin(deg_to_rad(angleOfLat)) * sin(deg_to_rad(angleOfLat))) + (cos(deg_to_rad(longt1)) * cos(deg_to_rad(longt2)) * sin(deg_to_rad(angleOfLongt)) * sin(deg_to_rad(angleOfLongt))))));

		return dist;
	}
}

	
	
	
 
int main() {
	port_Init();
	
	// Example for testing function
	
	double lat1 = 30.0003, lat2 = 29.999, longt1 = 31.1768, longt2 = 31.1736;
	double dist = distance(lat1, longt1, lat2, longt2);

	printf("the distance %f", dist); //tset the distance
	LED_Init(RED_LED);
	if (dist > 100)
	LED_Init(RED_LED);


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

