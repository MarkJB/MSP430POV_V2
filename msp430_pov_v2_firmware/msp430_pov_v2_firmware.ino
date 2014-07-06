//MSP430 POV V2 firmware - CCNC 2014
//
//Using the Energia IDE

//TODO: Upload data to display via serial port
//TODO: Save uploaded data to persistant storage
//TODO: Read data from persistant storage

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

void setup()
{
  // put your setup code here, to run once:

  //enable pins out by port register
  // High = 1 = write
  P2DIR = B11111111;
}

void loop()
{
  // put your main code here, to run repeatedly:

  //Define a pause value for use between led changes (value is in ms)
  int shortPauseValue = 3;
  
  //Define a pause value to hold the led pattern for a bit (value is in ms)
  int longPauseValue = 1000;
  
  //Initalise output pins (they will hold a random value after a restart)
  //Set digital pins low
  P2OUT = B0;

  //Write the message array to the LEDs
  for(int x=0; x<=sizeof(messageArray1); x++)
  {
   P2OUT = messageArray1[x];
   delay(shortPauseValue);
  }
  
}
