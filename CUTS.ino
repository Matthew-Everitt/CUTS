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
SdFat SD;

systemState_t systemState;
settings_t settings;

void setup() {
	setupDisplay();
	setupSD();
}

void loop() {
	updateDisplay();
}