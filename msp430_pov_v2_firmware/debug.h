//Using #define we can create macros to add (or not if not defined) the console debug statements
//By defining more than one debug level we can increase the verbosity of the output
//i.e. level 1 = basic, level 2 = verbose

// Enable the debug statements in the main code by uncommenting the #define lines
// 
// DEBUG1 should be basic information only
// DEBUG2 is verbos output
// DEBUG_SLOWDOWN will add delays into the code for easier viewing of debug info

// --------------------------------Change these----------------------------------

//#define DEBUG1
//#define DEBUG2
//#define DEBUG_SLOWDOWN

// ------------------------------------------------------------------------------

#ifdef DEBUG1
  #define DEBUG1_PRINTLN(str) Serial.println(str)
  #define DEBUG1_PRINT(str) Serial.print(str)
  #define DEBUG1_WRITE(str) Serial.write(str)
#else
  #define DEBUG1_PRINTLN(str)
  #define DEBUG1_PRINT(str)
  #define DEBUG1_WRITE(str)
#endif

#ifdef DEBUG2
  #define DEBUG2_PRINTLN(str) Serial.println(str)
  #define DEBUG2_PRINT(str) Serial.print(str)
  #define DEBUG2_WRITE(str) Serial.write(str)
#else
  #define DEBUG2_PRINTLN(str)
  #define DEBUG2_PRINT(str)
  #define DEBUG2_WRITE(str)
#endif

#ifdef DEBUG_SLOWDOWN
  #define DEBUG_SLOWDOWN_1 delay(1000)
  #define DEBUG_SLOWDOWN_2 delay(5000)
#else
  #define DEBUG_SLOWDOWN_1
  #define DEBUG_SLOWDOWN_2
#endif


