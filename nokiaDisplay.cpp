#include "U8glib.h"
#include <Encoder.h>
#include <intervalTimer.h>

#include "debounce.h"
#include "nokiaDisplay.h"
#include "common.h"

#include "baseMenu.h"
#include "fileMenu.h"
#include "normalMenu.h"
#include "settingsMenu.h"
#include "menu.h"

#include "FileTools.h"

#include "systemState.h"

extern SdFat SD;
extern bool done;
//IntervalTimer displayTimer; //Unused - display drawing takes far too long to run in an interrupt

extern displayProperties_t displayProperties;

extern systemState_t systemState;

//current menu
extern menu_t *  menu;


#include "menuDefinitions.h"
debounceISR(encButt) {
	Serial.println("BEEP");
	menu->select();
}

debounceISR(playBut) {
	Serial.println("Play/Pause");
	if (systemState.driveState == play) {
		systemState.driveState = pause;
	} else {
		systemState.driveState = play;
	}
}


debounceISR(backBut) {
	Serial.println("Back");
	menu->unload();
}

Encoder display_t::rotaryEncoder(encPin1, encPin2);


display_t::display_t(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset) : U8GLIB_PCD8544(sck, mosi, cs, a0, reset) {
	setupMenus();

	BUTTON(encButt);
	BUTTON(playBut);
	BUTTON(backBut);

	menu = &topMenu;

	//this->setFont(u8g_font_profont10); 
	//this->setFont(u8g_font_04b_03);
	//this->setFont(u8g_font_4x6);
	//this->setFont(u8g_font_5x8);
	this->setFont(u8g_font_5x7);
	//   this->setFont(u8g_font_6x10);
	//   this->setFont(u8g_font_6x13);
	displayProperties.fontHeight = this->getFontAscent() - this->getFontDescent();
	displayProperties.fontWidth = this->getStrWidth(" ");

	displayProperties.infoBarHeight = displayProperties.fontHeight;


	displayProperties.nRows = (this->getHeight() - displayProperties.infoBarHeight) / displayProperties.fontHeight;
	displayProperties.nCols = this->getWidth() / displayProperties.fontWidth; //Requires a monospace font, so not always reliable


	//Serial.print("We have "); Serial.print(displayProperties.nRows); Serial.println(" slots for menu items.");
	//Serial.println("Obviously we need one for the active item.");
	displayProperties.before = displayProperties.nRows / 2;
	//Serial.print("Assign "); Serial.print(displayProperties.before); Serial.println(" rows before the active");
	displayProperties.after = displayProperties.nRows - displayProperties.before;
	if (isOdd(displayProperties.nRows % 2)) displayProperties.before--;
	//Serial.print("Leaving "); Serial.print(displayProperties.after); Serial.println(" to go after");
	//Serial.print("Thats a total of "); Serial.println(1 + displayProperties.before + displayProperties.after);

	this->setColorIndex(1); // Instructs the display to draw with a pixel on. 


	//displayTimer.begin(updateDisplay, 100000); //Periodically update the display, no need for looping. This might cause issues as this is a pretty massive ISR. Depends how fast the processor is compared to the work load
}

void display_t::updateEncoder() {
	static int previousP = 0;
	int value = rotaryEncoder.read();
	if (value % 4 == 0) {
		int change = value / 4 - previousP;
		//The encoder give 4 ticks per detent
		if (change != 0) {
			menu->changeSelected(change);
			previousP += change;
		}
	}
}

void display_t::updateMainDisplay() {
	this->firstPage();
	do {
		drawInfoBar();
		//     Serial.print("\tMenu is 0x");Serial.println((int)menu,HEX);
		menu->draw();
		//     Serial.println(")");
	} while (this->nextPage());
}

void display_t::update() {
#ifdef verboseDisplayTime
	unsigned long  start = micros();
#endif
	updateEncoder();
	updateMainDisplay();
#ifdef verboseDisplayTime
	unsigned long  now = micros();

	Serial.print("Took ");
	Serial.print(now - start);
	Serial.println(" us");
#endif
	//delay(100);
	systemState.percentage++;
	systemState.percentage = systemState.percentage % 101;
}


void display_t::drawInfoBar(void) {

	uint8_t top = this->getHeight() - displayProperties.infoBarHeight;

	this->setFontPosBaseline();

	//Progress 
	char buf[10];
	const char *space;
	if (systemState.percentage == 100) {
		space = "";
	} else if (systemState.percentage < 10) {
		space = "  ";
	} else {
		space = " ";
	}
	snprintf(buf, 10, "%s%i%%", space, systemState.percentage);
	this->drawStr(displayProperties.infoBarHeight + 1, this->getHeight(), buf);

	//Line seperating the infoBar
	this->drawLine(0, top, this->getWidth(), top);

	//Drive state icon
	if (systemState.driveState == play) {
		this->drawTriangle(0, this->getHeight(), 0, this->getHeight() - displayProperties.infoBarHeight + 1, (displayProperties.infoBarHeight), this->getHeight() - displayProperties.infoBarHeight / 2);
	} else if (systemState.driveState == pause) {
		this->drawBox(0, this->getHeight() - displayProperties.infoBarHeight + 2, (displayProperties.infoBarHeight - 1) / 2, displayProperties.infoBarHeight);
		this->drawBox((displayProperties.infoBarHeight - 1) / 2 + 1, this->getHeight() - displayProperties.infoBarHeight + 2, (displayProperties.infoBarHeight - 1) / 2, displayProperties.infoBarHeight);
	} else if (systemState.driveState == stop) {
		this->drawBox(0, this->getHeight() - displayProperties.infoBarHeight + 2, (displayProperties.infoBarHeight), displayProperties.infoBarHeight);
	}

	bytesToString(systemState.freeSpace, buf, 10);
	this->drawStr(this->getWidth() - this->getStrWidth(buf), this->getHeight(), buf);
}
