//MSP430 POV V2 firmware - CCNC 2014
//
//Using the Energia IDE

// Testing flash memory erase/write/read

//TODO: Upload data to display via serial port - sort of working

//TODO: Save uploaded data to persistant storage
//TODO: Read data from persistant storage

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
  
  //Read in the message stored in flash
  doRead();
}

void loop()
{
  // put your main code here, to run repeatedly:

  //Define a pause value for use between led changes (value is in ms)
  int shortPauseValue = 3;
  
  //Write the message array to the LEDs
  for(int x=0; x<messageArray2Length; x++)
  {
    //Get the char value of the current char in the message
    char charToDisplay = messageArray2[x];
    byte byteToDisplay = byte(charToDisplay);
    //Serial.print("Current char has value :");
    //Serial.println(byteToDisplay);   
    for(int y=0; y<6; y++)
    {
       //Get the byte to display from the fonts table and display it here.
       Serial.print("About to fiddle with the leds");
       //P2OUT = messageArray[x];
       P2OUT = font[byteToDisplay][y];
       delay(shortPauseValue);
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
  }
}

void doRead()
{
  unsigned char p = 0;
  int i=0;
  //Serial.println("Read:");
  //Serial.print("Length currently stored in messageArray2Length = ");
  //Serial.println(messageArray2Length);
  
  do
  {
    Flash.read(flash+i, &p, 1);
    //Serial.write(p);
    //Serial.print(":");    
    //Serial.println(p);
    messageArray2Length = i;
    messageArray2[i]=p;
  } while ( p && byte(p) < 255 && (i++ < 64) );
  //Serial.println("End of message in flash segment");
  //Serial.print("Length of message read from flash = ");
  //Serial.println(messageArray2Length);
  //Serial.print("Length now stored in messageArray2Length = ");
  //Serial.println(messageArray2Length);
  
  //echo back the message now stored in messageArray2 (read from flash)
  /*Serial.println("messageArray2[] now contains:");
  for(int x=0; x<messageArray2Length; x++)
  {
    Serial.print(messageArray2[x]);
  }
  Serial.println("");
  */
}


void doErase()
{
 //Serial.println("Erase"); 
 Flash.erase(flash); 
 //Serial.println("Done."); 
}

void doWrite()
{
  //Store a message read in from the serial port or from flash memory at power on
  //char messageArray2[64];
  
  //init the array
  //Blank the current message in ram so we don't accidentally store random chars in flash
  for(int x=0; x<=sizeof(messageArray2); x++)
  {
    messageArray2[x]=' ';
  }
  //Serial.println("Erasing flash...");
  doErase();
  
  Serial.println("Write");
  Serial.println("Enter message to write to flash: (Will timeout in 30 secs)");
  int termChar = 10;
  
  Serial.flush();
  Serial.setTimeout(30000);
  int messageArray2Length = Serial.readBytesUntil('.', messageArray2, 64);
  //Serial.print("Length returned by readBytesUntil=");
  //Serial.println(messageArray2Length);
  //Serial.println("Done with readBytesUntil");
 
  //echo back the whole byte array ignoring message length
  /*
  Serial.println("Array contains:");
  for(int x=0; x<=sizeof(messageArray2); x++)
  {
    Serial.print(messageArray2[x]);
  }
  Serial.println("");
  */
  
  //Serial.println("Writting to flash");
  Flash.write(flash, (unsigned char*) messageArray2 , messageArray2Length); 
  //Serial.println("Done.");
}

void doHelp()
{
  //int div = (F_CPU/400000L) & 63; 
  Serial.println("flash test: e, r, w");
  Serial.println(F_CPU);
  //Serial.println(div); 
}
