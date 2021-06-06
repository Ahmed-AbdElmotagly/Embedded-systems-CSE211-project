#include <stdio.h>
#include <tm4c123gh6pm.h>
#include <stdlib.h>
#include <math.h>

#define RED_LED  0x02
#define BLUE_LED 0x04
#define pi 3.14159265358979323846
#define R 6371000 // raduis of Earth

void SystemInit () {} ;

void port_Init (void) {                     // this function is to activate the ports which will be used inshaa allah
	SYSCTL_RCGCGPIO_R = 0x37 ;                // to activate port F , E,C,B,A 
	while ((SYSCTL_PRGPIO_R & 0x37)==0){}     // waiting for the activation 
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
// port A ===> (A5 , A6 , A7) for controll
// A5 --> select register    ,  A6 ----> r/w , A7 ---->     
// Port B ====> For data D0 ===> D7

// 2 functions for Delay 
void delayMs(int ms){

    int i , j ;
    for(i=0;i<ms;i++)
    for(j=0;j<3180;j++)
    {} // delay for 1 Ms
}
void delayUs(int us){

    int i , j ;
    for(i=0;i<us;i++)
    for(j=0;j<3;j++)
    {} 
}	

void LCD_Command(unsigned char cmnd)
{
    GPIO_PORTB_DATA_R   |= cmnd ;
    GPIO_PORTA_DATA_R   &= ~(0x20) ;    //  A5--> RS=0 command register.
    GPIO_PORTA_DATA_R   &= ~(0x40) ;    //  A6--> RW=0 Write operation.
    GPIO_PORTA_DATA_R   |= (0x80) ;     // A7  --> Enable pulse 
   
    if(cmnd<4)
    delayMs(4);
    else
    delayUs(40);
//#######################################################################################################################################
void LCD_Data (unsigned char char_data)	    // LCD data for writting function 
{
	GPIO_PORTB_DATA_R |= char_data;
	GPIO_PORTA_DATA_R   |=(0x20) ;        //  A5--> RS=1 command register.
    GPIO_PORTA_DATA_R   &= ~(0x40) ;     //  A6--> RW=0 Write operation.
    GPIO_PORTA_DATA_R   |= (0x80) ;     // A7  --> Enable pulse 
	delayUs(40);
}
//############################################################################################################################################
void LCD_Initalization (void)				// LCD Initialize function 
{
	SYSCTL_RCGCGPIO_R |= 0x03 ;  	          // to activate port A & B
	while ((SYSCTL_PRGPIO_R & 0x03)==0){} 
	GPIO_PORTA_DIR_R |= 0xE0;		  	         // To Make LCD command port A direction as output 
	GPIO_PORTA_DEN_R |= 0xE0;		             	// To Make LCD command port A digital 
	GPIO_PORTA_AFSEL_R &= ~ 0XFF; 					 //Disable Analog mode on PORTA
	GPIO_PORTA_AMSEL_R &= ~ 0XFF;  					 //Disable Analog mode on PORTA
		
	GPIO_PORTB_DIR_R |= 0xFF;			           // To Make LCD Data port B direction as output 
	GPIO_PORTB_DEN_R |= 0xFF;                  // To Make LCD DATA port B digital  
  GPIO_PORTB_AFSEL_R &= ~ 0XFF; 					 //Disable Analog mode on PORTB
	GPIO_PORTB_AMSEL_R &= ~ 0XFF;  					 //Disable Analog mode on PORTB
		
    delayMs(20);			                       // LCD Power ON delay always more than 15Ms
	 
  LCD_Command(0x30);
	LCD_Command (0x38);		            // Initialization of 16X2 LCD in  8_bit mode 
	LCD_Command (0x0C);		           // Display ON Cursor OFF 
	LCD_Command (0x06);		          // Auto Increment cursor 
  LCD_Command (0x01);		         // Clear display 
	LCD_Command (0x80);		        // Cursor at home position 
}
	void LCD_String (char *str)		// Send string to LCD function 
{
	int i;
	for(i=0;str[i]!=0;i++)		// Send each char of string till the NULL 
	{
		LCD_Data (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)    // Send string to LCD with xy position 
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	           // Command of first row and required position<16 
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	         // Command of second row and required position<16 
	LCD_String(str);		                // Call LCD string function 
}

void LCD_Clear()
{
	LCD_Command (0x01);		// clear display 
	LCD_Command (0x80);		// return cursor at home position 
}
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
	
	
	// Example for testing function
	
	double lat1 = 30.0003, lat2 = 29.999, longt1 = 31.1768, longt2 = 31.1736;
	double dist = distance(lat1, longt1, lat2, longt2);

	printf("the distance %f", dist); //test the distance
	if (dist > 100){
	port_Init();
	LED_Init(RED_LED);
	}
}


	

