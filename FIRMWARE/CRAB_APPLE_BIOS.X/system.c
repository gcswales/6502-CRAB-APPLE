//*************************************************************
//***	FILE:		system.C								***
//***	DATE:		26/09/22								***
//***	AUTHOR:		GEOFFREY SWALES							***
//***	VERSION:	V0.1									***
//*************************************************************
//***	DESCRIPTION:										***
//***	JUST ROUTINES TO CONFIGURE SYSTEM HARDWARE			***
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
#include "system.h"											// include some useful definitions
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
//*** CONFIGURE FUSES *****************************************
//*************************************************************
//
// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator Selection->Oscillator not enabled
#pragma config RSTOSC = HFINTOSC_64MHZ    // Reset Oscillator Selection->HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1
// CONFIG2
#pragma config CLKOUTEN = OFF    // Clock out Enable bit->CLKOUT function is disabled
#pragma config PR1WAY = OFF    // PRLOCKED One-Way Set Enable bit->PRLOCKED bit can be cleared and set only once
#pragma config CSWEN = ON    // Clock Switch Enable bit->Writing to NOSC and NDIV is allowed
#pragma config FCMEN = OFF    // Fail-Safe Clock Monitor Enable bit->Fail-Safe Clock Monitor disabled
// CONFIG3
#pragma config MCLRE = INTMCLR    // MCLR Enable bit->If LVP = 0, MCLR pin function is port defined function; If LVP =1, RE3 pin fuction is MCLR
#pragma config PWRTS = PWRT_64    // Power-up timer selection bits->PWRT set at 64ms
#pragma config MVECEN = ON    // Multi-vector enable bit->Multi-vector enabled, Vector table used for interrupts
#pragma config IVT1WAY = OFF    // IVTLOCK bit One-way set enable bit->IVTLOCKED bit can be cleared and set only once
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit->Low-Power BOR disabled
#pragma config BOREN = SBORDIS    // Brown-out Reset Enable bits->Brown-out Reset enabled , SBOREN bit is ignored
// CONFIG4
#pragma config BORV = VBOR_1P9    // Brown-out Reset Voltage Selection bits->Brown-out Reset Voltage (VBOR) set to 1.9V
#pragma config ZCD = OFF    // ZCD Disable bit->ZCD module is disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON
#pragma config PPS1WAY = OFF    // PPSLOCK bit One-Way Set Enable bit->PPSLOCKED bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle
#pragma config STVREN = ON    // Stack Full/Underflow Reset Enable bit->Stack full/underflow will cause Reset
#pragma config LVP = OFF    // Low Voltage Programming Enable bit->HV on MCLR/VPP must be used for programming
#pragma config XINST = OFF    // Extended Instruction Set Enable bit->Extended Instruction Set and Indexed Addressing Mode disabled
// CONFIG5
#pragma config WDTCPS = WDTCPS_31    // WDT Period selection bits->Divider ratio 1:65536; software control of WDTPS
#pragma config WDTE = OFF    // WDT operating mode->WDT Disabled; SWDTEN is ignored
// CONFIG6
#pragma config WDTCWS = WDTCWS_6    // WDT Window Select bits->window always open (100%); no software control; keyed access required
#pragma config WDTCCS = LFINTOSC    // WDT input clock selector->WDT reference clock is the 31.0 kHz LFINTOSC
// CONFIG7
#pragma config BBSIZE = BBSIZE_512    // Boot Block Size selection bits->Boot Block size is 512 words
#pragma config BBEN = OFF    // Boot Block enable bit->Boot block disabled
#pragma config SAFEN = OFF    // Storage Area Flash enable bit->SAF disabled
#pragma config DEBUG = OFF    // Background Debugger->Background Debugger disabled
// CONFIG8
#pragma config WRTB = OFF    // Boot Block Write Protection bit->Boot Block not Write protected
#pragma config WRTC = OFF    // Configuration Register Write Protection bit->Configuration registers not Write protected
#pragma config WRTD = OFF    // Data EEPROM Write Protection bit->Data EEPROM not Write protected
#pragma config WRTSAF = OFF    // SAF Write protection bit->SAF not Write Protected
#pragma config WRTAPP = OFF    // Application Block write protection bit->Application Block not write protected
// CONFIG10
#pragma config CP = OFF    // PFM and Data EEPROM Code Protection bit->PFM and Data EEPROM code protection disabled
//
//*************************************************************
//*** INITIALISE BASIC HARDWARE *******************************
//*************************************************************
//
void system_init( void )
{
	oscillator_init();										// initialise oscillator
	interrupt_init();										// initialise vector interrupts
    peripheral_disable_init();								// initialise peripheral disable module
    pin_init();												// initialise GPIO pins
}
//
//*************************************************************
//*** INITIALISE OSCILLATOR ***********************************
//*************************************************************
//
void oscillator_init( void )								// configure oscillator
{
	OSCCON1 = 0x60;											// this isn't realy needed
	OSCCON3 = 0x00;											// as most of it is done with the
	OSCEN = 0x00;											// CONFIG fuses
	OSCFRQ = 0x08;											// it is only needed if you want to
	OSCTUNE = 0x00;											// use auto-tune or a secondary
	ACTCON = 0x00;											// oscillator for power saving applications.
}
//
//*************************************************************
//*** INITIALISE PERIPHERAL DISABLE MODULE ********************
//*************************************************************
//
void peripheral_disable_init( void )
{
    PMD0bits.SYSCMD = 0;									// enable: system clock network
	PMD0bits.FVRMD = 1;										// disable: fixed voltage reference
	PMD0bits.LVDMD = 1;										// disable: low-voltage detect
	PMD0bits.CRCMD = 1;										// disable: CRC module
	PMD0bits.SCANMD = 0;									// enable: NV Memory scan
	PMD0bits.CLKRMD = 1;									// disable: clock reference module
	PMD0bits.IOCMD = 1;										// disable interrupt on change
 //
	PMD1 = 0xFF;											// disable all peripherals
	PMD2 = 0xFF;											// disable all peripherals
	PMD3 = 0xFF;											// disable all peripherals
	PMD4 = 0xFF;											// disable all peripherals
	PMD5 = 0xFF;											// disable all peripherals
}
//
//*************************************************************
//*** INITIALISE GPIO PINS ************************************
//*************************************************************
//
void pin_init( void )
{
// configure pins to bog-standard GPIO pins
	INLVLA = 0x00;											// TTL input on port A
    INLVLB = 0x00;											// TTL input on port B
    INLVLC = 0x00;											// TTL input on port C
    ANSELA = 0x00;											// turn off analogue on port A
    ANSELB = 0x00;											// turn off analogue on port B
    ANSELC = 0x00;											// turn off analogue on port C
    WPUA = 0x00;											// turn off weak pull-ups on port A
    WPUB = 0x00;											// turn off weak pull-ups on port B
    WPUC = 0x00;											// turn off weak pull-ups on port C
	SLRCONA = 0x00;											// set slew rate to max on port A
	SLRCONB = 0x00;											// set slew rate to max on port B
	SLRCONC = 0x00;											// set slew rate to max on port C
	ODCONA = 0x00;											// turn off open-drain on port A
	ODCONB = 0x00;											// turn off open-drain on port B
	ODCONC = 0x00;											// turn off open-drain on port C
// configure reset button to trigger interrupt - INT0
	WPUAbits.WPUA1 = 1;										// enable weak pull-up, saves on resistors
	BUTTON_DIR = 1;											// BUTTON pin is an input
	INT0PPS = 0b00000001;									// BUTTON pin re-mapped to INT0
	INTCON0bits.INT0EDG = 0;								// INT0 interrupt on falling edge - button press	
	IPR1bits.INT0IP=0;										// INT0 is low priority
	PIR1bits.INT0IF=0;										// clear INT0 interrupt flag
	PIE1bits.INT0IE=1;										// INT0 interrupt enabled
//
	RW_DIR = 1;												// R/#W pin is an input
//
	CLOCK_DIR = 0;											// CLOCK pin is an output
	CLOCK_SET = 1;											// CLOCK pin is high
	RA4PPS = 0x26;											// CLOCK is re-mapped to DSM output	
//
	RESET_DIR = 0;											// RESET is an output
	RESET_SET = 0;											// RESET is low, keep CPU sleeping for now
//
	IO_CS_DIR=1;											// IO_CS pin is an input
	CLCIN0PPS = 0b00001100;									// CLCIN0=PORTB PIN 4
//
	IO_EN_DIR=1;											// IO_EN pin is an input
	CLCIN1PPS = 0b00001101;									// CLCIN1=PORTB PIN 5
//
	A0_DIR=1;												// CPU A0 pin is an input
	A1_DIR=1;												// CPU A1 pin is an input
//
	DATA_DIR=0xFF;											// DATA BUS port is all inputs
}
//
//*************************************************************
//*** INITIALISE VECTOR INTERRUPTS ****************************
//*************************************************************
//
void interrupt_init( void )
{
    INTCON0bits.IPEN = 1;									// enable priority interrupts
    bool state = (unsigned char)GIE;						// get state of global interrupt enable
    GIE = 0;												// turn off global interrupts
    IVTLOCK = 0x55;											// first part of password
    IVTLOCK = 0xAA;											// second part of password
    IVTLOCKbits.IVTLOCKED = 0x00;							// now we can unlock IVT			
    IVTBASEU = 0;											// vector table address upper byte
    IVTBASEH = 0;											// vector table address high byte
    IVTBASEL = 8;											// vector table address low byte
    IVTLOCK = 0x55;											// first part of password
    IVTLOCK = 0xAA;											// second part of password
    IVTLOCKbits.IVTLOCKED = 0x01;							// now we can lock IVT			
    GIE = state;											// restore state of global interrupt enable
}
//
//*************************************************************
//*** INT0 INTERRUPT - RESET BUTTON PRESSED! ******************
//*************************************************************
//
void __interrupt(irq(IRQ_INT0),low_priority, base(8)) INT0_vect_isr()
{   
	PIR1bits.INT0IF = 0;									// clear INT0 interrupt flag
	RESET_SET = 0;											// RESET set low, keep CPU sleeping for now
	RESET();												// then just reset everything...
}
//
//*************************************************************
//*** END *****************************************************
//*************************************************************
//
