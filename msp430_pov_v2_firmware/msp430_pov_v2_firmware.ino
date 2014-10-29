//MSP430 POV HW Rev2 firmware v1.00 - CCNC 2014
//
//Using the Energia IDE

//See debug.h for details of enabling or disabling the debug statements
//Saves about 2k of flash when all turned off
#include "debug.h"

//Include a fonts definition table - need to rewrite this from scratch
#include "fonts.h"

//Include lib to allow access to flash memory
#include "MspFlash.h"

//use one of the 64 byte info segments. Use SEGMENT_B, SEGMENT_C or SEGMENT_D (each 64 bytes, 192 bytes in total)
#define flash SEGMENT_D

//Byte arry
//PACMAN style ghost displayed by default

byte messageArray1[] = {
B00000000,
B00111111,
B01001110,
B11001111,
B11111110,
B11001111,
B01001110,
B00111111,
B00000000
};

//Array to hold ASCII message
char messageArray2[64];
int messageArray2Length = 0;

//Offset for font table - ASCII val - offset gives position in array that matches the ASCII value
int fontTableIndexOffset = 32;

//Define a pause value for use between led changes (value is in ms)
//Large values make characters appear wider, smaller values make them thinner.
//msg1 pause is for the default message routine.
//msg2 pause is for the ASCII message display routine
//Char spacing alters the gap between ASCII chars
int msg1PauseValue = 5;
int msg2PauseValue = 5;
int charSpacingPauseValue = 5;

void setup()
{
  //Enable serial comms
  Serial.begin(9600);
  
  //By default Port2 P2.6 & P2.7 are set as xin/xout to use external xtal
  //To use them as GPIO, set all port 2 pins as input/output here.
  P2SEL = B00000000;

  //enable pins out by port register
  // High = 1 = write
  P2DIR = B11111111;
  
  //Initalise output pins (they will hold a random value after a restart)
  //Set digital pins low
  P2OUT = B0;

  //If defined put in a delay
  DEBUG_SLOWDOWN_2;

  //Echo useful information at startup
  Serial.println("POV HW Rev 002 - firmware v1.00 - github.com/markjb/msp430pov_v2 - CCNC 2014");
  Serial.print("CPU Frequency = ");
  Serial.print((F_CPU/1000000));
  Serial.println("MHz");
  
  //Read in the message stored in flash
  DEBUG1_PRINTLN("Call doRead() to read flash memory");
  doRead();
  
}

void loop()
{
  //Main code to write the message array to the LEDs

  DEBUG2_PRINTLN("About to enter led printing loop");
  DEBUG_SLOWDOWN_1;

  //If there is no message data read from flash (i.e messageArray2Length = 0) then display
  //the default pacman ghost otherwise display the message retrived from flash
  
  DEBUG2_PRINTLN("Checking length of message array. If 0 then default else use messageArray2");
  DEBUG2_PRINT("messageArray2Length = ");
  DEBUG2_PRINTLN(int(messageArray2Length));
  
  if(messageArray2Length!=0)
  {
    //display messageArray2
    DEBUG2_PRINTLN("In display messageArray2 branch");
    
    //Loop thru messageArray2 one char at a time until there are no more left
    for(int x=0; x<messageArray2Length; x++)
    {
      DEBUG2_PRINTLN("In x part of messageArray2 led printing loop");
      DEBUG_SLOWDOWN_1;
      
      //Get the char value of the current char in the message
      char charToDisplay = messageArray2[x];
      byte byteToDisplay = byte(charToDisplay);
      
      //Include the offset value to make the ascii value match that of the font index array so we pick the right char
      byteToDisplay = byteToDisplay - fontTableIndexOffset;
      DEBUG2_PRINT("Current char has value :");
      DEBUG2_PRINTLN(byteToDisplay);   
      
      //Write a blank line before any char to give space between letters
      P2OUT = 0;
      delay(charSpacingPauseValue);
      
      //Write out each line of the current char
      for(int y=0; y<5; y++)
      {
         DEBUG2_PRINTLN("In y part of messageArray2 led printing loop");
         DEBUG_SLOWDOWN_1;    
         
         //Get the byte to display from the fonts table and display it here.      
         DEBUG2_PRINT("Getting font data for font[");
         DEBUG2_PRINT(byteToDisplay);
         DEBUG2_PRINT("][");
         DEBUG2_PRINT(y);
         DEBUG2_PRINT("] from font table. Data = ");
         DEBUG2_PRINTLN(font[byteToDisplay][y]);
         DEBUG2_PRINTLN("About to output data to the leds");
         
         //This is where the data is written to the leds.
         P2OUT = font[byteToDisplay][y];
         delay(msg2PauseValue);
         
      }
    }
    
    //Check if we have a command waiting on the serial port
    DEBUG2_PRINTLN("Polling for serial data from message2 loop");
    pollForSerialCommand();
    
  }
  else
  {
    //display messageArray1 (pacman style ghost)
    DEBUG2_PRINTLN("In display messageArray1 branch");
    DEBUG2_PRINTLN("-------------------------------------------");
    DEBUG2_PRINTLN("About to display default pacman style ghost");
    
    //Loop thru messageArray1 one byte at a time until there are no more left
    for(int x=0; x<=sizeof(messageArray1); x++)
    {
      DEBUG2_PRINTLN("About to output messageArray1 data to the leds");
      DEBUG2_PRINT("Outputting: ");
      DEBUG2_PRINTLN(messageArray1[x]);
      
      //This is where the data is written to the leds.
      P2OUT = messageArray1[x];
      delay(msg1PauseValue);
      
    }
    
    //Check if we have a command waiting on the serial port
    DEBUG2_PRINTLN("Polling for serial data from default message loop");
    pollForSerialCommand(); 
    
  }
  
}

void pollForSerialCommand()
{
  //Read input from serial and call functions if char is a valid command
  DEBUG2_PRINTLN("In pollForSerialCommand()");
  if ( Serial.available() )
  {
    switch ( Serial.read() )
    { 
      case 'e': doErase(); break;  
      case 'r': doRead(); break; 
      case 'w': doWrite(); break; 
      case 10:
      case 13: break;
      default: doHelp();
    }
  } 
}

void doRead()
{
  DEBUG1_PRINTLN("In doRead()");
  
  Serial.println("----------------------");
  Serial.println("Reading flash.........");
  
  DEBUG2_PRINT("Length currently stored in messageArray2Length = ");
  DEBUG2_PRINTLN(messageArray2Length);

  unsigned char p = 0;
  int i=0;
    
  do
  {
    Flash.read(flash+i, &p, 1);
    
    DEBUG2_WRITE(p);
    DEBUG2_PRINT(":");   
    DEBUG2_PRINTLN(p);
    DEBUG2_PRINT("Using the Flash.read() var i which is currently = ");
    DEBUG2_PRINTLN(i);

    messageArray2Length = i;
    messageArray2[i]=p;
  } while ( p && byte(p) < 255 && (i++ < 64) );
  
  DEBUG1_PRINTLN("Reached end of message in flash segment");
  DEBUG1_PRINT("Length of message read from flash (messageArray2Length) = ");
  DEBUG1_PRINTLN(messageArray2Length);
  
  //echo back the message now stored in messageArray2 (read from flash)
  DEBUG1_PRINT("messageArray2[] now contains: '");
  
  for(int x=0; x<messageArray2Length; x++)
  {
    DEBUG1_PRINT(messageArray2[x]);
  }
  DEBUG1_PRINTLN("'");
  
  Serial.println("Reading flash.....Done");
  Serial.println("----------------------");
  
  DEBUG_SLOWDOWN_2;
  
  Serial.println("Commands (r)ead, (w)rite and (e)rase");
}



void doErase()
{
  DEBUG1_PRINTLN("In doErase()");
  
  Serial.println("----------------------");
  Serial.println("Erasing flash........."); 
  
  Flash.erase(flash); 
  
  Serial.println("Erasing flash.....Done"); 
 
  DEBUG1_PRINTLN("Erasing ram..........."); 
  
  //Blank the current message in ram so we don't accidentally store random chars in flash
  for(int x=0; x<=sizeof(messageArray2); x++)
  {
    messageArray2[x]=' ';
  }
  
  DEBUG1_PRINTLN("Erasing ram.......Done");
  DEBUG1_PRINTLN("Verifying flash.......");
  
  doRead();
  
  DEBUG1_PRINTLN("Verifying flash...Done");
  DEBUG_SLOWDOWN_2;
}

void doWrite()
{
  //Store a message read in from the serial port or from flash memory at power on
  
  DEBUG1_PRINTLN("In doWrite()");
    
  //Erase flash before a write (can't write a 1 to flash memory, if it is set to zero it can't be set back unless you erase it)
  
  doErase();
  
  Serial.println("----------------------");
  Serial.println("Write to flash........");
  Serial.println("Enter message to write to flash. Terminate string with a '.' (Message entry will timeout in 30 secs)");
  
  //Not used at the moment - need to decide if we need a better terminator char
  //int termChar = 10;
  
  Serial.flush();
  Serial.setTimeout(30000);
  
  DEBUG1_PRINTLN("About to readBytesUntil() from serial port");
  
  int messageArray2Length = Serial.readBytesUntil('.', messageArray2, 64);
  
  DEBUG1_PRINT("Length returned by readBytesUntil=");
  DEBUG1_PRINTLN(messageArray2Length);
  DEBUG1_PRINTLN("Done with readBytesUntil");
 
  //echo back the whole byte array ignoring message length
  DEBUG1_PRINTLN("Array contains:");
 
  for(int x=0; x<=sizeof(messageArray2); x++)
  {
    DEBUG1_PRINT(messageArray2[x]);
  }
  
  DEBUG1_PRINTLN("");
  
  
  Serial.println("Writing flash.........");
  
  Flash.write(flash, (unsigned char*) messageArray2 , messageArray2Length); 
  
  Serial.println("Writing flash.....Done");
  
  DEBUG1_PRINTLN("Reading back message from flash to populate variables (doRead() called from doWrite())");
  
  doRead();
  
  DEBUG1_PRINTLN("Reading back message from flash to populate variables... Done.");
  DEBUG1_PRINTLN("exiting doWrite()");
  DEBUG_SLOWDOWN_2;
}

void doHelp()
{
  DEBUG1_PRINTLN("In doHelp()");
  Serial.println("Commands (r)ead, (w)rite and (e)rase");
}
