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

TODO: Add the abililty to load messages via the com port. Need to look at how to save messages into flash so they are persistant. Currently firmware will need to be loaded onto the chip using a suitable TI Launchpad as a programmer.