#include <stdio.h>
#include <tm4c123gh6pm.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define RED_LED 0x02
#define BLUE_LED 0x04
#define GREEN_LED 0x08
#define pi 3.14159265358979323846
#define R 6371000 // raduis of Earth


void SystemInit(){};

void port_Init(void)
{							  // this function is to activate the ports which will be used inshaa allah
	SYSCTL_RCGCGPIO_R = 0x37; // to activate ports  F,E,C,B,A
	while ((SYSCTL_PRGPIO_R & 0x37) == 0)	{}								// waiting for the activation
	GPIO_PORTF_LOCK_R = 0x4C4F434B; // UNLOCKING PORT F
}
// ##################################################################################################################################
void LED_Init(char data)
{						 // this function is to turn on Led using char for 8 bits
	GPIO_PORTF_DATA_R |= data;		 // on
	GPIO_PORTF_PUR_R |= data;		 // use pull up resistance
	GPIO_PORTF_CR_R |= data;		 // unlock pin F1 (the red led )
	GPIO_PORTF_DIR_R |= data;		 // pin F1 as output
	GPIO_PORTF_DEN_R |= data;		 // to enable digital
	GPIO_PORTF_AFSEL_R &= ~(data);		 // disable Alternate function select
	GPIO_PORTF_AMSEL_R &= ~(data);		 // disable Analog
	GPIO_PORTF_PCTL_R &= 0x00000000;	 // disable special functions
}
// ####################################################################################################################################
// port A ===> (A5 , A6 , A7) for controll
// A5 --> select register    ,  A6 ----> r/w , A7 ---->
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

	GPIO_PORTA_DATA_R = 0; //  E1 , E2 ,E3 ----> RS=0 ,RW=0, EN=0 .
	GPIO_PORTB_DATA_R = cmnd;
	GPIO_PORTA_DATA_R = 0x80; // E3  --> Enable pulse
	GPIO_PORTA_DATA_R = 0;

	if (cmnd < 4)
		delayMs(2);
	else
		delayUs(40);
}
//##############################################################################################################################################
void LCD_Data_ch(unsigned char char_data) // LCD data for writting function
{

	GPIO_PORTA_DATA_R = 0x20; //  A5--> RS=1 command register.
	GPIO_PORTB_DATA_R = char_data;
	GPIO_PORTA_DATA_R = (0x80 | 0x20); // Pulse E
	delayUs(0);
	GPIO_PORTA_DATA_R = 0;
	delayUs(40);
}
//############################################################################################################################################

void LCD_Initalization(void) // LCD Initialize function
{
	SYSCTL_RCGCGPIO_R |= 0x03; // to activate port A & B
	while ((SYSCTL_PRGPIO_R & 0x03) == 0)
	{
	}
	GPIO_PORTA_DIR_R |= 0xE0;	 // To Make LCD command port A direction as output
	GPIO_PORTA_DEN_R |= 0xE0;	 // To Make LCD command port A digital
	GPIO_PORTA_AFSEL_R &= ~0XFF; //Disable Analog mode on PORTA
	GPIO_PORTA_AMSEL_R &= ~0XFF; //Disable Analog mode on PORTA

	GPIO_PORTB_DIR_R |= 0xFF;	 // To Make LCD Data port B direction as output
	GPIO_PORTB_DEN_R |= 0xFF;	 // To Make LCD DATA port B digital
	GPIO_PORTB_AFSEL_R &= ~0XFF; //Disable Analog mode on PORTB
	GPIO_PORTB_AMSEL_R &= ~0XFF; //Disable Analog mode on PORTB

	delayMs(20); // LCD Power ON delay always more than 15Ms

	LCD_Command(0x30);
	LCD_Command(0x38); // Initialization of 16X2 LCD in  8_bit mode
	LCD_Command(0x0C); // Display ON Cursor OFF
	LCD_Command(0x06); // Auto Increment cursor
	LCD_Command(0x01); // Clear display
	LCD_Command(0x80); // Cursor at home position
}
// ##################################################################################################################################

void LCD_String(char *str) // Send string to LCD function
{
	int i;
	for (i = 0; str[i] != 0; i++) // Send each char of string till the NULL
	{
		LCD_Data_ch(str[i]);
	}
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
double deg_to_rad(double deg)
{
	return deg * (pi / 180);
}
double rad_to_deg(double rad)
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
// ##################################################################################################################################

float get_langitude(float lang){
// if lang dddmm.tttt
// from this formula ===>  ddd + mm.tttt to get actuall langitude
float x = (lang/100);
int y = (int)x;
return    y +(((x-y)*100)/60);
}

// ##################################################################################################################################

float get_latitude(float lat){
// if lang ddmm.tttt
// from this formula ===>  dd + mm.tttt to get actuall latitude
float x = (lat/100);
int y = (int)x;
return    y +(((x-y)*100)/60);
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
LCD_Data_ch(' m');      // print unit
}
// ##################################################################################################################################

void uart_Init(void){
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

 	SYSCTL_RCGCUART_R |= 0x0020; 					// enable uart5 , E4 --> rx ,, E5 -->tx
	while((SYSCTL_PRUART_R & 0x0020)==0){};
	UART5_CTL_R &= ~0x0001;
	UART5_IBRD_R = 104;
	UART5_FBRD_R = 11; 
	UART5_LCRH_R = 0x0070;
	UART5_CTL_R = 0x0301;
	
	
	
	}

unsigned char  uart_reciever(void)
	{
	while((UART5_FR_R & 0x10)!=0);
	return ((unsigned char)(UART5_DR_R & 0xFF));
}


//#######################################################################################################################	
//intilization for variables
char strng[50];
int strng_counter=0;
char Gps_Message[500];
int i ;
int finish =0; 
int position_counter=0;
char *data[25];


int data_length ;

void Receive_GPS_Data()   
{ 
	for(int i=0; i<500; i++)    //store the message in an array
	
        Gps_Message[i] = uart_reciever();
	
    while(finish==0)
{
for(int i=0;i<500;i++)
{
//try to find GPGGA message that i need  to get latitude and longitude 

if( Gps_Message[i]=='$' ){ 

if( Gps_Message[i+1]=='G' ) {

if( Gps_Message[i+2]=='P' ) {

 if( Gps_Message[i+3]=='G' ){

if( Gps_Message[i+4]=='G' ) {

if( Gps_Message[i+5]=='A' ) {

if( Gps_Message[i+6]==',') 
{
    
    for(int j=i;j<300;j++)
    {
        
        strng[strng_counter++] = Gps_Message[j];
   
        
    }
}
}
}
}
}
}
}
}

 finish =1;

    
}
}

void split_GPGGA()
{
    data_length=0;
    data[0] = strtok (strng ,",");
    while (data[data_length]!= NULL && data_length <14){
        data_length++ ;
        data[data_length] = strtok(NULL,",");
    }
}


// ##################################################################################################################################
float old_lat=0 , old_lang=0 , now_lat=0 , now_lang=0;
float dist=0, Total_dist=0;
float LAT;
 float LONG;
int main()
{
	
	SCB->CPACR|=((3UL<<10*2)|(3UL<<11*2));

		port_Init();

		uart_Init();
		Receive_GPS_Data() ;
		split_GPGGA();
		LAT = atof(data[2]);
		LONG = atof(data[4]);
		


	
	
  LCD_Initalization(); // Initialize LCD
	LCD_Clear();
	LCD_String("Distance = ");
	delayMs(500);
		
	
	while(Total_dist<=100){   
		// if you want to make another condition 
	// make swith to close the loop if you don't need 100m

	old_lat  = now_lat;
	old_lang = now_lang;

	now_lat  = get_latitude(LAT);  
	now_lang = get_langitude(LONG);


	if(!(old_lat==0 && old_lang==0)){  
	dist = distance(old_lat,old_lang,now_lat,now_lang);
	Total_dist+=dist;
	};

while(1)
    {
	LCD_Command(0xC0);				  // Go to 2nd line
	num_To_Ch(Total_dist);   // print distance in 2nd line of LCD		
	delayMs(500);
    }
        };
			LED_Init(RED_LED);  		//LED_Init(RED_LED)
	LCD_Command(0xC0);				  // Go to 2nd line
	num_To_Ch(Total_dist);   // print distance in 2nd line of LCD		
	delayMs(500);
    
	return 0;
}
