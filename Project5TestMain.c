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


#include <stdint.h>
//#include <stdio.h>
#include "PLL.h"
#include "tm4c123gh6pm.h"


#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode

// **************SysTick_Init*********************
// Initialize Systick periodic interrupts
// Input: interrupt period
//        Units of period are 12.5ns (assuming 50 MHz clock)
//        Maximum is 2^24-1
//        Minimum is determined by lenght of ISR
// Output: none
void SysTick_Init(uint32_t period);

// Interrupt service routine
// Executed every 20ns*(period)
void SysTick_Handler(void);

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

void PortFInit(void);
void PortBInit(void);
int PortBInput(void);
void PortBOutput(int);
void delay(int);
void Timer_Init(void);
void debounce(int);
int wave_index=0;



void SysTick_Handler(void){
	
	


	

}
//debug code
void main(void){
	
	PortBInit();
	PortFInit();
	SysTick_Init(1250);
	int SW1=0x10;   			//Value for SW1 - PORTF4             
  int SW2=0x01; 				//Value for SW2 - PORTF0
	
	
	int n=0;
	int i=0;
	
	while(1)
	{
		
		
		/*
		PortBOutput(i);
		
		n=PortFInput();
		debounce(n);
		
		if(n==0x01)
		{
			
			
			if(i==16)
			{
				i=0;
			}
			i++;
		}*/
		
		/*
		for(int i=0;i<=15;i++)
			{
				PortBOutput(i);
			for(int j=0;j<1000;j++)
				{
				}
			}
		*/
		
	}
}

void debounce(int button_pressed){

	while(button_pressed==0x01 || button_pressed==0x10 || button_pressed==0){
		button_pressed=PortFInput();
	}
	
}



//  to be called by asm
//unsigned long int M;
void PortFInit(){
	
	SYSCTL_RCGCGPIO_R|=0x20;
	while((SYSCTL_PRGPIO_R&0x20)==0){};
	
	GPIO_PORTF_LOCK_R=0x4C4F434B;
	GPIO_PORTF_CR_R=0xFF;
	GPIO_PORTF_AMSEL_R=0;
	GPIO_PORTF_PCTL_R=0;
	GPIO_PORTF_DIR_R=0x0E;
	GPIO_PORTF_AFSEL_R=0;
	GPIO_PORTF_PUR_R=0x11;
	GPIO_PORTF_DEN_R =0xFF;
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

int PortBInput(){
	int M;
	
	M=GPIO_PORTB_DATA_R;
	return M;
	
}

void PortBOutput(int M){
	
    GPIO_PORTB_DATA_R=M; 
		
}

void Timer_Init(void){
	
	NVIC_ST_CTRL_R=0;
	NVIC_ST_RELOAD_R=0x00FFFFFF;
	NVIC_ST_CURRENT_R=0;
	NVIC_ST_CTRL_R=0x00000005;
	
}


void SysTick_Init(uint32_t period){
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_PRI3_R = (NVIC_SYS_PRI3_R&0x80000000)|0x00FFFFFF; // priority 4
  NVIC_ST_CTRL_R = 0x00000007;	// enable SysTick with core clock and interrupts
}
