# makefile for ECE-231 Arduino Uno projects
# revision history
#	Date		Author			Revision
#	2/14/22		D. McLaughlin	initial release 

# Instructions: 
# (1) You need to copy this makefile into the project
# folder for each coding project you undertake. 
# (2) Edit the SERIALPORT variable:  replace "/dev/tty.usbmodem2101" 
# with the port your Arduino UNO is connected to. eg, COM2 on Win10 or 
# usbmodemxyz on macOS. If you're unsure, type "ls /dev/tty/*" from macOS 
# Terminal for a listing of devices and ports. For windows, check 
# Device Manager>Ports  to find your Arduino Uno COM port
# (3) Replace "blink.c" with the name of your source code file
# (4) To compile, simply type "make"
# (5) To flash compiled code to Arduino, type "make flash"

SERIALPORT = COM3
SOURCEFILE = lab2.c

begin:	lab2.hex

lab2.hex: lab2.elf
	rm -f lab2.hex
	avr-objcopy -j .text -j .data -O ihex lab2.elf lab2.hex
	avr-size --format=avr --mcu=atmega328p lab2.elf

lab2.elf: $(SOURCEFILE)
	avr-gcc -Wall -Os -DF_CPU=16000000 -mmcu=atmega328p -o lab2.elf $(SOURCEFILE)

flash:	begin
	avrdude -c arduino -b 115200 -P $(SERIALPORT) -p atmega328p -U flash:w:lab2.hex:i
