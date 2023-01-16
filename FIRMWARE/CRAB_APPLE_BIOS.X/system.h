//*************************************************************
//***	FILE:		SYSTEM.H								***
//***	DATE:		28/09/21								***
//***	AUTHOR:		GEOFFREY SWALES							***
//***	VERSION:	V1.0									***
//*************************************************************
//***	DESCRIPTION:										***
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
#ifndef SYSTEM_H
#define	SYSTEM_H
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
#include <xc.h>												// PIC18F definitions
#include <stdint.h>											// industrial standard variable types
#include <stdbool.h>										// some boolian definitions
//
//*************************************************************
//*** USEFUL DEFINITIONS **************************************
//*************************************************************
//
#define _XTAL_FREQ 64000000									// define system speed
//
#define INTERRUPT_GlobalInterruptHighEnable() (INTCON0bits.GIEH = 1)
#define INTERRUPT_GlobalInterruptHighDisable() (INTCON0bits.GIEH = 0)
#define INTERRUPT_GlobalInterruptLowEnable() (INTCON0bits.GIEL = 1)
#define INTERRUPT_GlobalInterruptLowDisable() (INTCON0bits.GIEL = 0)
//
//*************************************************************
//*** DECLARE PIN NAMES ***************************************
//*************************************************************
// 
// UART TX OUTPUT (DISPLAY)
//
#define	TX_SET			LATAbits.LATA0
#define	TX_GET			PORTAbits.RA0
#define	TX_DIR			TRISAbits.TRISA0
//
// RESET BUTTON INPUT
//
#define	BUTTON_SET		LATAbits.LATA1
#define	BUTTON_GET		PORTAbits.RA1
#define	BUTTON_DIR		TRISAbits.TRISA1
//
// 6502 R/#W INPUT
//
#define	RW_SET			LATAbits.LATA2
#define	RW_GET			PORTAbits.RA2
#define	RW_DIR			TRISAbits.TRISA2
//
// UART RX INPUT (KEYBOARD)
//
#define	RX_SET			LATAbits.LATA3
#define	RX_GET			PORTAbits.RA3
#define	RX_DIR			TRISAbits.TRISA3
//
// CPU CLOCK OUTPUT
//
#define	CLOCK_SET		LATAbits.LATA4
#define	CLOCK_GET		PORTAbits.RA4
#define	CLOCK_DIR		TRISAbits.TRISA4
//
// CPU RESET OUTPUT
//
#define	RESET_SET		LATAbits.LATA5
#define	RESET_GET		PORTAbits.RA5
#define	RESET_DIR		TRISAbits.TRISA5
//
// OUT CHIP SELECT ACTIVE LOW INPUT
//
#define	IO_CS_SET		LATBbits.LATB4
#define	IO_CS_GET		PORTBbits.RB4
#define	IO_CS_DIR		TRISBbits.TRISB4
//
// OUR CHIP ENABLE ACTIVE HIGH INPUT
//
#define	IO_EN_SET		LATBbits.LATB5
#define	IO_EN_GET		PORTBbits.RB5
#define	IO_EN_DIR		TRISBbits.TRISB5
//
// CPU ADDRESS LINE 1 INPUT
//
#define	A1_SET			LATBbits.LATB6
#define	A1_GET			PORTBbits.RB6
#define	A1_DIR			TRISBbits.TRISB6
//
// CPU ADDRESS LINE 0 INPUT
//
#define	A0_SET			LATBbits.LATB7
#define	A0_GET			PORTBbits.RB7
#define	A0_DIR			TRISBbits.TRISB7
//
// CPU DATA BUS INPUT (8-BITS)
//
#define	DATA_SET		LATC
#define	DATA_GET		PORTC
#define	DATA_DIR		TRISC
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
void system_init( void );									// initialise basic hardare
void oscillator_init( void );								// initialise oscillator
void peripheral_disable_init( void );						// initialise peripheral disable module
void pin_init( void );										// initialise pins
void interrupt_init( void );								// initialise vector interrupts
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