#include "tm4c123gh6pm.h"
#include <stdint.h>

void ADC_Init(void);
uint32_t ADC0_In(void);
void ADC_Init(void){
	int i=0;
	//Use PE4
	SYSCTL_RCGCGPIO_R |= 0x10;
	while((SYSCTL_PRGPIO_R&0x10)==0){};
	GPIO_PORTE_DIR_R &= ~0x10;
	GPIO_PORTE_AFSEL_R|=0x10;
	GPIO_PORTE_DEN_R &= ~0x10;
	GPIO_PORTE_AMSEL_R|=0x10;
	SYSCTL_RCGCADC_R |= 0x01;
	for(i=0;i<50000;i++)			//This delay was required otherwise program went to fault handler.  Found through trial and error.
		{
		}
		//delay(50000);
	ADC0_PC_R=0x01;
	ADC0_SSPRI_R=0x0123;
	ADC0_ACTSS_R&=~0x0008;
	ADC0_EMUX_R &= ~0xF000;
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+9;//or this one
	ADC0_SSCTL3_R = 0x0006;
	ADC0_IM_R &= ~0x0008;
	ADC0_ACTSS_R |= 0x0008;
	

	
	
}

uint32_t ADC0_In(void){
	
	uint32_t result;
	
	ADC0_PSSI_R=0x0008;
	while((ADC0_RIS_R&0x08)==0){};
	result=ADC0_SSFIFO3_R&0xFFF;
	ADC0_ISC_R = 0x0008;
	return result;
	
}