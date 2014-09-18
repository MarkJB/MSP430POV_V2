MSP430POV_V2
============

Revison 2 of the MSP430 based persistance of vision hardware.

The PCBs for this version are intended to be manufactured professionally.

================================HARDWARE=============================

22/07/2014 Received first batch of thru-hole PCBs. Assembled and tested board. When running on CR2302 works for about 30 seconds before locking up. Tried external 3V battery (2xAA) but exhibited same behaviour. Added 10uF cap. System locks up on CR2303 after about 30 seconds but runs fine on external 3V battery. Reflashed board (via P2 header connected to Launchpad) with current firmware and board set to 1MHz. Board now runs fine on CR2302. Need to determine if 10uF is necessary or if system is stable @ 1MHz without it.

To add a 1MHz board to Energia:

Add the follow to the boards.txt file in energia/hardware/msp430/boards.txt

##############################################################
lpmsp430g2553_1.name=LaunchPad w/ msp430g2553 (1MHz)
lpmsp430g2553_1.upload.protocol=rf2500
lpmsp430g2553_1.upload.maximum_size=16384
lpmsp430g2553_1.build.mcu=msp430g2553
lpmsp430g2553_1.build.f_cpu=1000000L
lpmsp430g2553_1.build.core=msp430
lpmsp430g2553_1.build.variant=launchpad
lpmsp430g2553_1.upload.maximum_ram_size=512

Restart Energia - check for extra board (Tools -> Boards)

28/07/2014 Received the first batch of SMT (surface mount) PCBs. Assembled and tested board without 10uF cap. Worked ok but two LEDs were out (P2.6 & P2.7). Checked LEDs and chip. Replaced chip. Same problem. Discovered that on blank chips the shared pins were in external xtal mode (P2.6 & P2.7 were set to xin and xout). Cleared the P2SEL register (setting all bits to zero) sets all pins to be port 2 input/outputs. 

Removed the 10uF cap from the thru-hole board and it works fine @ 1MHz. Don't think I need to alter the PCB to include it. Will need to see how well the boards perform as the battery voltage drops.

==========================FIRMWARE===================================

Master branch is the current stable v1.00 firmware.

It allows you to upload ASCII chars via serial. These messages are then stored in flash and will persist after power off.

If no message exists, the default pacman style ghost is displayed.

To upload ASCII messages, connect a serial terminal and power on and you will see info and a menu:

>POV HW Rev 002 - firmware v1.00 - github.com/markjb/msp430pov_v2 - CCNC 2014
>CPU Frequency = 1MHz
>----------------------
>Reading flash.........
>Reading flash.....Done
>----------------------
>Commands (r)ead, (w)rite and (e)rase

Read  - will read the current content of the flash memory.
Erase - will erase the flash memory.
Write - will erase, write and read the flash memory.

You only need to use write as this will first call erase, then read, then write the message, then call read again.

>w
>----------------------
>Erasing flash.........
>Erasing flash.....Done
>----------------------
>Reading flash.........
>Reading flash.....Done
>----------------------
>Commands (r)ead, (w)rite and (e)rase
>----------------------
>Write to flash........
>Enter message to write to flash. Terminate string with a '.' (Message entry will timeout in 30 secs)

Type a message using the basic ASCII character set, end the message with a full stop (e.g. '.') and press return.

>Hello World!.
Writing flash.........
Writing flash.....Done
----------------------
Reading flash.........
Reading flash.....Done
----------------------
Commands (r)ead, (w)rite and (e)rase

Extra debug output can be enabled by uncommenting the #define DEBUG lines in debug.h.

e.g. look for these lines and remove the //
//#define DEBUG1
//#define DEBUG2
//#define DEBUG_SLOWDOWN

DEBUG1 gives some extra info but should not interfere with normal operation.
DEBUG2 gives a lot more info and will slow down the led display. You will 
probably need to uncomment DEBUG_SLOWDOWN to add pauses between the debug statements.