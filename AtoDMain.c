// UARTTestMain.c
// Runs on LM4F120/TM4C123
// Used to test the UART.c driver
// Daniel Valvano
// May 30, 2014

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1
#include <stdint.h>
#include <string.h>
#include "PLL.h"
#include "UART.h"
#include "tm4c123gh6pm.h"

//---------------------OutCRLF---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none
void OutCRLF(void);
//debug code
float distance;
uint32_t ADC0Result;
int RED=0x02;					//Red LED
int BLUE=0x04;				//Blue LED
int PURPLE=0x06;      // Red and Blue 
int GREEN=0x08;				//Green LED
int YELLOW=0x0A;      // Green and Red
int CYAN=0x0C;				//Blue and Green
int WHITE = 0x0E;     //All Colors
int OFF=0;

void SysTick_Handler(void){
		ADC0Result=ADC0_In();
}

void main(void){
	

	

	int i=0;
	int j=0;
  char ch;
  char string[20];  // global to assist in debugging
  uint32_t n;


	PortFInit();
  PLL_Init();       // 50  MHz
	
	//Timer_Init();	//this is for non-interrupted version of code
	//delay(10000); //this is for non-interrupted version of code
	ADC_Init();
	
  UART_Init();              // initialize UART
	UART1_Init();
	for(i=0;i<100000;i++)
	{}
	//delay(10000); //this is for non-interrupted version of code
  OutCRLF();
	SysTick_Init(40000);
	for(j=0;j<100000;j++)
	{}
  while(1){
		
		
		distance=4800/(((ADC0Result*3.3)/4095)*200-20);
		
		if(distance>60)
		{
			distance=60;
		}
		UART_OutUDec(distance);
		UART_OutString(" cm");
		for(int i=0;i<1000000;i++);
		OutCRLF();

		
		if(ADC0Result<=511)
		{
			PortFOutput(RED);
		}
		else if(ADC0Result>511 && ADC0Result<=1022)
		{
			PortFOutput(GREEN);
		}		
		else if(ADC0Result>1022 && ADC0Result<=1533)
		{
			PortFOutput(BLUE);
		}		
		else if(ADC0Result>1533 && ADC0Result<=2044)
		{
			PortFOutput(PURPLE);
		}		
		else if(ADC0Result>2044 && ADC0Result<=2555)
		{
			PortFOutput(GREEN);
		}		
		else if(ADC0Result>2555 && ADC0Result<=3066)
		{
			PortFOutput(YELLOW);
		}
		else if(ADC0Result>3066 && ADC0Result<=3577)
		{
			PortFOutput(CYAN);
		}
		else if(ADC0Result>3577 && ADC0Result<=4095)
		{
			PortFOutput(WHITE);
		}
		/*
		
		if(strcmp(string,"red")==0||strcmp(string,"\nred")==0)
		{
			PortFOutput(RED);
		}
		
		if(strcmp(string,"green")==0||strcmp(string,"\ngreen")==0)
		{
			PortFOutput(GREEN);
		}
		
		if(strcmp(string,"blue")==0||strcmp(string,"\nblue")==0)
		{
			PortFOutput(BLUE);
		}
		
		if(strcmp(string,"purple")==0||strcmp(string,"\npurple")==0)
		{
			PortFOutput(PURPLE);
		}
		
		if(strcmp(string,"green")==0||strcmp(string,"\ngreen")==0)
		{
			PortFOutput(GREEN);
		}
		
		if(strcmp(string,"yellow")==0||strcmp(string,"\nyellow")==0)
		{
			PortFOutput(YELLOW);
		}
		if(strcmp(string,"cyan")==0||strcmp(string,"\ncyan")==0)
		{
			PortFOutput(CYAN);
		}
		if(strcmp(string,"white")==0||strcmp(string,"\nwhite")==0)
		{
			PortFOutput(WHITE);
		}
		if(strcmp(string,"off")==0||strcmp(string,"\noff")==0)
		{
			PortFOutput(0);
		}
			
0 001 0   2 RED
0 010 0   4 BLUE
0 011 0   6 PURPLE
0 100 0   8 GREEN
0 101 0   A YELLOW
0 110 0   C CYAN
0 111 0   E WHITE
0 000 0   0 OFF
	
		strcpy(string,"");
    UART_OutString("Waiting on Bluetooth... ");OutCRLF();
    UART1_InString(string,25);
    UART_OutString(" String from Bluetooth: "); UART_OutString(string); 

OutCRLF();
		*/
		
		
		
		
  }
}
void OutCRLF(void){
  UART_OutChar(CR);
  UART_OutChar(LF);
}
