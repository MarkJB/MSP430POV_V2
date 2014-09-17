//#define DEBUG1
//#define DEBUG2
//#define DEBUG_SLOWDOWN

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
  #define DEBUG_SLOWDOWN_2 delay(2000)
#else
  #define DEBUG_SLOWDOWN_1
  #define DEBUG_SLOWDOWN_2
#endif


