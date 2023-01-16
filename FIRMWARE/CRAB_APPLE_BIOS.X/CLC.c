//*************************************************************
//***	FILE:		CLC1.C									***
//***	DATE:		07/09/21								***
//***	AUTHOR:		GEOFFREY SWALES							***
//***	VERSION:	V0.1									***
//*************************************************************
//***	DESCRIPTION:										***
//***	CONFIGURES PIC'S CUSTOM-LOGIC-CONTROL MODULES   	***
//***	THESE ARE USED TO DETECT CS, EN, AND CLOCK PHASE	***
//***	THEY PAUSE THE CLOCK OUTPUT, SO THE PIC HAS TIME TO	***
//***	GENERATE AN INTERRUPT TO RESPOND TO BUS REQUESTS	***
//*************************************************************
//***	VERSION TRACK:										***
//***	V0.1: INITIAL DEVELOPMENT VERSION					***
//***	V1.0:												***
//*************************************************************
//	
// IMPORTANT: THERE IS A MISTAKE IN THE MICROCHIP PIC INC FILE
// WITH THE IRQ NUMBERS, DOUBLE CKECK YOUR VERSION WITH THE
// DATASHEET!
//
//*************************************************************
//*** INCLUDE REQUIRED SPECIFIC DEFINTIONS ********************
//*************************************************************
//
#include "CLC.h"											// include definitions
#include "system.h"											// include pin names
#include "uart1.h"											// include uart
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
//*** INITIALISE CLC1 *****************************************
//*************************************************************
//
void clc1_init(void)										// initialise CLC1 hardware
{
	PMD4bits.CLC1MD=0;										// enable CLC1 module
	CLCSELECTbits.SLCT=0b00;								// select CLC1
	// disable gate
	CLCnCONbits.EN=0;										// disable gate while we configure it
	// data selection
	CLCnSEL0bits.D1S=0;										// select CLCIN0PPS (#IO_CS) for input 1
	CLCnSEL1bits.D2S=1;										// select CLCIN1PPS (IO_EN) for input 2
	CLCnSEL2bits.D3S=2;										// not used
	CLCnSEL3bits.D4S=32;									// select DSM1 (CLOCK) for input 4
	// GATE 1 - AND GATE - INVERT INPUTS, INVERT OUTPUT
	CLCnGLS0bits.G1D1N=0;									// not interested
	CLCnGLS0bits.G1D1T=1;									// when IO_CS goes low (inverted)
	CLCnGLS0bits.G1D2N=1;									// when IO_EN goes high (inverted)
	CLCnGLS0bits.G1D2T=0;									// not interested
	CLCnGLS0bits.G1D3N=0;									// not interested
	CLCnGLS0bits.G1D3T=0;									// not interested
	CLCnGLS0bits.G1D4N=1;									// when CLOCK goes high (inverted)
	CLCnGLS0bits.G1D4T=0;									// not interested
	CLCnPOLbits.G1POL=1;									// invert output
	// GATE 2 - JUST OUTPUTS 1
	CLCnGLS1bits.G2D1N=0;									// not interested
	CLCnGLS1bits.G2D1T=0;									// not interested
	CLCnGLS1bits.G2D2N=0;									// not interested
	CLCnGLS1bits.G2D2T=0;									// not interested
	CLCnGLS1bits.G2D3N=0;									// not interested
	CLCnGLS1bits.G2D3T=0;									// not interested
	CLCnGLS1bits.G2D4N=0;									// not interested
	CLCnGLS1bits.G2D4T=0;									// not interested
	CLCnPOLbits.G2POL=1;									// invert output so it is 1
	// GATE 3 - JUST OUTPUTS 1
	CLCnGLS2bits.G3D1N=0;									// not interested
	CLCnGLS2bits.G3D1T=0;									// not interested
	CLCnGLS2bits.G3D2N=0;									// not interested
	CLCnGLS2bits.G3D2T=0;									// not interested
	CLCnGLS2bits.G3D3N=0;									// not interested
	CLCnGLS2bits.G3D3T=0;									// not interested
	CLCnGLS2bits.G3D4N=0;	    							// not interested
	CLCnGLS2bits.G3D4T=0;									// not interested
	CLCnPOLbits.G3POL=1;									// invert output so it is 1
	// GATE 4 - JUST OUTPUTS 1
	CLCnGLS3bits.G4D1N=0;									// not interested
	CLCnGLS3bits.G4D1T=0;									// not interested
	CLCnGLS3bits.G4D2N=0;									// not interested
	CLCnGLS3bits.G4D2T=0;									// not interested
	CLCnGLS3bits.G4D3N=0;									// not interested
	CLCnGLS3bits.G4D3T=0;									// not interested
	CLCnGLS3bits.G4D4N=0;									// not interested
	CLCnGLS3bits.G4D4T=0;									// not interested
	CLCnPOLbits.G4POL=1;									// invert output so it is 1
	// set logic gate
	CLCnCONbits.MODE=0b010;									// 4-input AND gate
	// logic output polarity
	CLCnPOLbits.POL=0;										// output of logic cell not inverted
	// set logic output interrupts
	CLCnCONbits.INTP=1;										// interrupt on rising edge
	CLCnCONbits.INTN=0;										// no interrupt on falling edge
	// finished so enable the gate
	CLCnCONbits.EN=1;										// enable CLC system
	// config interrupt
	IPR0bits.CLC1IP=1;										// CLC1 is high priority
	PIR0bits.CLC1IF=0;										// CLC1 interrupt flags clear
	PIE0bits.CLC1IE=1;										// enable interrupt for CLC1	
}
//
//*************************************************************
//*** INITIALISE CLC2 *****************************************
//*************************************************************
//
void clc2_init(void)										// initialise CLC2 hardware
{
	PMD4bits.CLC2MD=0;										// enable CLC2 module
	CLCSELECTbits.SLCT=0b01;								// select CLC2
	// disable gate
	CLCnCONbits.EN=0;										// disable gate while we configure it
	// data selection
	CLCnSEL0bits.D1S=0;										// not used
	CLCnSEL1bits.D2S=1;										// not used
	CLCnSEL2bits.D3S=2;										// not used
	CLCnSEL3bits.D4S=32;									// not used
	// GATE 1 - JUST OUTPUTS 1
	CLCnGLS0bits.G1D1N=0;									// not interested
	CLCnGLS0bits.G1D1T=0;									// not interested
	CLCnGLS0bits.G1D2N=0;									// not interested
	CLCnGLS0bits.G1D2T=0;									// not interested
	CLCnGLS0bits.G1D3N=0;									// not interested
	CLCnGLS0bits.G1D3T=0;									// not interested
	CLCnGLS0bits.G1D4N=0;									// not interested
	CLCnGLS0bits.G1D4T=0;									// not interested
	CLCnPOLbits.G1POL=1;									// invert output so it is 1
	// GATE 2 - JUST OUTPUTS 1
	CLCnGLS1bits.G2D1N=0;									// not interested
	CLCnGLS1bits.G2D1T=0;									// not interested
	CLCnGLS1bits.G2D2N=0;									// not interested
	CLCnGLS1bits.G2D2T=0;									// not interested
	CLCnGLS1bits.G2D3N=0;									// not interested
	CLCnGLS1bits.G2D3T=0;									// not interested
	CLCnGLS1bits.G2D4N=0;									// not interested
	CLCnGLS1bits.G2D4T=0;									// not interested
	CLCnPOLbits.G2POL=1;									// invert output so it is 1
	// GATE 3 - JUST OUTPUTS 1
	CLCnGLS2bits.G3D1N=0;									// not interested
	CLCnGLS2bits.G3D1T=0;									// not interested
	CLCnGLS2bits.G3D2N=0;									// not interested
	CLCnGLS2bits.G3D2T=0;									// not interested
	CLCnGLS2bits.G3D3N=0;									// not interested
	CLCnGLS2bits.G3D3T=0;									// not interested
	CLCnGLS2bits.G3D4N=0;	    							// not interested
	CLCnGLS2bits.G3D4T=0;									// not interested
	CLCnPOLbits.G3POL=1;									// invert output so it is 1
	// GATE 4 - JUST OUTPUTS 1
	CLCnGLS3bits.G4D1N=0;									// not interested
	CLCnGLS3bits.G4D1T=0;									// not interested
	CLCnGLS3bits.G4D2N=0;									// not interested
	CLCnGLS3bits.G4D2T=0;									// not interested
	CLCnGLS3bits.G4D3N=0;									// not interested
	CLCnGLS3bits.G4D3T=0;									// not interested
	CLCnGLS3bits.G4D4N=0;									// not interested
	CLCnGLS3bits.G4D4T=0;									// not interested
	CLCnPOLbits.G4POL=1;									// invert output so it is 1
	// set logic gate
	CLCnCONbits.MODE=0b010;									// 4-input AND gate
	// logic output polarity
	CLCnPOLbits.POL=0;										// output of logic cell not inverted
	// set logic output interrupts
	CLCnCONbits.INTP=0;										// interrupt on rising edge
	CLCnCONbits.INTN=0;										// no interrupt on falling edge
	// finished so enable the gate
	CLCnCONbits.EN=1;										// enable CLC system
	// config interrupt
	IPR5bits.CLC2IP=1;										// CLC1 is high priority
	PIR5bits.CLC2IF=0;										// CLC1 interrupt flags clear
	PIE5bits.CLC2IE=0;										// enable interrupt for CLC1	
}
//
//*************************************************************
//*** CLC1 INTERRUPT -RESPONDS TO CPU BUS *********************
//*************************************************************
//
//	
// IMPORTANT: THERE IS A MISTAKE IN THE MICROCHIP PIC INC FILE
// WITH THE IRQ NUMBERS, DOUBLE CKECK YOUR VERSION WITH THE
// DATASHEET!
//
// (MICROCHIP LOVE TO KEEP US DEVELOPERS ON OUR TOES!)
//
// MAKE SURE 'IRQ_CLC1' IS CORRECT AS PER THE DATASHEET
//
void __interrupt(irq(IRQ_CLC1),high_priority, base(8)) CLC1_vect_isr()
{   
	if(RW_GET)												// if its a bus read (we send out data)
	{
		if(A1_GET)											// if A1 line is high...
		{
			if(A0_GET)										// if A0 line is high; $D013 (DSOCR)
			{
															// just PIA config register so do nothing
			}
			else											// if A0 line is low; $D012 (DSP)
			{
				DATA_SET = (U1FIFObits.TXBF) ? 0x80 : 0x00;	// load data bus with state of TX buffer
			}
		}
		else												// if A1 line is low...
		{
			if(A0_GET)										// if A0 line is high; $D011 (KBDCR)
			{
				DATA_SET = kbhit() ? 0x80 : 0x00;			// load data bus with state of RX buffer
			} 
			else											// if A0 line is low; $D010 (KBD)
			{
				DATA_SET = kbhit() ? (getch() | 0x80) : 0x00;// load data bus with byte from RX buffer
			}			
		}
		DATA_DIR=0x00;										// set data bus to all outputs (driven)
  	}
	else													// if it's a bus write (we read data)
	{
		if(A1_GET)											// if A1 line is high...
		{
			if(A0_GET)										// if A0 line is high; $D013 (DSOCR)
			{
															// just PIA configuration so do nothing
			}
			else											// if A0 line is low; $D012 (DSP)
			{
				if(!U1FIFObits.TXBF)						// if TX buffer is not full (sanity check)
				{
					U1TXB=DATA_GET & 0x7F;					// load TX buffer with data bus (clear bit 8)
				}
			}
		}
		else												// if A1 line is low...
		{
			if(A0_GET)										// if A0 line is high; $D011 (KBDCR)
			{
															// never writes to KBDCR, do nothing
			}
			else											// if A0 line is low; $D010 (KBD)
			{
															// never writes to KBD, do nothing
			}			
		}						
 	}
	PIR0bits.CLC1IF=0;										// clear interrupt flag
 	CLCSELECTbits.SLCT=0b01;								// select CLC2
	CLCnPOLbits.POL=1;										// output of logic cell not inverted
	CLCnPOLbits.POL=0;										// output of logic cell not inverted
	DATA_DIR=0xFF;											// set data bus to inputs again (high Z)
}
//
//*************************************************************
//*** END *****************************************************
//*************************************************************

