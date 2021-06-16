#include <stdio.h>
#include <tm4c123gh6pm.h>
#include <stdlib.h>
#include <Math.h>
#include <string.h>

#define RED_LED 0x02
#define BLUE_LED 0x04
#define GREEN_LED 0x08
#define ALL 0x0E
#define pi 3.14159265358979323846
#define R 6371000 // raduis of Earth


void SystemInit(){};

void portF_Init(void)
{							  // this function is to activate the ports which will be used inshaa allah   ====> will be removed
	SYSCTL_RCGCGPIO_R |= 0x20; // to activate ports  F
	while ((SYSCTL_PRGPIO_R & 0x20) == 0)	{}								// waiting for the activation
	GPIO_PORTF_LOCK_R = 0x4C4F434B; // UNLOCKING PORT F
		
}
// ##################################################################################################################################
void LED_Init(char data)
{						 // this function is to turn on Led using char for 8 bits
		
	GPIO_PORTF_PUR_R =0x00;		 // use pull up resistance ===> will be changed
	GPIO_PORTF_CR_R |= data;		 // unlock pin F1 (the red led )   
	GPIO_PORTF_DIR_R |= data;		 // pin F1 as output 
	GPIO_PORTF_DEN_R |= data;		 // to enable digital  
	GPIO_PORTF_AFSEL_R &= ~(data);		 // disable Alternate function select   
	GPIO_PORTF_AMSEL_R &= ~(data);		 // disable Analog  
	GPIO_PORTF_PCTL_R = 0x00000000;	 // disable special functions
	GPIO_PORTF_DATA_R = data;		 // on ====> will be changed & replaced last for logic

}
// ####################################################################################################################################
// port E ===> (E1 , E2 , E3) for controll
// E1 --> select register    ,  E2 ----> r/w , E3 ---->
// Port B ====> For data D0 ===> D7

// 2 functions for Delay
void delayMs(int ms)
{

	int i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 3180; j++)
		{
		} // delay for 1 Ms
}
// ##################################################################################################################################

void delayUs(int us)
{

	int i, j;
	for (i = 0; i < us; i++)
		for (j = 0; j < 3; j++)
		{
		}
}
// ##################################################################################################################################


void LCD_Command(unsigned char cmnd)
{

	GPIO_PORTE_DATA_R = 0; //  E1 , E2 ,E3 ----> RS=0 ,RW=0, EN=0 .
	GPIO_PORTB_DATA_R = cmnd;
	GPIO_PORTE_DATA_R = 0x08; // E3  --> Enable pulse E3
	GPIO_PORTE_DATA_R = 0;

	if (cmnd < 4)
		delayMs(2);
	else
		delayUs(40);
}
//##############################################################################################################################################
void LCD_Data_ch(unsigned char char_data) // LCD data for writting function
{

	GPIO_PORTE_DATA_R = 0x02; //  A5--> RS=1 command register. E1
	GPIO_PORTB_DATA_R = char_data;
	GPIO_PORTE_DATA_R = (0x08 | 0x02); // Pulse E
	delayMs(1);
	GPIO_PORTE_DATA_R = 0;
	delayMs(50);
}
//############################################################################################################################################

void LCD_Initalization(void) // LCD Initialize function
{
	
	// PORT E lcd controls ====> TRUE
	SYSCTL_RCGCGPIO_R |= 0x00000010;
  while ((SYSCTL_PRGPIO_R&0x10) == 0){};
	GPIO_PORTE_LOCK_R = 0x4C4F434B;
	GPIO_PORTE_CR_R |= 0x3E;
	GPIO_PORTE_DIR_R = 0x0E;
	GPIO_PORTE_DEN_R = 0x3E;
	GPIO_PORTE_AMSEL_R = 0x00;
	GPIO_PORTE_AFSEL_R = 0x30;
	GPIO_PORTE_PCTL_R = 0x00110000;
  GPIO_PORTE_PUR_R = 0x00;

	// PORT B lcd data ======> TRUE
	SYSCTL_RCGCGPIO_R |= 0x00000002;
  while ((SYSCTL_PRGPIO_R&0x02) == 0){};
	GPIO_PORTB_LOCK_R = 0x4C4F434B;
	GPIO_PORTB_CR_R |= 0xFF;
	GPIO_PORTB_DIR_R = 0xFF;
	GPIO_PORTB_DEN_R = 0xFF;
	GPIO_PORTB_AMSEL_R = 0x00;
	GPIO_PORTB_AFSEL_R = 0x00; 
	GPIO_PORTB_PCTL_R = 0x00000000;
  GPIO_PORTB_PUR_R = 0x00;

	delayMs(20); // LCD Power ON delay always more than 15Ms

	LCD_Command(0x30);
	delayMs(1);
	LCD_Command(0x38); // Initialization of 16X2 LCD in  8_bit mode
	delayMs(1);
	LCD_Command(0x0C); // Display ON Cursor OFF
	delayMs(1);
	LCD_Command(0x06); // Auto Increment cursor
	delayMs(1);
	LCD_Command(0x01); // Clear display
	delayMs(1);
	LCD_Command(0x80); // Cursor at home position
	delayMs(1);
	return;
}
// ##################################################################################################################################

void LCD_String(char *str) // Send string to LCD function
{
	int i;
	for (i = 0; str[i] != 0; i++) // Send each char of string till the NULL
	{
		LCD_Data_ch(str[i]);
	}
	delayMs(100);
	return;
}
// ##################################################################################################################################


void LCD_String_xy(char row, char pos, char *str) // Send string to LCD with xy position
{
	if (row == 0 && pos < 16)
		LCD_Command((pos & 0x0F) | 0x80); // Command of first row and required position<16
	else if (row == 1 && pos < 16)
		LCD_Command((pos & 0x0F) | 0xC0); // Command of second row and required position<16
	LCD_String(str);					  // Call LCD string function
}
// ##################################################################################################################################


void LCD_Clear()
{
	LCD_Command(0x01); // clear display
	LCD_Command(0x80); // return cursor at home position
}
// ##################################################################################################################################

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::/
/:                       Function prototypes                        :/
/:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
float deg_to_rad(float deg)
{
	return deg * (pi / 180);
}
float rad_to_deg(float rad)
{
	return rad * (180 / pi);
}

float distance(float lat1, float longt1, float lat2, float longt2)
{
	float dist, angleOfLat, angleOfLongt;
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


float distance_total (float lat_old, float lon_old, float lat_new, float lon_new)
{ 
	// used to accumlate the distance to get the total
	float Total=0;
	if(lon_old!=0 && lat_old !=0){ 
		Total = distance(lat_old,lon_old,lat_new,lon_new);
	}
	return Total;
}



// ##################################################################################################################################

void num_To_Ch(int x){
int a,b,c;
a = x % 10; // to get el A7ad
x/= 10;  // now x====> 34rat & me2at
b = x % 10; // to get el 34rat
x/= 10;  // now x====> me2at
c = x;   // to get el me2at
// now we will convert a,b,c from integer to char by adding '0' ===> (Type casting) 
LCD_Data_ch((c+'0'));  // print me2at num
LCD_Data_ch((b+'0'));  // print 34rat num
LCD_Data_ch((a+'0'));  // print A7ad num
LCD_Data_ch(' ');      // print space
LCD_Data_ch('m');      // print unit
}
// ##################################################################################################################################

void uart_Init(void){
		SYSCTL_RCGCUART_R |= 	SYSCTL_RCGCUART_R5  ; // ENABLE UART5 CLOCK
		SYSCTL_RCGCGPIO_R |=	SYSCTL_RCGCGPIO_R5  ; // ENABLE PORT E CLOCK
		UART5_CTL_R &= ~(UART_CTL_UARTEN);					// DISABL UART TO CONFIGURE
		UART5_IBRD_R = 104;
		UART5_FBRD_R = 11;
		UART5_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
		UART5_CTL_R |= (UART_CTL_UARTEN|UART_CTL_RXE|UART_CTL_TXE);
		GPIO_PORTE_AFSEL_R |= 0x30;
		GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R&~(0xFF))|(GPIO_PCTL_PE4_U5RX | GPIO_PCTL_PE5_U5TX);
		GPIO_PORTE_DEN_R|=0x30;
	  GPIO_PORTE_AMSEL_R&=~0x30; // NEW ADDING
	}

unsigned char  uart_reciever(void)
	{
	while((UART5_FR_R & 0x10)!=0);
	return ((unsigned char)(UART5_DR_R & 0xFF));

	}


//#######################################################################################################################	
unsigned char counter; 
	unsigned char flag=0;  
	unsigned char* y;   
	unsigned char* x;  
	unsigned char* n;  
	unsigned char* e;  
	unsigned char* gps_fix;  
	float plat;  
	float plong;  
	unsigned char lat_int;  
	unsigned char long_int;  
	float lat_real;  
	float long_real;  
	unsigned char inc[50];  

void Parsing(float array[])
{
	
		while(flag==0){
			    counter = uart_reciever();
				if(counter =='$'){
					counter = uart_reciever();
				if(counter=='G'){
					counter = uart_reciever();
				if(counter=='P'){
					counter = uart_reciever();
	 		  if(counter=='G'){
					counter = uart_reciever();
				if(counter=='G'){
					counter = uart_reciever();
				if(counter=='A'){
					counter = uart_reciever();
				if(counter==','){
					int i; 
					for( i=0;i<50;i++){
				  	inc[i]= uart_reciever(); // if gps sends "$GPGGA," we will take the data after it
					}
				}
				}
				}
	  		}
				}
				}
				}
			y = strtok(inc, ","); 
			y = strtok(NULL, ",");
			n = strtok(NULL, ",");
			x = strtok(NULL, ",");
			e = strtok(NULL, ",");
		  gps_fix = strtok(NULL, ",");
			if(gps_fix[0]=='1'){ 
				flag=1;
			}
		}
		plat= strtod(y,NULL); 
		plong= strtod(x,NULL);  
		plat=plat/100;
		plong=plong/100;
		lat_int=(unsigned char)plat;
		long_int=(unsigned char)plong;
		plat=plat-lat_int;
		plong=plong-long_int;
		plat=(plat*100)/60;
		plong=(plong*100)/60;
		lat_real=lat_int+plat; // final latitude value
		long_real=long_int+plong; // final longitude value
		if(n[0] == 'S'){
			lat_real*=-1;
		}
		if(e[0]=='W'){
			long_real*=-1;
		}
		array[0]=lat_real;
		array[1]=long_real;
}



// ##################################################################################################################################


float Total_dist=0;
	float old_lat=0; 
	float old_lang=0;
	float array[2];
int main(void)
{
	LCD_Initalization();
	portF_Init();
	uart_Init();
	

	
	
	 	
	LCD_Command(0x01);
	while(1)
	{
	Parsing(array);
		Total_dist+= distance_total(old_lat, old_lang , array[0], array[1]);		
		old_lat=array[0];
		old_lang=array[1];
	LCD_Clear();
	LCD_String("Distance = ");
	LCD_Command(0xC0);
	num_To_Ch((int)Total_dist);   // print distance in 2nd line of LCD
			
	if(Total_dist>=100){
		LED_Init(ALL);
	}
	delayMs(1000);
}

	return 0;
}
/* 
// keypad code
void keypad_init(void)
{
SYSCTL_RCGCGPIO_R |= 0x04; /* enable clock to GPIOC 
SYSCTL_RCGCGPIO_R |= 0x10; /* enable clock to GPIOE 
 
GPIO_PORTE_DIR_R |= 0x0F; /* set column pins 3-1 as output 
GPIO_PORTE_DEN_R |= 0x0F; /* set column pins 3-1 as digital pins 

 
GPIO_PORTC_DIR_R &= ~0xF0; /* set row pin 7-4 as input 
GPIO_PORTC_DEN_R |= 0xF0; /* set row pin 7-4 as digital pins
GPIO_PORTC_PUR_R |= 0xF0; /* enable pull-ups for pin 7-4 *
}
 
/* This is a non-blocking function to read the keypad.
/* If a key is pressed, it returns the key label in ASCII encoding. Otherwise, it returns a 0 (not ASCII 0). 
unsigned char keypad_getkey(void)
{
const unsigned char keymap[4][3] = {
{ '1', '2', '3'},
{ '4', '5', '6'},
{ '7', '8', '9'},
{ '*', '0', '#'},
};
 
int row, col;
 
/* check to see any key pressed first 
GPIO_PORTE_DATA_R = 0; /* enable all coluns 
row = GPIO_PORTC_DATA_R & 0xF0; /* read all rows 
if (row == 0xF0) return 0; /* no key pressed 
/* If a key is pressed, it gets here to find out which key. 
/* Although it is written as an infinite loop, it will take one of the breaks or return in one pass.
while (1)
{
col = 0;
GPIO_PORTE_DATA_R = 0x0D; /* enable column 1 
delayUs(2); /* wait for signal to settle 
row = GPIO_PORTC_DATA_R & 0xF0;
if (row != 0xF0) break;
 
col = 1;
GPIO_PORTE_DATA_R = 0x0B; /* enable col 2 
delayUs(2); /* wait for signal to settle 
row = GPIO_PORTC_DATA_R & 0xF0;
if (row != 0xF0) break;
 
col = 2;
GPIO_PORTE_DATA_R = 0x07; /* enable col 3 
delayUs(2); /* wait for signal to settle 
row = GPIO_PORTC_DATA_R & 0xF0;
if (row != 0xF0) break;
 

 
return 0; /* if no key is pressed 
}
 
/* gets here when one of the rows has key pressed 
if (row == 0xE0) return keymap[0][col]; /* key in row 0 
if (row == 0xD0) return keymap[1][col]; /* key in row 1 
if (row == 0xB0) return keymap[2][col]; /* key in row 2 
if (row == 0x70) return keymap[3][col]; /* key in row 3 
return 0; /* just to be safe 
}
unsigned char keypad_kbhit(void)
{
int col;
 
/* check to see any key pressed 
GPIO_PORTE_DATA_R = 0; /* enable all rows 
col = GPIO_PORTC_DATA_R & 0xF0; /* read all columns 
if (col == 0xF0)
return 0; /* no key pressed 
else
return 1; /* a key is pressed 
}
*/
