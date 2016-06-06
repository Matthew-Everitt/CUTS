//Changes needed in external libraries:
//	Must include the line #define U8G_16BIT 1 in u8g.h for u8glib in order for long strings to work properly (anything longer then 256 pixels will fail otherwise).
//	In order for the SD card pins (SPI peripheral)  to be configurable  SD_SPI_CONFIGURATION must be set to 1 in SdFatConfig.h of sdFat
//

#include "common.h" //Lots of type definitions etc, and includes all the libraries in an effort to get Arduino to actually include them.
#include "nokiaDisplay.h"
#include <U8glib.h>
#include <Encoder.h>
#include <Arduino.h>
#include <SPI.h>
#include <stdbool.h>
#include <SdFat.h>
#include "FileTools.h"
#include "systemState.h"
#include "menu.h"
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
menu_t * menu;


//nokiaDisplay.h which is used in baseMenu.h
display_t display(dispCLK, dispMOSI, dispSCE, dispRST, dispDC);


void toggleLED(void) {
	digitalWrite(LED, !digitalRead(LED));
}

void setup() {
	pinMode(LED, OUTPUT);
	pinMode(inputPin, INPUT);
	attachInterrupt(inputPin, toggleLED, CHANGE);

	setupSD();
}
void loop() {
	display.update();
}
