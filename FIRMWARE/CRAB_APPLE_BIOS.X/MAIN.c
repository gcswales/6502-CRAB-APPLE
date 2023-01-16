//*************************************************************
//***	FILE:		MAIN.C									***
//***	DATE:		26/09/22								***
//***	AUTHOR:		GEOFFREY SWALES							***
//***	VERSION:	V0.1									***
//*************************************************************
//***	DESCRIPTION:										***
//***	A LITTLE BIOS SYSTEM THAT HANDLES SOME FETURES FOR	***
//***	A SMALL 6502 BASED SYSTEM:-							***
//***	*	CONFIGUIRABLE CPU CLOCK SOURCE					***
//***	*	CLEAN CPU RESET CONTROL							***
//***	*	UART WITH CONFIGURABLE BAUD RATES				***
//***	*	256 BYTE RX BUFFER AND FLOW CONTROL				***
//*************************************************************
//***	VERSION TRACK:										***
//***	V0.1: INITIAL DEVELOPMENT VERSION					***
//***	V1.0: IT WORKS!!!									***
//***	v1.1: POLISHED TURD									*** 
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
#include <xc.h>												// PIC hardware definitions
#include "system.h"											// our system hardware definitions
#include "uart1.h"											// our uart definitions
#include "CLOCK.h"											// our clock definitions
#include "CLC.h"											// our clc definitions
//
//*************************************************************
//*** STRUCTURE DEFINITIONS ***********************************
//*************************************************************
//
typedef union												// keep all settings in one place
{
	uint8_t arr[5];											// array overlay for saving with a loop
	struct
	{
		uint8_t ver;										// settings version
		uint8_t speed;										// CPU clock speed
		uint16_t baud;										// UART BAUD rate
		uint8_t colour;										// screen text colour
	} settings;												// all part of 'settings' structure
} _eprom;
_eprom eprom;												// declare object as 'eprom'
//
//*************************************************************
//*** GLOBAL VARIABLES ****************************************
//*************************************************************
//

//
//*************************************************************
//*** DEFINITIONS *********************************************
//*************************************************************
//
#define	BIOS_VERSION	11									// BIOS version 1.1
#define BIOS_DELAY		20									// BIOS menu wait time (20*100ms)
#define	DEFAULT_SPEED	1									// default clock speed of 1MHz
#define DEFAULT_COLOUR	7 									// default text colour is 7 (white)
#define	DEFAULT_BAUD	192									// default baud speed is 19200
//
//*************************************************************
//*** FUNCTION PROTOTYPES *************************************
//*************************************************************
//
void main(void);											// main routine
void reset_init(void);										// performs CPU reset sequence
void print_title(void);										// prints tit;e to UART
void clear_screen(void);									// clear UART screen
void text_colour(uint8_t c);								// set UART text colour
void bios_menu(void);										// enables BIOS menu
void bios_menu_top(void);									// main BIOS menu
void bios_menu_speed(void);									// CPU speed menu
void bios_menu_baud(void);									// BAUD speed menu
void bios_menu_colour(void);								// UART text colour menu
void load_settings(void);									// load user settings from flash
void save_settings(void);									// save user settings to flash
void dfm_write(uint16_t adr,uint8_t dat);					// write byte to 'Data Flash Memory' (was EEPROM)
uint8_t dfm_read(uint16_t adr);								// read byte from 'DFM' (see above)
void menu_print_colour(void);								// prints colour in menu
void bios_waiting(void);									// services animated rotating cursor thingy
//
//*************************************************************
//*** MAIN ROUTINE ********************************************
//*************************************************************
//
void main(void)
{
	system_init();											// initialise system hardware
	clock_init();											// initialise processor clock
	clc1_init();											// initialise configurable logic control block 1
	clc2_init();											// initialise configurable logic control block 2
	load_settings();										// load BIOS settings from flash
	uart1_init(eprom.settings.baud);						// initialise UART1 based on BIOS BAUD settings
	INTERRUPT_GlobalInterruptHighEnable();					// Enable high priority global interrupts
	INTERRUPT_GlobalInterruptLowEnable();					// Enable low priority global interrupts
	__delay_ms(100);										// small delay to let voltages settle
	print_title();											// print title screen
	bios_menu();											// run BIOS menu (activated by SPACE BAR)
	text_colour(eprom.settings.colour);						// set text colour to user setting
	putch('\r');											// start on a new line
	reset_init();											// perform processor reset sequence
	while(1)												// main loop...
	{
		NOP();												// nothing to do, interrupts handle everything!
	}
}
//
//*************************************************************
//*** INITIALISE RESET LINE SEQUENCE **************************
//*************************************************************
//
void reset_init(void)
{
	while(!BUTTON_GET);										// wait while BUTTON line is low
	__delay_ms(5);											// wait for any button debounce to pass
	clock_set(eprom.settings.speed);						// start CPU clock based on user settings
	__delay_ms(2);											// small delay
	RESET_SET=1;											// set RESET line high - wake up CPU!
}
//
//*************************************************************
//*** CLEAR SCREEN ********************************************
//*************************************************************
//
void clear_screen(void)
{
	printf( "\e[0m\e[2J\e[H\e[1m");							// clear screen, home, bright text
	text_colour(eprom.settings.colour);						// set text colour to user setting
}
//
//*************************************************************
//*** SET TEXT COLOUR *****************************************
//*************************************************************
//
void text_colour(uint8_t c)
{
	printf( "\e[3%um",c);									// send ANSI foreground colour sequence
}
//
//*************************************************************
//*** PRINT BIOS TITLE SCREEN *********************************
//*************************************************************
//
void print_title(void)										// prints pretty colour apple logo
{
	uint8_t v=BIOS_VERSION;									// load v with firmware version
	clear_screen();											// clear the screen
	text_colour(ANSI_GREEN);								// set colour to green
	printf("\r                   #");	
	printf("\r                 ###");		
	printf("\r               ####");
	printf("\r               ##");
	printf("\r        ######   ######");	
	printf("\r      ###################");	
	text_colour(ANSI_YELLOW);								// set colour to yellow
	printf("\r     ####################");
	printf("\r     ###################");
	printf("\r    ###################");
	text_colour(ANSI_RED);									// set colour to red
	printf("\r    ###################");
	printf("\r    ###################");
	printf("\r    ####################");
	text_colour(ANSI_MAGENTA);								// set colour to magenta
	printf("\r     #####################");
	printf("\r     #####################");
	printf("\r      ###################");
	text_colour(ANSI_BLUE);									// set colour to blue
	printf("\r       ##################");
	printf("\r        ################");
	printf("\r         #####    ####");
	printf("\r");
	text_colour(ANSI_WHITE);								// set colour to white
	printf("\r     CRAB APPLE BIOS V%u.%u ", v/10,v%10);	// display name & version
}
//
//*************************************************************
//*** SERVICE BIOS MENU - HIT [SPACE BAR] *********************
//*************************************************************
//
void bios_menu(void)										// wait to see if user needs menu
{
	uint8_t w=BIOS_DELAY;									// loop for a delay period
	while(w--)												// loop while period is non-zero
	{
		if(kbhit())											// if keyboard is hit
		{
			if(getch()==' ')								// if key pressed is [spacebar]
			{
				bios_menu_top();							// call bios main menu
			}
			return;											// once menu done, just return
		}
		bios_waiting();										// service rotating cursor thingy
	}
}
//
//*************************************************************
//*** SERVICES 'ROTATING LINE' PROMPT *************************
//*************************************************************
//
void bios_waiting(void)
{
	static uint8_t i=0;										// steps animation sequence
	switch(i++)												// next step and show...
	{
		case 0:												
			printf("\b|");									// print back-space then cursor sequence
			break;											// thats all for now
		case 1:												
			printf("\b/");									// print back-space then cursor sequence
			break;											// that's all for now
		case 2:
			printf("\b-");									// print back-space then cursor sequence
			break;											// that's all for now
		case 3:
			printf("\b\\");									// print back-space then cursor sequence
			i=0;											// reset animation sequence for next time
			break;											// that's all for now
	}
	__delay_ms(100);										// small delay, to make things visible
	printf("\b ");											// clear cursor artifact incase we're done
}
//
//*************************************************************
//*** HANDLES BIOS TOP MENU ***********************************
//*************************************************************
//
void bios_menu_top(void)									// bios main menu
{
	uint8_t m=0;											// holds our menu selection
	while(m!='\e')											// while Esc isn't pressed
	{
		clear_screen();										// clear the screen
		printf ("\r\t1 - PROCESSOR SPEED\r");				// print processor speed
		printf ("\r\t2 - UART BAUD RATE\r");				// print uart speed
		printf ("\r\t3 - TEXT COLOUR\r");					// print text colour
		printf ("\r\t[ESC] - EXIT\r");						// print exit
		while(!kbhit());									// loop until key press
		m=getch();											// get the key press
		if(m=='1') bios_menu_speed();						// if '1' then call CPU speed menu
		if(m=='2') bios_menu_baud();						// if '2' then baud menu
		if(m=='3') bios_menu_colour();						// if '3' then colour menu
	}
	RESET();												// RESET! - 'cos it's tidy
}
//
//*************************************************************
//*** HANDLES BIOS PROCESSOR SPEED MENU ***********************
//*************************************************************
//
void bios_menu_speed(void)									// CPU speed menu
{
	uint8_t m=0;											// holds our menu selection
	while(m!='\e')											// while Esc isn't pressed
	{
		clear_screen();										// clear the screen
		printf("\r\tPROCESSOR SPEED : %u MHz\r",eprom.settings.speed );// print menu title
		printf ("\r\t1 - 1MHz\r");							// print option 1MHz
		printf ("\r\t2 - 2MHz\r");							// print option 2MHz
		printf ("\r\t3 - 4MHz\r");							// print option 4MHz
		printf ("\r\t[ESC] - EXIT\r");						// print Esc to exit
		while(!kbhit());									// wait for key press
		m=getch();											// get key press
		if(m=='1') eprom.settings.speed=1;					// if '1' then set speed
		if(m=='2') eprom.settings.speed=2;					// if '2' then set speed
		if(m=='3') eprom.settings.speed=4;					// if '3' then set speed
	}	
	save_settings();										// save user setting to flash
}
//
//*************************************************************
//*** HANDLES BIOS BAUD RATE MENU *****************************
//*************************************************************
//
void bios_menu_baud(void)									// baud speed menu
{
	uint8_t m=0;											// holds our menu selection
	while(m!='\e')											// while Esc isn't pressed
	{
		clear_screen();										// clear the screen
		printf("\r\tBAUD RATE : %u00\r",eprom.settings.baud );// print menu
		printf ("\r\t1 - 9600\r");
		printf ("\r\t2 - 19200\r");
		printf ("\r\t3 - 57600\r");
		printf ("\r\t4 - 115200\r");
		printf ("\r\t[ESC] - EXIT\r");
		while(!kbhit());									// wait for key press
		m=getch();											// get key press
		if(m=='1') eprom.settings.baud=96;					// set baud rate setting
		if(m=='2') eprom.settings.baud=192;					// based on key press
		if(m=='3') eprom.settings.baud=576;
		if(m=='4') eprom.settings.baud=1152;
	}	
	save_settings();										// save settings to flash
}
//
//*************************************************************
//*** HANDLES BIOS TEXT COLOUR MENU ***************************
//*************************************************************
//
void bios_menu_colour(void)									// text colour menu
{
	uint8_t m=0;											// see comments in above function
	while(m!='\e')
	{
		clear_screen();
		printf("\r\tTEXT COLOUR : ");
		menu_print_colour();
		text_colour(1);
		printf ("\r\t1 - RED\r");
		text_colour(2);
		printf ("\r\t2 - GREEN\r");
		text_colour(3);
		printf ("\r\t3 - YELLOW\r");
		text_colour(4);
		printf ("\r\t4 - BLUE\r");
		text_colour(5);
		printf ("\r\t5 - MAGENTA\r");
		text_colour(6);
		printf ("\r\t6 - CYAN\r");
		text_colour(7);
		printf ("\r\t7 - WHITE\r");
		text_colour(eprom.settings.colour);
		printf ("\r\t[ESC] - EXIT\r");
		while(!kbhit());
		m=getch();
		if(m=='1') eprom.settings.colour=1;
		if(m=='2') eprom.settings.colour=2;
		if(m=='3') eprom.settings.colour=3;
		if(m=='4') eprom.settings.colour=4;
		if(m=='5') eprom.settings.colour=5;
		if(m=='6') eprom.settings.colour=6;
		if(m=='7') eprom.settings.colour=7;
	}
	save_settings();	
}
//
//*************************************************************
//*** PRINTS SELECTED MENU TEXT COLOUR ************************
//*************************************************************
//
void menu_print_colour(void)								// prints user setting colour as text
{
	switch(eprom.settings.colour)							// switch based on user settings colour
	{
		default:											// this is also the default colour
		case ANSI_RED:
			printf("RED\r");								// print colour 'red'
			break;
		case ANSI_GREEN:
			printf("GREEN\r");								// print colour 'green'
			break;	
		case ANSI_YELLOW:
			printf("YELLOW\r");								// print colour 'yellow'
			break;	
		case ANSI_BLUE:
			printf("BLUE\r");								// print colour 'blue'
			break;		
		case ANSI_MAGENTA:
			printf("MAGENTA\r");							// print colour 'magenta'
			break;		
		case ANSI_CYAN:
			printf("CYAN\r");								// print colour 'cyan'
			break;		
		case ANSI_WHITE:
			printf("WHITE\r");								// print colour 'white'
			break;			
	}
}
//
//*************************************************************
//*** LOADS USER SETTINGS FROM FLASH DATA *********************
//*************************************************************
//
void load_settings(void)									// load user settings from flash
{
	uint8_t adr=sizeof(_eprom);								// set address to (size) end of setting array
	while(adr--)											// while we loop down through address
	{
		eprom.arr[adr]=dfm_read(adr);						// read flash into array
	}
	if(eprom.settings.ver!=BIOS_VERSION)					// if settings version is wrong
	{
		eprom.settings.ver=BIOS_VERSION;					// store default version
		eprom.settings.colour=DEFAULT_COLOUR;				// store default colour
		eprom.settings.baud=DEFAULT_BAUD;					// store default baud
		eprom.settings.speed=DEFAULT_SPEED;					// store default cpu speed
		save_settings();									// save settings to flash
	}
}
//
//*************************************************************
//*** SAVES USER SETTINGS TO FLASH DATA ***********************
//*************************************************************
//
void save_settings(void)									// save user setting to flash
{
	uint8_t adr=sizeof(_eprom);								// address is (size) end of eprom structure
	while(adr--)											// loop down through addresses
	{
		if(dfm_read(adr)!=eprom.arr[adr])					// if current setting not same as setting in flash
		{
			dfm_write(adr,eprom.arr[adr]);					// write current setting to flash
		}
	}
}
//
//*************************************************************
//*** READ BYTE FROM DATA FLASH *******************************
//*************************************************************
//
uint8_t dfm_read(uint16_t adr)								// reads data from Data Flash Memory (AKA EEPROM)
{
	NVMADR=0x380000 + adr;									// get address from DFM address plus our offset adr
	NVMCON1bits.CMD=0x00;									// byte read command
	NVMCON0bits.GO=1;										// just do it
	while(NVMCON0bits.GO);									// wait until read is complete
	return(NVMDATL);										// return data
}
//
//*************************************************************
//*** WRITE BYTE TO DATA FLASH ********************************
//*************************************************************
//
void dfm_write(uint16_t adr,uint8_t dat)					// write data to Data Flash Memory (AKA EEPROM)
{
	uint8_t GIEBitValue = INTCON0bits.GIE;					// get status of global interrupt
	NVMADR=0x380000 + adr;									// get address from DFM address plus our offset adr
	NVMDATL = dat;											// Load NVMDAT with the desired value
	NVMCON1bits.CMD = 0x03;									// Set the byte write command	
	INTCON0bits.GIE = 0;									// Disable global interrupt
	NVMLOCK = 0x55;											// flash unlock sequence
	NVMLOCK = 0xAA;											// flash unlock sequence
	NVMCON0bits.GO = 1;										// Start byte write
	while(NVMCON0bits.GO);									// Wait for the write operation to complete
	if (NVMCON1bits.WRERR)									// Verify byte write operation success
	{
		// should do something here but I don't know what!
	}
	INTCON0bits.GIE = GIEBitValue;							// Restore interrupt enable bit value
	NVMCON1bits.CMD = 0;									// Disable writes to memory
}
//
//*************************************************************
//*** END *****************************************************
//*************************************************************
//