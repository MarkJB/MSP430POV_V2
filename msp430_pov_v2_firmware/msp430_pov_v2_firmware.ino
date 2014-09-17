//MSP430 POV V2 firmware - CCNC 2014
//
//Using the Energia IDE

//TODO: Upload data to display via serial port - working

//TODO: Save uploaded data to persistant storage - working

//TODO: Read data from persistant storage - working

//TODO: Tidy up code and comment out debug print statements (maybe write a debug function or check if one alreay exists)

//Using #define we can create macros to add (or not if not defined) the console debug statements
//By defining more than one debug level we can switch the verbosity of the output
//i.e. level 1 = basic, level 2 = verbose
#include "debug.h"

//Working as a proof of concept. Comment or uncomment the //#define DEBUG1 lines in debug.h to active the debug statements
//Saves about 2k of flash when all turned off

//Include lib to allow access to flash memory
#include "MspFlash.h"

//use one of the 64 byte info segments. Use SEGMENT_B, SEGMENT_C or SEGMENT_D (each 64 bytes, 192 bytes in total)
#define flash SEGMENT_D

//Include a fonts definition table
#include "fonts.h"

//Byte arry

//PACMAN style ghost
byte messageArray1[] = {
B00000000,
B11111100,
B01110010,
B11110011,
B01111111,
B11110011,
B01110010,
B11111100,
B00000000
};

char messageArray2[64];
int messageArray2Length = 0;

//Offset for font table - ASCII val - offset gives position in array that matches the ASCII value
int fontTableIndexOffset = 32;

void setup()
{
  //Enable serial comms
  Serial.begin(9600);
  
  //Port2 P2.6 & P2.7 aren't working on surface mount version
  //by default they are set as xin/xout to use external xtal
  //Set all port 2 pins as input/output here.
  P2SEL = B00000000;

  //enable pins out by port register
  // High = 1 = write
  P2DIR = B11111111;
  
  //Initalise output pins (they will hold a random value after a restart)
  //Set digital pins low
  P2OUT = B0;

  //If defined put in a delay
  DEBUG_SLOWDOWN_2;

  //Echo information at startup
  Serial.println("POV Rev 002 - Test firmware v0.01 - github.com/markjb/msp430pov_v2 - CCNC 2014");
  Serial.println("Commands (r)ead, (w)rite and (e)rase");
  
  //Read in the message stored in flash
  DEBUG1_PRINTLN("Call doRead() to read flash memory");
  doRead();
}

void loop()
{
  // put your main code here, to run repeatedly:

  //Define a pause value for use between led changes (value is in ms)
  int shortPauseValue = 3;
  
  //Write the message array to the LEDs
  DEBUG2_PRINTLN("About to enter loop");
  DEBUG_SLOWDOWN_2;

  //If there is no message data read from flash (i.e messageArray2Length = 0) then display
  // the default pacman ghost otherwise display the message
  if(messageArray2Length>0)
  {

    //display messageArray2
  
    for(int x=0; x<messageArray2Length; x++)
    {
      DEBUG2_PRINTLN("In x part of loop");
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
      delay(shortPauseValue);
      
      //Write out each line of the current char
      for(int y=0; y<5; y++)
      {
         DEBUG2_PRINTLN("In y part of loop");
         DEBUG_SLOWDOWN_1;
                  
         //Check if we have a command waiting on the serial port
         DEBUG2_PRINTLN("Polling for serial data");
         pollForSerialCommand();
                  
         //Get the byte to display from the fonts table and display it here.      
         DEBUG2_PRINT("Getting font data for font[");
         DEBUG2_PRINT(byteToDisplay);
         DEBUG2_PRINT("][");
         DEBUG2_PRINT(y);
         DEBUG2_PRINT("] from font table. Data = ");
         DEBUG2_PRINTLN(font[byteToDisplay][y]);
         DEBUG2_PRINTLN("About to output data to the leds");
         
         P2OUT = font[byteToDisplay][y];
         delay(shortPauseValue);
      }
    }
  }
  else
  {
    //display pacman ghost
    //Write the message array to the LEDs
    DEBUG2_PRINTLN("About to display default pacman style ghost");
    
    for(int x=0; x<=sizeof(messageArray1); x++)
    {
      DEBUG2_PRINTLN("About to output data to the leds");
      P2OUT = messageArray1[x];
      delay(shortPauseValue);
      
      //Check if we have a command waiting on the serial port
      DEBUG2_PRINTLN("Polling for serial data");
      pollForSerialCommand(); 
    }
  }
}

void pollForSerialCommand()
{
  //Read input from serial and call functions if char is a valid command
  DEBUG1_PRINTLN("In pollForSerialCommand()");
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
  unsigned char p = 0;
  int i=0;
  DEBUG2_PRINTLN("Read:");
  DEBUG2_PRINT("Length currently stored in messageArray2Length = ");
  DEBUG2_PRINTLN(messageArray2Length);
  
  do
  {
    Flash.read(flash+i, &p, 1);
    
    //Serial.write(p);
    DEBUG2_WRITE(p);
    //Serial.print(":"); 
    DEBUG2_PRINT(":");   
    //Serial.println(p);
    DEBUG2_PRINTLN(p);
    
    messageArray2Length = i;
    messageArray2[i]=p;
  } while ( p && byte(p) < 255 && (i++ < 64) );
  
  DEBUG2_PRINTLN("End of message in flash segment");
  DEBUG2_PRINT("Length of message read from flash = ");
  DEBUG2_PRINTLN(messageArray2Length);
  DEBUG2_PRINT("Length now stored in messageArray2Length = ");
  DEBUG2_PRINTLN(messageArray2Length);
  
  //echo back the message now stored in messageArray2 (read from flash)
  DEBUG2_PRINTLN("messageArray2[] now contains:");
  for(int x=0; x<messageArray2Length; x++)
  {
    DEBUG2_PRINT(messageArray2[x]);
  }
  DEBUG2_PRINTLN("");

}



void doErase()
{
   DEBUG1_PRINTLN("In doErase()");
   Serial.println("Erase flash"); 
   Flash.erase(flash); 
   Serial.println("Done."); 
 
   DEBUG2_PRINTLN("Erase ram"); 
   //init the array
   //Blank the current message in ram so we don't accidentally store random chars in flash
   for(int x=0; x<=sizeof(messageArray2); x++)
   {
     messageArray2[x]=' ';
   }
   DEBUG2_PRINTLN("Done.");
   Serial.println("Verifying");
   doRead();
   Serial.println("Done.");
}

void doWrite()
{
  DEBUG2_PRINTLN("In doWrite()");
  //Store a message read in from the serial port or from flash memory at power on
  //char messageArray2[64];
  
  //Erase flash before a write (can't write a 1 to flash memory, if it is set to zero it can't be set back unless you erase it)
  doErase();
  
  Serial.println("Write");
  Serial.println("Enter message to write to flash. Terminate string with a '.' (Message entry will timeout in 30 secs)");
  
  //Not used at the moment - need to decide if we need a better terminator char
  int termChar = 10;
  
  Serial.flush();
  Serial.setTimeout(30000);
  
  DEBUG2_PRINTLN("About to readBytesUntil() from serial port");
  int messageArray2Length = Serial.readBytesUntil('.', messageArray2, 64);
  DEBUG2_PRINT("Length returned by readBytesUntil=");
  DEBUG2_PRINTLN(messageArray2Length);
  DEBUG2_PRINTLN("Done with readBytesUntil");
 
  //echo back the whole byte array ignoring message length
  DEBUG2_PRINTLN("Array contains:");
  for(int x=0; x<=sizeof(messageArray2); x++)
  {
    DEBUG2_PRINT(messageArray2[x]);
  }
  DEBUG2_PRINTLN("");
  
  
  Serial.println("Writting to flash");
  Flash.write(flash, (unsigned char*) messageArray2 , messageArray2Length); 
  Serial.println("Done.");
  
  DEBUG1_PRINTLN("Reading back message from flash to populate variables - doRead() called from doWrite()");
  doRead();
  DEBUG2_PRINTLN("Done reading back, exiting doWrite()");
}

void doHelp()
{
  DEBUG1_PRINTLN("In doHelp()");
  //int div = (F_CPU/400000L) & 63; 
  Serial.println("flash test: e, r, w");
  Serial.println(F_CPU);
  //Serial.println(div); 
}
