//*************************************************************
//***	FILE:		UART1.H									***
//***	DATE:		28/09/21								***
//***	AUTHOR:		GEOFFREY SWALES							***
//***	VERSION:	V1.0									***
//*************************************************************
//***	DESCRIPTION:										***
//***	HANDLES ALL UART ACCESS, BUFFERING AND INTERRUPTS   ***
//*************************************************************
//***	VERSION TRACK:										***
//***	V0.1: INITIAL DEVELOPMENT VERSION					***
//*************************************************************
//
//
//*************************************************************
//*** REDEFINITION BLOCKING ***********************************
//*************************************************************
//
#ifndef _UART1_H											// if not define
#define _UART1_H											// define it now
//
//*************************************************************
//*** C++ COMPATABILITY ***************************************
//*************************************************************
//
#ifdef __cplusplus
extern "C" {
#endif
//
//*************************************************************
//*** INCLUDE USEFUL COMPILER DEFINTIONS **********************
//*************************************************************
//
#include <xc.h>												// defines PIC registers
#include <stdbool.h>										// defines standard boolian types
#include <stdint.h>											// define industrial standard types
#include <stdio.h>											// we need standard IO definitions
//
//*************************************************************
//*** USEFUL DEFINITIONS **************************************
//*************************************************************
//
#define		RX_BUFF_SIZE		256							// software buffer size
#define		XON					0x11						// XON token for flow control
#define		XOFF				0x13						// XOFF token for flow control
//
#define		ANSI_BLACK			0							// ANSI colour black
#define		ANSI_RED			1							// ANSI colour red
#define		ANSI_GREEN			2							// ANSI colour green
#define		ANSI_YELLOW			3							// ANSI colour yellow
#define		ANSI_BLUE			4							// ANSI colour blue
#define		ANSI_MAGENTA		5							// ANSI colour magenta
#define		ANSI_CYAN			6							// ANSI colour cyan
#define		ANSI_WHITE			7							// ANSI colour white
//
//*************************************************************
//*** DECLARE NEW STRUCURES ***********************************
//*************************************************************
//  

//
//*************************************************************
//*** DECLARE EXTERNAL VARIABLES ******************************
//*************************************************************
//
	
//
//*************************************************************
//*** PROTOTYPE FUNCTIONS *************************************
//*************************************************************
//
void uart1_init( uint16_t b );								// initialise UART1 hardware
void putch( uint8_t b );									// transmit a byte
uint8_t getch( void );										// receive a byte
uint8_t kbhit( void );										// see if we have received a byte
//
//*************************************************************
//*** END C++ COMPATABILITY ***********************************
//*************************************************************
//
#ifdef __cplusplus
}
#endif
//
//*************************************************************
//*** END OF REDEFINITION BLOCKING ****************************
//*************************************************************
//
#endif	