/*Richard Sherwood
	Embedded 2 Final Project
	04/25/2018

	A Bluetooth controlled system that will turn a Sharp IR range sensor 
	to a specific point and then tell the user the distance to the nearest object. 
	
	The Bluetooth terminal app will ask the user for a radial angle from 0 to 180 
	degrees then the servo will point to that angle.  
	
	Once the servo reaches the angle, the microprocessor will get a reading from the 
	Sharp IR ranger sensor to detect the nearest object and record the distance.  
	
*/

#include <stdint.h>
#include <string.h>
#include "tm4c123gh6pm.h"
#include "SysTickInts.h"
#include "PLL.h"


/*Function Prototypes*/
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
volatile uint32_t Counts = 0; //Counts used for systick timer to create PWM signal
int PortFInput();							//Initialize PortF for TIVA C Board buttons and LEDs PF4-0 [in out out out in]
void PortBInit();							//Initialize Port B to use PB0 for PWM output
void PortFOutput(int);				//Function used to turn on and off LEDs if necessary
/*------------------*/

volatile int angle=280;				//Initial RC servo angle (actual radial angle is value -100 and divided by 2 so 90degrees)

uint32_t ADC0Result;					//Used to store result from A to D - Sharp Ranger Signal


/*  Main function initializes all ports, UART and A to D.  

		It sends test output through Uart 1 connected to bluetooth asking for angle of servo.
		
		After input is received, it rotates servo motor to the angle input and takes an A to D reading.
*/
int main(void){
	
	char ch;
  char string[20];
	int distance=65;
	
/*------------------------------------------------------Initialization------------------------------------------------------*/	
	PLL_Init();                 // bus clock at 50 MHz
	PortBInit();	
	UART1_Init();  //This function is in Uart.c file.  
								 //Uart initialized to 9600 baud to match HC05 module
								 //standard start/stop/parity bits.
	
	for(int i=0;i<100000;i++)//This delay is required to give UART1 time to finish intialization, otherwise it is out of sync
		{
		}

	ADC_Init();		//A to D initialization to Port E4.  Only need one channel single sequence.
/*-------------------------------------------------------------------------------------------------------------------------*/

		
  while(1){

/*-----------------------------Bluetooth Input and Results--------------------------------------------------------*/
		
//UART1 string function was locking up and I did not have time to troubleshoot it 
//		so I sent my strings one character at a time.
UART1_OutChar('A');UART1_OutChar('N');UART1_OutChar('G');UART1_OutChar('L');UART1_OutChar('E');UART1_OutChar('>');
	
	angle=UART1_InUDec();//Receive an ascii response from bluetooth and use InUDec function to convert to decimal
	angle=(angle*2)+100;//Convert range of values 0-180 to PWM values 100-460.
		
	//Set max and min.  In this case, if a larger value it entered it automatically converts to 
	if(angle>460)
	{
		angle=460;
	}
	
	if(angle<100)
	{
		angle=100;
	}
	
	SysTick_Init(278); //I'm breaking up the clock into increments to match resolution to 180 degrees.
										 //The RC servo requires an on time range of 1ms-2ms over a 20ms total period (5% to 10% duty cycle)
										 //50MHz=.5.56 uS   5.56uS * 180= 1ms   5.56 uS * 360 = 2ms
	
										//In theory I wanted to be able to enter the angle in directly using time values but when I tested it
										//the angle that the servo went to was exactly half
	
									  //after testing, I found the values to be 100-460 equating to 0-180 degrees
							
	EnableInterrupts();//Turn on interrupts to move servo

	for(int j=0;j<10000000;j++)//This delay is 10 counts of 20 milliseconds
														//to allow enough time for servo to move to it's intended angle
		{
		}
	
	DisableInterrupts();	//Turn off interrupts after servo reaches angle
	OutCRLF();
		

	ADC0Result=ADC0_In();		//Take a reading after moving to angle
	distance=4800/(((ADC0Result*3.3)/4095)*200-20);//This function is derived from the Sharp IR Range sensor distance vs Vout formula
																								
																								// 4800/Vout * 200 - 20
		
																								//Vout=(ADCval * Vmax)/((2^12)-1) = (ADCval*3.3V)/4095
		
		
		if(distance>60)//According to the data sheet the linearization formula has a higher error margin after 60 cm
		{
			distance=60;
		}
		
		
		UART1_OutUDec(distance);//output distance in cm
		UART1_OutChar(' ');UART1_OutChar('c');UART1_OutChar('m');
		for(int i=0;i<1000000;i++);//delay for 20 ms
		OutCRLF();//New Line then run process again
/*---------------------------------------------------------------------------------------------------*/	
	}   //End while(1)               
}//End Main Function


/*------------------------------------------------PWM Interrupt Service Routine--------------------------------------------*/
void SysTick_Handler(void){//Systick interrupt handler is used to create PWM 5-10% duty cycle and 20ms period for RC servo
	Counts=Counts+1;

	if(Counts==1){//Turn on at count 1
		//GPIO_PORTF_DATA_R=0x02;
		 GPIO_PORTB_DATA_R=0x01;
	}
	if(Counts==angle){//control values range is 100-450
 //GPIO_PORTF_DATA_R^=0x02;              // toggle PF1
	  GPIO_PORTB_DATA_R=0x00;
		
	}
	if(Counts==3597){//period value = 3597*(1/50MHz) = 20 ms
		Counts=0;//When period ends start a new cycle
	}
}
/*------------------------------------------------------------------------------------------------------------------------*/


void PortBInit(){
	
	SYSCTL_RCGCGPIO_R|=0x02;
	while((SYSCTL_PRGPIO_R&0x02)==0){};
	GPIO_PORTB_CR_R=0xFF;
	GPIO_PORTB_AMSEL_R=0;
	GPIO_PORTB_PCTL_R=0;
	GPIO_PORTB_DIR_R=0x0F;
	GPIO_PORTB_AFSEL_R=0;	
	GPIO_PORTB_DEN_R =0xFF;
}

void PortFInit(){
	
	SYSCTL_RCGCGPIO_R|=0x20;
	while((SYSCTL_PRGPIO_R&0x20)==0){};
	
	
	GPIO_PORTF_LOCK_R=0x4C4F434B;
	GPIO_PORTF_CR_R=0xFF;  
	
  GPIO_PORTF_DIR_R |= 0x02;   // make PF1 output (red LED)
  GPIO_PORTF_AFSEL_R &= ~0x02;// disable alt funct on PF1
  GPIO_PORTF_DEN_R |= 0xFF;   // enable digital I/O on PF1										  
	GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFFFFD);	// configure PF1 as GPIO 
  GPIO_PORTF_AMSEL_R = 0;     // disable analog functionality on PF 
	
}

int PortFInput(){
	int M;
	
	M=GPIO_PORTF_DATA_R;
	M&=0x11;
	return M;
	
}

void PortFOutput(int M){
	
    GPIO_PORTF_DATA_R=M; 
		
}


