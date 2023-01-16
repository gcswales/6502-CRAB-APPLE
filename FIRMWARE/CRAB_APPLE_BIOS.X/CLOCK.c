//*************************************************************
//***	FILE:		CLC1.C									***
//***	DATE:		07/09/21								***
//***	AUTHOR:		GEOFFREY SWALES							***
//***	VERSION:	V0.1									***
//*************************************************************
//***	DESCRIPTION:										***
//***	WE USE THE DIGITAL SIGNAL MODULATOR TO SWITCH		***
//***	BETWEEN THE REFERENCE CLOCK AND A DC LEVEL WITHOUT	***
//***	HAVING ISSUES WITH GLITCHES.						***
//***	THIS IS CALLED CPU CLOCK-STRETCHING AND BUYS US		***
//***	TIME TO CORRECLY PROCESS THE CPU BUS.				***
//*************************************************************
//***	VERSION TRACK:										***
//***	V0.1: INITIAL DEVELOPMENT VERSION					***
//***	V1.0:												***
//*************************************************************
//	
//*************************************************************
//*** INCLUDE REQUIRED SPECIFIC DEFINTIONS ********************
//*************************************************************
//
#include "CLOCK.h"											// include definitions
//
//*************************************************************
//*** STRUCTURE DEFINITIONS ***********************************
//*************************************************************
//

//
//*************************************************************
//*** GLOBAL VARIABLES ****************************************
//*************************************************************
//

//
//*************************************************************
//*** INITIALISE THE CPU CLOCK ********************************
//*************************************************************
//
void clock_init( void )
{
//
	PMD2bits.DSM1MD = 0;									// enable DSM peripheral
	MD1CON0bits.EN = 0;										// disable MSD
	MD1CON1bits.CHSYNC = 0;									// no high line synchronisation
	MD1CON1bits.CLSYNC = 1;									// low line synchronisation	
	MD1CARHbits.MD1CH = 0b1011;								// MSD high carrier input is CLC2_OUT (SOFTWARE CONTROLLED!)
	MD1CARLbits.MD1CL = 0b0100;								// MSD low carrier input is CLKREF output
	MD1SRCbits.MS = 0b01100;								// MSD source input is CLC1_OUT
	MD1CON0bits.OPOL = 0;									// output not inverted
	MD1CON0bits.EN = 1;										// enable MSD
//
	PMD0bits.CLKRMD = 0;									// enable reference clock peripheral
	CLKRCONbits.EN = 0;										// disable reference clock
	CLKRCLK = 0b0000;										// Fosc as reference clock source
	CLKRCONbits.DIV = CLKREF_1MHZ;							// clock frequency of 1MHz
	CLKRCONbits.DC = 0b10;									// 50% duty cycle
}
//
//*************************************************************
//*** SET CPU CLOCK FREQUENCY *********************************
//*************************************************************
//
void clock_set( uint8_t f )
{
	if(f)													// if we have a frequency...
	{
		CLKRCONbits.EN=0;									// disable reference clock			
		switch(f)											// select frequency value
		{
			default:										// default is 1MHz
			case 1:											// 1MHz
				CLKRCONbits.DIV=CLKREF_1MHZ;				// clock frequency of 1MHz
			break;
			case 2:											// 2MHz
				CLKRCONbits.DIV=CLKREF_2MHZ;				// clock frequency of 2MHz
			break;
			case 4:											// 4MHz
				CLKRCONbits.DIV=CLKREF_4MHZ;				// clock frequency of 4MHz
			break;
		}
		CLKRCONbits.EN=1;									// enable reference clock	
	}
	else													// no frequency...
	{
		CLKRCONbits.EN=0;									// disable reference clock			
	}
}
//
//*************************************************************
//*** END *****************************************************
//*************************************************************


