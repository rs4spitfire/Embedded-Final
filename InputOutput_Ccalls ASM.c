// Richard Sherwood
// 12/03/2017
// This code will play jingle bells if SW1 is pressed and Rudolph the Red Nosed Reindeer if SW2 is pressed

#include "tm4c123gh6pm.h"
#include<stdio.h>
#include<stdint.h>

extern void PortF_Init();					//ASM function initializes Port F for F0 and F4 inputs - F1-F3 outputs
extern int PortF_Input();					//ASM function returns an int for whichever switch is pressed
extern void PLL_Init();						//ASM function initializes PLL to 16 MHz external crystal
void PORTBINIT();									//Initializes PortB 2 for output to speaker
void Timer_Init();								//Initializes Timer
int PlayNote(int,int,int);				//Plays a note with parameters for pitch,duration,and a short delay to make note distinct
extern void PortF_Output(int);		//ASM function used to turn on colored LEDs
void delay(int);									//delay using timer - int is loaded as reload value - this function polls timer flag

void debounce(int);
int checkInputs(void);
void playJingleBells(int);
void playRudolph(int);
		

extern unsigned long int M;

	int ONESEC=5333333; 
	int QUARTERSEC=1333333; 
  int FIFTHSEC= 1066666;


void main(void) {
	

	int SW1=0x10;   			//Value for SW1 - PORTF4             
  int SW2=0x01; 				//Value for SW2 - PORTF0
  int BOTH=0x00;				//Value for neither switch pressed
	int NONE=0x11;				//Value for both switches pressed
	
	
	int CHANGE_TEMPO=-120;//This value will change the tempo without affecting pitch
												//only use range from -500 to 500
												//negative numbers will speed the tempo up while positive numbers slow it down
	
												//The original tempo was based on 120 beats per measure but it seemed slow
												//so -120 is me speeding it up a little bit


	
	//-----PITCHES AND BEATS of NOTES-----
	//All pitches are based on piano note frequencies and a 16MHz clock speed
	//For example the C6 pitch is 1046.5 hz.  1/1046.5=.000956 seconds   .000956/(1/16Mhz)=15290 timer counts
	//To make the C6 pitch, turn portB2 on for 7645 cycles and off for 7645 cycles
	
	//The note durations are based on a quarter note taking 0.5 seconds (60seconds/120 beats per minute)
	//All other note durations are modified from quarter note time
	//-------------------------------------

	
	int SHORT_DELAY=100000;//This short delay is around 0.01 seconds and is just to make the notes distinct
	
	int C_6_PITCH=7645;	
	int C_6_QUARTER=523+CHANGE_TEMPO;
	int C_6_EIGTH=C_6_QUARTER/2;
	int C_6_DQUARTER=C_6_EIGTH*3;
	int C_6_HALF=C_6_QUARTER*2;
	int C_6_WHOLE=C_6_QUARTER*4;
	
	int D_6_PITCH=6810;
	int D_6_QUARTER=587+CHANGE_TEMPO;;
	int D_6_EIGTH=D_6_QUARTER/2;
	int D_6_DQUARTER=D_6_EIGTH*3;
	int D_6_HALF=D_6_QUARTER*2;
	int D_6_WHOLE=D_6_QUARTER*4;
	
	int E_6_PITCH=6067;
	int E_6_QUARTER=660+CHANGE_TEMPO;
	int E_6_EIGTH=E_6_QUARTER/2;
	int E_6_DQUARTER=E_6_EIGTH*3;
	int E_6_HALF=E_6_QUARTER*2;
	int E_6_WHOLE=E_6_QUARTER*4;
	
	int F_6_PITCH=5727;
	int F_6_QUARTER=698+CHANGE_TEMPO;
	int F_6_EIGTH=F_6_QUARTER/2;
	int F_6_DQUARTER=F_6_EIGTH*3;
	int F_6_HALF=F_6_QUARTER*2;
	int F_6_WHOLE=F_6_QUARTER*4;
	
	
	int G_6_PITCH=5102;
	int G_6_QUARTER=784+CHANGE_TEMPO;
	int G_6_EIGTH=G_6_QUARTER/2;
	int G_6_DQUARTER=G_6_EIGTH*3;
	int G_6_HALF=G_6_QUARTER*2;
	int G_6_WHOLE=G_6_QUARTER*4;
	
	int A_6_PITCH=4545;
	int A_6_QUARTER=880+CHANGE_TEMPO;
	int A_6_EIGTH=G_6_QUARTER/2;
	int A_6_DQUARTER=G_6_EIGTH*3;
	int A_6_HALF=G_6_QUARTER*2;
	int A_6_WHOLE=G_6_QUARTER*4;
	
	int B_6_PITCH=4050;
	int B_6_QUARTER=988+CHANGE_TEMPO;
	int B_6_EIGTH=G_6_QUARTER/2;
	int B_6_DQUARTER=G_6_EIGTH*3;
	int B_6_HALF=G_6_QUARTER*2;
	int B_6_WHOLE=G_6_QUARTER*4;
	
	int C_7_PITCH=3822;
	int C_7_QUARTER=1047+CHANGE_TEMPO;
	int C_7_EIGTH=C_7_QUARTER/2;
	int C_7_DQUARTER=C_7_EIGTH*3;
	int C_7_HALF=C_7_QUARTER*2;
	int C_7_WHOLE=C_7_QUARTER*4;
	
	int D_7_PITCH=3405;
	int D_7_QUARTER=1175+CHANGE_TEMPO;
	int D_7_EIGTH=D_7_QUARTER/2;
	int D_7_DQUARTER=D_7_EIGTH*3;
	int D_7_HALF=D_7_QUARTER*2;
	int D_7_WHOLE=D_7_QUARTER*4;
//-----------------------------------------	
	

	int QUARTER_REST=C_6_QUARTER*C_6_PITCH; //Quarter rest is delay for the same time as a quarter note
	int MEASURE_REST=(QUARTER_REST*4);
	
	M=0x00;
	int COLOR=0x00;
	short change_song=0;
	
//-----Initialization-------
	PortF_Init();
	PORTBINIT();
	Timer_Init();
	PLL_Init();
//---------------------------

	while(1){
	

		
		delay(QUARTERSEC);//Delay a quarter second before starting program to make sure everything is reset
			
		M=PortF_Input();//Get switch inputs
		debounce(M);
		
		//Check inputs
		
		if(M==SW1)//Play Jingle Bells with blinking red and green leds on SW1
		{		
			COLOR=0;
			change_song=0;
			while(change_song==0){
				
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02; PortF_Output(COLOR);
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);	if(change_song==1) break;	
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);	if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
						
				change_song=PlayNote(E_6_PITCH,E_6_HALF,SHORT_DELAY);		if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);	if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);	if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				change_song=PlayNote(E_6_PITCH,E_6_HALF,SHORT_DELAY);		if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);   if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(G_6_PITCH,G_6_QUARTER,SHORT_DELAY);	if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(C_6_PITCH,C_6_DQUARTER,SHORT_DELAY);	if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(D_6_PITCH,D_6_EIGTH,SHORT_DELAY);	if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(E_6_PITCH,E_6_HALF,SHORT_DELAY);	if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);

				delay(QUARTER_REST);

					
				change_song=PlayNote(F_6_PITCH,F_6_QUARTER,SHORT_DELAY); if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(F_6_PITCH,F_6_QUARTER,SHORT_DELAY); if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(F_6_PITCH,F_6_DQUARTER,SHORT_DELAY); if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(F_6_PITCH,F_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(F_6_PITCH,F_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(E_6_PITCH,E_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(E_6_PITCH,E_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(D_6_PITCH,D_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(D_6_PITCH,D_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(D_6_PITCH,D_6_HALF,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(G_6_PITCH,G_6_HALF,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
							
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);	if(change_song==1) break;	
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);				
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
					
					
				change_song=PlayNote(E_6_PITCH,E_6_HALF,SHORT_DELAY);		if(change_song==1) break;	
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);	if(change_song==1) break;	
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);				
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(E_6_PITCH,E_6_HALF,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(G_6_PITCH,G_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(C_6_PITCH,C_6_DQUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(D_6_PITCH,D_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(E_6_PITCH,E_6_HALF,SHORT_DELAY);		if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				delay(QUARTER_REST);

					
				change_song=PlayNote(F_6_PITCH,F_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(F_6_PITCH,F_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(F_6_PITCH,F_6_DQUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(F_6_PITCH,F_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(F_6_PITCH,F_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(E_6_PITCH,E_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(E_6_PITCH,E_6_EIGTH,SHORT_DELAY);		if(change_song==1) break;	
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(G_6_PITCH,G_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(G_6_PITCH,G_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(F_6_PITCH,F_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(D_6_PITCH,D_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(C_6_PITCH,C_6_WHOLE,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=1;
			
			}
				PortF_Output(0);
		}
		else if(M==SW2)//Play RUDOLPH with blinking red and green leds on switch 2
		{
			COLOR=0;
			change_song=0;
			while(change_song==0){
				

				
				change_song=PlayNote(G_6_PITCH,G_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(A_6_PITCH,A_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
			
				
				change_song=PlayNote(G_6_PITCH,G_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);if(change_song==1) break;	
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);				
				change_song=PlayNote(C_7_PITCH,C_7_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
			
				
				change_song=PlayNote(A_6_PITCH,A_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(G_6_PITCH,G_6_HALF,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
			
				
				delay(QUARTER_REST);
			
				
				change_song=PlayNote(G_6_PITCH,G_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(A_6_PITCH,A_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				change_song=PlayNote(G_6_PITCH,G_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(A_6_PITCH,A_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
			
				
				change_song=PlayNote(G_6_PITCH,G_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(C_7_PITCH,C_7_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(B_6_PITCH,B_6_WHOLE,SHORT_DELAY);	if(change_song==1) break;		
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
	//--------------------------------------------------------	
					
				change_song=PlayNote(F_6_PITCH,F_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(G_6_PITCH,G_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(F_6_PITCH,F_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(D_6_PITCH,D_6_QUARTER,SHORT_DELAY);if(change_song==1) break;	
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);				
				change_song=PlayNote(B_6_PITCH,B_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(A_6_PITCH,A_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(G_6_PITCH,G_6_HALF,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				delay(QUARTER_REST);
				
				
				change_song=PlayNote(G_6_PITCH,G_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(A_6_PITCH,A_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				change_song=PlayNote(G_6_PITCH,G_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(A_6_PITCH,A_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(G_6_PITCH,G_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(A_6_PITCH,A_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(E_6_PITCH,E_6_WHOLE,SHORT_DELAY);	if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
								
				change_song=PlayNote(G_6_PITCH,G_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(A_6_PITCH,A_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
			
				
				change_song=PlayNote(G_6_PITCH,G_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(E_6_PITCH,E_6_QUARTER,SHORT_DELAY);if(change_song==1) break;	
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);				
				change_song=PlayNote(C_7_PITCH,C_7_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
			
				
				change_song=PlayNote(A_6_PITCH,A_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(G_6_PITCH,G_6_HALF,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
			
				
				delay(QUARTER_REST);
			
				
				change_song=PlayNote(G_6_PITCH,G_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(A_6_PITCH,A_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				change_song=PlayNote(G_6_PITCH,G_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(A_6_PITCH,A_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
			
				
				change_song=PlayNote(G_6_PITCH,G_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(C_7_PITCH,C_7_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(B_6_PITCH,B_6_WHOLE,SHORT_DELAY);	if(change_song==1) break;	
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);	
	//--------------------------------------------------------	
				
				change_song=PlayNote(F_6_PITCH,F_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(G_6_PITCH,G_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(F_6_PITCH,F_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(D_6_PITCH,D_6_QUARTER,SHORT_DELAY);	if(change_song==1) break;	
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);				
				change_song=PlayNote(B_6_PITCH,B_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(A_6_PITCH,A_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(G_6_PITCH,G_6_HALF,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				delay(QUARTER_REST);
				
				
				change_song=PlayNote(G_6_PITCH,G_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(A_6_PITCH,A_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				change_song=PlayNote(G_6_PITCH,G_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(A_6_PITCH,A_6_EIGTH,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(G_6_PITCH,G_6_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				change_song=PlayNote(D_7_PITCH,D_7_QUARTER,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				
				change_song=PlayNote(C_7_PITCH,C_7_WHOLE,SHORT_DELAY);if(change_song==1) break;
				COLOR=COLOR+0x02; if(COLOR>0x0E) COLOR=0x02;PortF_Output(COLOR);
				
				change_song=1;
			}
			PortF_Output(0);
		}		
		
		
	}
}


void Timer_Init(void){
	
	NVIC_ST_CTRL_R=0;
	NVIC_ST_RELOAD_R=0x00FFFFFF;
	NVIC_ST_CURRENT_R=0;
	NVIC_ST_CTRL_R=0x00000005;
	
}

void delay(int delaytime){
	
	int SW1=0x10;   			//Value for SW1 - PORTF4             
  int SW2=0x01; 				//Value for SW2 - PORTF0
	

	
	NVIC_ST_RELOAD_R=delaytime-1;
	NVIC_ST_CURRENT_R=0;
	while((NVIC_ST_CTRL_R&0x00010000)==0){}//Poll Timer Set flag
	
	
	
}

void PORTBINIT(){
	
	SYSCTL_RCGCGPIO_R|=0x02; //0000 0000 0000 0010   Initialize clock for Port B2
	while((SYSCTL_PRGPIO_R&0x02)==0){};
	
	GPIO_PORTB_AMSEL_R=0;
	GPIO_PORTB_PCTL_R=0;
	GPIO_PORTB_DIR_R=0xFF; //All of Port B will be output
	GPIO_PORTB_AFSEL_R=0;
	GPIO_PORTB_DEN_R =0xFF; 
}

int PlayNote(int pitch,int duration,int delay_time){ //pitch is one half of the cycle for the note being played

	
	for(int i=1; i<duration;i++)
	{
		GPIO_PORTB_DATA_R=0x08;//0000 0000 0000 0100
		delay(pitch);
		GPIO_PORTB_DATA_R=0x00;
		delay(pitch);
	}
	
	int SW1=0x10;   			//Value for SW1 - PORTF4             
  int SW2=0x01; 				//Value for SW2 - PORTF0
	int CHANGE_TEMPO=-120;
	M=PortF_Input();
	//debounce(M);
		
	if(M==SW1)//Play Jingle Bells with blinking red and green leds on SW1
	{		
		
		return 1;
		
	}
	else if(M==SW2)//Play RUDOLPH with blinking red and green leds on switch 2
	{
		return 1;
	}
	
	delay(delay_time);
	return 0;
}



void playJingleBells(int CHANGE_TEMPO){
}

void playRudolph(int CHANGE_TEMPO){

	
	int RED=0x02;					//Red LED
  int BLUE=0x04;				//Blue LED
  int GREEN=0x08;				//Green LED
	int SHORT_DELAY=100000;//This short delay is around 0.01 seconds and is just to make the notes distinct
	
	int C_6_PITCH=7645;	
	int C_6_QUARTER=523+CHANGE_TEMPO;
	int C_6_EIGTH=C_6_QUARTER/2;
	int C_6_DQUARTER=C_6_EIGTH*3;
	int C_6_HALF=C_6_QUARTER*2;
	int C_6_WHOLE=C_6_QUARTER*4;
	
	int D_6_PITCH=6810;
	int D_6_QUARTER=587+CHANGE_TEMPO;;
	int D_6_EIGTH=D_6_QUARTER/2;
	int D_6_DQUARTER=D_6_EIGTH*3;
	int D_6_HALF=D_6_QUARTER*2;
	int D_6_WHOLE=D_6_QUARTER*4;
	
	int E_6_PITCH=6067;
	int E_6_QUARTER=660+CHANGE_TEMPO;
	int E_6_EIGTH=E_6_QUARTER/2;
	int E_6_DQUARTER=E_6_EIGTH*3;
	int E_6_HALF=E_6_QUARTER*2;
	int E_6_WHOLE=E_6_QUARTER*4;
	
	int F_6_PITCH=5727;
	int F_6_QUARTER=698+CHANGE_TEMPO;
	int F_6_EIGTH=F_6_QUARTER/2;
	int F_6_DQUARTER=F_6_EIGTH*3;
	int F_6_HALF=F_6_QUARTER*2;
	int F_6_WHOLE=F_6_QUARTER*4;
	
	
	int G_6_PITCH=5102;
	int G_6_QUARTER=784+CHANGE_TEMPO;
	int G_6_EIGTH=G_6_QUARTER/2;
	int G_6_DQUARTER=G_6_EIGTH*3;
	int G_6_HALF=G_6_QUARTER*2;
	int G_6_WHOLE=G_6_QUARTER*4;
	
	int A_6_PITCH=4545;
	int A_6_QUARTER=880+CHANGE_TEMPO;
	int A_6_EIGTH=G_6_QUARTER/2;
	int A_6_DQUARTER=G_6_EIGTH*3;
	int A_6_HALF=G_6_QUARTER*2;
	int A_6_WHOLE=G_6_QUARTER*4;
	
	int B_6_PITCH=4050;
	int B_6_QUARTER=988+CHANGE_TEMPO;
	int B_6_EIGTH=G_6_QUARTER/2;
	int B_6_DQUARTER=G_6_EIGTH*3;
	int B_6_HALF=G_6_QUARTER*2;
	int B_6_WHOLE=G_6_QUARTER*4;
	
	int C_7_PITCH=3822;
	int C_7_QUARTER=1047+CHANGE_TEMPO;
	int C_7_EIGTH=C_7_QUARTER/2;
	int C_7_DQUARTER=C_7_EIGTH*3;
	int C_7_HALF=C_7_QUARTER*2;
	int C_7_WHOLE=C_7_QUARTER*4;
	
	int D_7_PITCH=3405;
	int D_7_QUARTER=1175+CHANGE_TEMPO;
	int D_7_EIGTH=D_7_QUARTER/2;
	int D_7_DQUARTER=D_7_EIGTH*3;
	int D_7_HALF=D_7_QUARTER*2;
	int D_7_WHOLE=D_7_QUARTER*4;
//-----------------------------------------	
	

	int QUARTER_REST=C_6_QUARTER*C_6_PITCH; //Quarter rest is delay for the same time as a quarter note
	int MEASURE_REST=(QUARTER_REST*4);
	
	

				PortF_Output(0);
}

void debounce(int button_pressed){

	while(button_pressed==0x01 || button_pressed==0x10 || button_pressed==0){
	button_pressed=PortF_Input();
	}
	
}