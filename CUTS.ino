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

void setup() {
	setupDisplay();
	setupSD();
}

void loop() {
	updateDisplay();
}