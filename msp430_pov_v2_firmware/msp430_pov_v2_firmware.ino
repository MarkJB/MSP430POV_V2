//MSP430 POV V2 firmware - CCNC 2014
//
//Using the Energia IDE

//TODO: Upload data to display via serial port
//WORKINPROGRESS: Able to upload 256 char data via serial port and display it on the leds. Adding reset command (4 space chars (32dec, 0x20hex))

//TODO: Save uploaded data to persistant storage
//TODO: Read data from persistant storage

// Test Byte arry
// PACMAN style ghost
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

//Variable to hold incoming serial data (one byte at a time)
byte incomingByte = 0;

//Variable to store incoming bytes to display
//Note: Need to invesitgate dynamic arrays
char messageArray[256];

//Variable to index message array (so we don't display empty bits of the array on the leds)
int messageArrayIndex = 0;

//Variable to track consecutive space char count - if we see 4 in a row then reset the messageArrayIndex to 0
int resetCharTracker = 0;

//Do setup bits and bobs here (i.e. initialise pins and ports)
void setup()
{
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
  
  //Setup hardware serial port
  Serial.begin(9600);
  
  //Write some data to the serial port so we know it is working
  //Users probably won't see this message unless the serial port
  //is being monitored at power up. Still useful as a diagnostic.
  Serial.println("Firmware version 0001");
}


void loop()
{
  //Define a pause value for use between led changes (value is in ms)
  int shortPauseValue = 3;
  


  //Write the test message array (pacman ghost) to the LEDs
  /*for(int x=0; x<=sizeof(messageArray1); x++)
  {
   P2OUT = messageArray1[x];
   delay(shortPauseValue);
  }
  */

  //Write read in message to leds
  for(int x=0; x<=messageArrayIndex; x++)
  {
   P2OUT = messageArray[x];
   delay(shortPauseValue);
  }
  

  //TODO: Delete
  //P2OUT = incomingByte;
  //delay(shortPauseValue);
  //delay(100); 

  
  //Read some test data from the serial port and echo it back to the serial port
  if (Serial.available() > 0) 
  {
    // read the incoming byte:
    incomingByte = Serial.read();

    //TODO: Command to reset message array - 4 spaces?
    //Track number of char 32 (Space char)
    if (incomingByte == 0x20)
    {
      resetCharTracker++;
      Serial.print("Incrementing resetCharTracker = ");
      Serial.println(resetCharTracker);
    }
    else
    {
      resetCharTracker=0;
      Serial.print("Restting resetCharTracker = ");
      Serial.println(resetCharTracker);
    }

    //If number of char 32 (Space char) = 4 then reset messageArrayIndex (and by extension, the message displayed) and resetCharTracker
    if (resetCharTracker > 3)
    {
      messageArrayIndex=0;
      messageArray[0]=0x00;
      resetCharTracker=0;
      Serial.println("Resetting messageArrayIndex and resetCharTracker");
      Serial.print("messageArrayIndex now = ");
      Serial.println(messageArrayIndex);
    }

    //TODO: When the reset occurs this next bit still adds a space char to the message array - need to stop that happening...

    if (messageArrayIndex <= 256)
    {
      messageArray[messageArrayIndex] = incomingByte;
      messageArrayIndex++;
      
      //TODO: print whole message here? This is only practical for debug
      for(int x=0; x<messageArrayIndex; x++)
      {
        Serial.print(messageArray[x]);
        Serial.print(":");
      }
      Serial.println("");
    }
    else
    {
      Serial.println("Maximum array size reached, resetting message");
      messageArrayIndex=0;
    }
    
    //TODO: Remove this? Useful for debug
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);

  }
  
}
