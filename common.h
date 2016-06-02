#pragma once
#include <U8glib.h>
#include <Encoder.h>
#include <Arduino.h>
#include <SPI.h>
#include <stdbool.h>
#include <SdFat.h>


extern SdFat SD;

#define carrierTimer Timer1
// #define chatty
#define reportFunctionCalls
#define reportBlockDetails


#ifdef chatty
  #define detailedChecksum
  #define rawBytes
  #define twoByteNumbers
  #define reportFreqChanges
  #define reportFunctionCalls
  #define reportBlockDetails
  #define reportCarrierState
  #define verboseFile
  #define reportRawBits
#endif






enum pins{
  //LCD display pins
  //TODO - Check these, I think DC & RST are swapped
  dispCLK  = 18,
  dispMOSI = 19,
  dispSCE  = 22,
  dispRST  = 20,
  dispDC   = 21,
  dispLED  = 23,

  //Rotary encoder pins
  encPin1  = 9,
  encPin2  = 10,
  encButt  = 11,
  
  //Simple buttons
  playBut  =  3, //Not used?
  backBut  =  2,
  
  //CUTS interface pins
  inputPin  = 15,
  outputPin = 13,
  
  //SD card pins
  chipSelect = 6,
  sdMOSI = 7,
  sdMISO = 12,
  sdSCK  = 14,
};


typedef uint8_t byte;

typedef enum frequencies {
  lowFreq  = 0,
  highFreq = 1,
  transitionFreq,
  unknownFreq,
} frequency;

class format{
  public:
    virtual void RX ( byte b, String defaultFilename )=0; //We recieve data from the Atom one byte at a time
    virtual void TX ( File * file )=0; //But we might as well send an entire file at once.
};


typedef struct {
  char name[9];
  char ext[4];
} name_t;


/*Prototype for the byte handing functions. Functions take the byte as input, and return 'finished', so true means move on to next function, false means pass next byte to this function*/
typedef bool (*byteHandler)(byte b);

typedef uint8_t bit;


typedef struct{
  bool overwrite;
  bool detokenise;
} settings_t;

typedef struct{
  uint8_t fontHeight;
  uint8_t infoBarHeight;
  uint8_t nRows;
  uint8_t before;
  uint8_t after;
} displayProperties_t;



extern U8GLIB_PCD8544 disp;
extern displayProperties_t displayProperties;
