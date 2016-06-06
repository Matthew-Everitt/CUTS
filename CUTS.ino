//Changes needed in external libraries:
//	Must include the line #define U8G_16BIT 1 in u8g.h for u8glib in order for long strings to work properly (anything longer then 256 pixels will fail otherwise).
//	In order for the SD card pins (SPI peripheral)  to be configurable  SD_SPI_CONFIGURATION must be set to 1 in SdFatConfig.h of sdFat
//

#include "common.h" //Lots of type definitinons etc, and includes all the libraries in an effort to get Arduino to actually include them.
#include "nokiaDisplay.h"
#include <U8glib.h>
#include <Encoder.h>
#include <Arduino.h>
#include <SPI.h>
#include <stdbool.h>
#include <SdFat.h>
#include "FileTools.h"
#include "systemState.h"

//common.h
//debugLevels.h
//FileTools.h
//namedFile.h
//pinout.h
//systemState.h
//fileMenu.cpp
//FileTools.cpp
//namedFile.cpp
//nokiaDisplay.cpp
//	
SdFat SD;

//systemState.h
//FileTools.cpp
//namedFile.cpp
//nokiaDisplay.cpp
//	
systemState_t systemState;

//common.h
//menuDefinitions.h
//settingsMenu.h
//namedFile.cpp
//settingsMenu.cpp
//	
settings_t settings;

//
//common.h
//debugLevels.h
//fileSendMenu.h
//menu.h
//nokiaDisplay.h
//pinout.h
//settingsMenu.h
//baseMenu.cpp
//nokiaDisplay.cpp
//settingsMenu.cpp
//	
U8GLIB_PCD8544 disp(dispCLK, dispMOSI, dispSCE, dispRST, dispDC);


//common.h
//baseMenu.cpp
//nokiaDisplay.cpp
//settingsMenu.cpp
//	
displayProperties_t displayProperties;


//nokiaDisplay.cpp
//	
Encoder rotaryEncoder(encPin1, encPin2);


//baseMenu.h
//common.h
//debugLevels.h
//fileMenu.h
//fileSendMenu.h
//menu.h
//menuDefinitions.h
//normalMenu.h
//settingsMenu.h
//baseMenu.cpp
//fileMenu.cpp
//fileSendMenu.cpp
//nokiaDisplay.cpp
//normalMenu.cpp
//settingsMenu.cpp
//CUTS.ino
menu_t * menu;

void toggleLED(void) {
	digitalWrite(LED, !digitalRead(LED));
}

void setup() {
	pinMode(LED, OUTPUT);
	pinMode(inputPin, INPUT);
	attachInterrupt(inputPin, toggleLED, CHANGE);
	display::setupDisplay();
	setupSD();
}

void loop() {
	display::updateDisplay();
}
