# CRAB APPLE - A MINIMAL 6502 COMPUTER

![6502_SBC](https://user-images.githubusercontent.com/50659826/213731069-e93c9aa4-9158-4db3-b60e-5158fdb3a500.jpg)

This project was initially intended as a personal project but due to the level of interest I have decided to publish the documentation.

## Introduction

The Crab Apple is a very simple and minimalist computer, as such is ideal for the first-time builder wanting to build their own retro-computer.

Just to clarify; **the system will run most software for the Apple I computer but, due to the minimalist nature of the design, it is NOT hardware compatible with the Apple I.**

Ultimately the Crab Apple is minimum complexity, minimum cost and maximum fun!

## Minimal

The Crab Apple is not emulated and does not use an FPGA; the architecture is fairly standard with the exception of an 8-bit microcontroller which will be explained later.

## Memory

At the heart of the Crab Apple is a 65C02 microprocessor. This is the modern version of the classic 6502 device which has a wider operating frequency (you’ll see why later) and still readily available.

The system has 32k of RAM in the form of a 28-pin static RAM chip. It has a 28-pin 8k of ROM chip; this is an EEPROM rather than the older (and considerably more expensive) EPROMs.

Memory address decoding is achieved with a single 7400 (quad NAND gate) chip. This provides quite a loose decoding of the address space and isn’t very efficient but it does the job.

Memory Map:

|Start Address | End Address | Description |
|-----|-----|-----|
| E000 | FFFF | 8K ROM FOR OS |
| 8000 | DFFF | UART |
| 0000 | 7FFF | 32K RAM |

## UART

This is the biggest *hack*; Rather than use several chips the Crab Apple uses a single low-cost microcontroller to act as an all-in-one serial interface. When used in conjunction with a PC running suitable terminal software the system can display text and receive keyboard input.

The microcontroller used (PIC18F15Q41) runs off an internal oscillator at 64MHz (16MIPS), however it still does not have sufficient speed to access and respond to the 6502 data bus  in real-time. To over-come this the PIC also provides the system clock to the CPU. When the PIC is selected by the data bus it halts the system clock (effectively freezing time for the 6502). This enables the PIC to leisurely read from/write to the data bus, before resuming the system clock. 

Despite this momentary drop in CPU clock speed, this has a negligible affect on performance. The biggest ‘bottle-neck’ in the system is the serial BAUD rate. The PIC is capable of higher BAUD rates than older devices; up to 115200. Additionally the PIC can provide a CPU clock speed up to 4MHz, further boosting performance.

Because of this momentary drop is CPU clock speed it is recommended to only use the modern MOS 65C02 CPU.
At high BAUD rates, especially when downloading software, serial flow control is essential to guard against data loss/corruption. The PIC handles this in two ways;

* The PIC uses Xon/Xoff flow control. It is recommended to have this enables in any terminal software connected to the Crab Apple.

* The PIC uses a 256-byte deep receiver buffer. Some USB-to-TTL serial modules insist on sending their entire packet (32 bytes) despite flow control. Once the PICs buffer is 75% full it issues an Xoff to pause the data. It still has room to receive 64 bytes, so no data is lost. The PIC issues the Xon to resume data once the receive buffer is down to 25% full.

## CPU Clock

To reduce cost the PIC provides a user adjustable system clock. Clock speeds of 1MHz, 2MHz or 4MHz can be selected from the BIOS menu.

## CPU Reset

To reduce board complexity the PIC also provides a clean reset, which is triggered on power-up or by the user pressing the reset button.

## Building

The schematic and PCB have been created using KiCAD, these files can be found in the *ELECTRONICS* folder.

You can buy the PCB from here: <a href="https://www.pcbway.com/project/shareproject/MINIMAL_6502_SINGLE_BOARD_COMPUTER_CRAB_APPLE_I_d3186ce3.html">PCBWay</a>

The BOM uses <a href="https://www.mouser.co.uk/">Mouser</a> codes and prices as on January 2023, however buying from <a href="https://www.ebay.co.uk/">Ebay</a> will considerably reduce the cost (buyer be ware!).

I recommend using IC sockets so the ICs can be removed (especially the ROM for programming). As with any build; start with the smallest (profile) parts and build up to the taller components; resistors, capacitors, button, IC sockets, serial connector then LED last.

## PIC18 Programming

To program the PIC18F you will need a PICKIT4 programmer, I do not think the PICKIT3 is compatible with the newer PICs. I cannot comment on the PICKIT3.5 clones.

You will also need to download and install the free programming IDE MPLABX: <a href="https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide"> MPLABX</a>

The source code is written in XC8, the compiler can be downloaded for free: <a href="https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers">XC8</a>

If you prefer to keep it simple the HEX code is here: <a href="https://github.com/gcswales/6502-CRAB-APPLE/blob/main/FIRMWARE/CRAB_APPLE_BIOS.X/dist/default/production/CRAB_APPLE_BIOS.X.production.hex">HEX</a>

The PIC18 can be programmed in-circuit by connecting to the serial connector on the board. You will need to access the PICKIT4 options and enable the PICKIT4 to provide 5V power to the board.

| SERIAL PIN | PICKIT4 PIN |
|--------|---------|
| 1** |  |
| 2 | ICSPDATA |
| 3 | MCLR |
| 4 | Vdd |
| 5 | ICSPCLK |
| 6 | GND |

** PIN1 is the pin closest to the silk screen legend ‘J1’.

## ROM Programming

You will need a suitable EEPROM programmer in order to program the EEPROM. There are many ROM images on line, I would recommend ensuring that whatever image you go for it has the obligatory WOZMON memory monitor in it. You will need to ensure the ROM image you use is targetted at the address range E000 – FFFF.

## Serial Connection

The serial connections with a USB/TTL module are as follows:-
| SERIAL PIN | PICKIT4 PIN |
|--------|---------|
| 1** | DTR |
| 2 | RX |
| 3 | TX |
| 4 | Vcc |
| 5 | CTS |
| 6 | GND |

** PIN1 is the pin closest to the silk screen legend ‘J1’.

![image](https://user-images.githubusercontent.com/50659826/213722178-f4dc8b20-cb57-478c-9e31-6e90f8c39b28.png)

**IMPORTANT: POWER TO THE CRAB APPLE IS PROVIDED BY THE USB/TTL MODULE. ENSURE THE POWER JUMPER ON THE MODULE IS SET TO 5V AS IN THE IMAGE ABOVE.**

The default BAUD rate is 19200. This can be modified by accessing the Crab Apple BIOS menu.
To ensure data integrity it is recommended to enable software flow control (Xon/Xoff).
A lot of Apple I software only sends carriage return (CR) without line feed (LF). For text to display correctly you may need to modify the setting in your serial terminal software to *imply* a LF for every CR received.

## BIOS Menu

The Crab Apple BIOS menu is activated by pressing the space bar during the boot screen while the animated cursor is *rotating*. The menu enables the user to configure the CPU speed, serial BAUD rate and default text colour.

Here is a short video (no sound) showing the BIOS menu in action: <a href="https://youtube.com/shorts/APpxrt6QOSg">YouTube Video</a>

## Thank you

I hope you enjoy the Crab Apple. Please  post photos and comments online and feel free to modify/improve the system.

Best regards,

*Geoffrey*

