MSP430POV_V2
============

Version 2 of the MSP430 based persistance of vision thing.

The PCBs for this version are intended to be manufactured professionally.

Basic software to display a Pac Man style ghost included.

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

TODO: Add the abililty to load messages via the com port. Need to look at how to save messages into flash so they are persistant. Currently firmware will need to be loaded onto the chip using a suitable TI Launchpad as a programmer.

Added 'serial_port' branch to test serial coms for uploading message data. So far I am able to send serial data from a PC to the boards (via the launch pad serial port - this should work just as well with a standalone USB to serial addaptor), and display that data on the leds. Sending 4 space chars in a row will reset the displayed message. Need to look at max size of message. Flash limitations may also apply but it more likely that the sweep or arc of movement needed to display a message is too short to allow long strings (unless mounted on a bike wheel).

Reseaching the MspFlash library for writing data to flash (program) memory. https://github.com/energia/Energia/blob/master/hardware/msp430/libraries/MspFlash/examples/flash_readwrite/flash_readwrite.ino

TODO: Add infill on PCBs for next batch.
