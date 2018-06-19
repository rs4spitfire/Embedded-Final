#include "tm4c123gh6pm.h"
#include<stdio.h>
#include<stdint.h>

void PortFInit(void);
int PortFInput(void);
void PortFOutput(int);

//  to be called by asm
//unsigned long int M;
void PortFInit(){
	
	SYSCTL_RCGCGPIO_R|=0x20;
	while((SYSCTL_PRGPIO_R&0x20)==0){};
	
	GPIO_PORTF_LOCK_R=0x4C4F434B;
	GPIO_PORTF_CR_R=0xFF;
	GPIO_PORTF_AMSEL_R=0;
	GPIO_PORTF_PCTL_R=0;
	GPIO_PORTF_DIR_R=0x0E; //0 - 111 - 0
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

