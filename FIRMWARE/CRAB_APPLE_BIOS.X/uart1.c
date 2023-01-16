//*************************************************************
//***	FILE:		UART1.C									***
//***	DATE:		28/09/21								***
//***	AUTHOR:		GEOFFREY SWALES							***
//***	VERSION:	V1.0									***
//*************************************************************
//***	DESCRIPTION:										***
//***	UART 1 ROUTINES, RX BUFFER & XON/XOFF FLOW CONTROL	***
//*************************************************************
//***	VERSION TRACK:										***
//***	V0.1: INITIAL DEVELOPMENT VERSION					***
//*************************************************************
//
//*************************************************************
//*** INCLUDE REQUIRED SPECIFIC DEFINTIONS ********************
//*************************************************************
//
#include <xc.h>												// definiti	ons for this PIC
#include <ctype.h>											// needed to convert to upper case
#include "uart1.h"											// some useful UART definitions
#include "system.h"											// some useful sysytem definitions
//
//*************************************************************
//*** GLOBAL VARIABLES ****************************************
//*************************************************************
//
uint8_t buffer[ RX_BUFF_SIZE ];								// buffer holds received UART data
uint8_t buff_in;											// buffer index for data in
uint8_t buff_out;											// buffer index for data out
uint8_t buff_count;											// buffer data counter
//
//*************************************************************
//*** INITIALISE UART *****************************************
//*************************************************************
//
void uart1_init( uint16_t b )								// initialise uart1 hardware
{	
	PMD3bits.U1MD = 0;										// enable: uart 1 module
    PIE4bits.U1RXIE = 0;									// disable UART1 receive interrupt
    PIE4bits.U1TXIE = 0;									// disable UART1 transmit interrupt
    PIE4bits.U1EIE = 0;										// disable UART1 error interrupt
	TX_SET = 1;												// TX line taken high
	TX_DIR = 0;												// TX line is an output (to USB UART RX)
	RA0PPS = 0x10;											// RA0 mapped to UART1 TX	 
	RX_DIR = 1;												// RX line is input (to USB UART TX)
	U1RXPPS = 0b000011;										// UART1 RX mapped to PORTA pin 3
	U1CON0 = 0x00;											// clear all settings
	U1CON0bits.BRGS = 1;									// high speed BAUD clock
	U1CON0bits.TXEN = 1;									// TX is enabled
	U1CON0bits.RXEN = 1;									// RX is enabled
	U1CON1 = 0x00;											// clear all settings here
	U1CON2 = 0x00;											// clear all settings
	U1CON2bits.RUNOVF = 1;									// RX continues after overflow
	switch(b)												// select BAUD settings
	{
		case 1152:											// 115200
			U1BRGL = 0x8A;									// baud rate of 115200
			U1BRGH = 0x00;									// baud rate of 115200
			break;
		case 576:											// 57600
			U1BRGL = 0x15;									// baud rate of 57600
			U1BRGH = 0x01;									// baud rate of 57600
			break;	
		case 192:											// 19200
			U1BRGL = 0x40;									// baud rate of 19200
			U1BRGH = 0x03;									// baud rate of 19200
			break;
		default:											// default 9600 BAUD
		case 96:											// 9600
			U1BRGL = 0x82;									// baud rate of 9600
			U1BRGH = 0x06;									// baud rate of 9600
			break;
	}
    U1FIFO = 0x00;											// clear FIFO status register
    U1UIR = 0x00;											// clear auto-BAUD interrupts
    U1ERRIR = 0x00;											// clear error interrupt flags
	U1ERRIE = 0x00;											// disable error interrupts
    IPR4bits.U1TXIP = 0;									// UART1 TX empty is low-priority interrupt
    IPR4bits.U1EIP = 0;										// UART1 error is low-priority interrupt
    IPR4bits.U1RXIP = 1;									// UART1 RX byte is low-priority interrupt
	buff_in = 0;											// initialise software buffer data in index
	buff_out = 0;											// initialise software buffer data out index
	buff_count = 0;											// initialise software buffer data count
    PIE4bits.U1RXIE = 1;									// UART1 RX interrupt is enabled
    PIE4bits.U1EIE = 1;										// UART1 error interrupt is enabled
	U1CON1bits.ON=1;										// finally enable UART1
	putch( XON );											// send XON handshake to let the data flow!
}
//
//*************************************************************
//*** SENDS BYTE OUT OF SERIAL PORT ***************************
//*************************************************************
//
void putch( uint8_t b )										// sends b out of serial port
{			
	while( U1FIFObits.TXBF );								// wait while TX FIFO is full
	U1TXB = b;												// now write b to UART1 TX FIFO buffer
}
//
//*************************************************************
//*** RETURNS RECEIVED DATA COUNT *****************************
//*************************************************************
//
uint8_t kbhit( void )										// have we received serial data?
{		
	return( buff_count );									// return software serial buffer count	
}
//
//*************************************************************
//*** GET BYTE FROM RECEIVER BUFFER ***************************
//*************************************************************
//
uint8_t getch( void )										// returns byte from receiver buffer
{
	if(!buff_count) return(0);
	buff_count--;											// we have data, decrease buffer count
	if( buff_count == 64 ) putch( XON );					// if buffer is getting low enable data flow (XON)
	return( buffer[ buff_out++ ] );							// return byte from buffer and increment buffer out index
}
//
//*************************************************************
//*** SERVICES UART1 RX INTERRUPT *****************************
//*************************************************************
//
void __interrupt(irq(IRQ_U1RX),high_priority,base(8)) UART1_rx_vect_isr()
{
	buff_count++;											// increment buffer counter
	buffer[ buff_in++ ] = (uint8_t)toupper(U1RXB);			// put upper-case data in buffer and increment index
	if( buff_count == 192 ) putch( XOFF );					// if nearly full, stop data flow (XOFF)
 	U1ERRIRbits.U1RXFOIF=0;									// clear any FIFO overflow error (USE XON/OFF FLOW CONTROL, IDIOT!)
}
//
//*************************************************************
//*** SERVICES UART1 ERROR INTERRUPT **************************
//*************************************************************
//
void __interrupt(irq(IRQ_U1E),low_priority,base(8)) UART1_framing_err_vect_isr()
{
    U1ERRIR = 0;											// clear any error flags
	U1CON1bits.ON = 0;										// try turning it off...
	U1CON1bits.ON = 1;										// then on again!
}
//
//*************************************************************
//*** END OF FILE *********************************************
//*************************************************************
//
