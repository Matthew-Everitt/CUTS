#include "U8glib.h"
#include <Encoder.h>
#include <intervalTimer.h>

#include "debounce.h"
#include "nokiaDisplay.h"
#include "common.h"

#include "baseMenu.h"
#include "fileMenu.h"
#include "normalMenu.h"
#include "menu.h"

#include "FileTools.h"

#include "systemState.h"

extern SdFat SD;

IntervalTimer displayTimer;

displayProperties_t displayProperties;
//I think DC & RST may be swapped
U8GLIB_PCD8544 disp(dispCLK, dispMOSI, dispSCE, dispRST, dispDC);
Encoder rotaryEncoder(encPin1, encPin2);

extern systemState_t systemState;

//current menu
menu_t *  menu;

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


void setupDisplay() {
	setupMenus();
	pinMode(13, OUTPUT);

	BUTTON(encButt);
	BUTTON(playBut);
	BUTTON(backBut);

	menu = &topMenu;

	//disp.setFont(u8g_font_profont10); 
	//disp.setFont(u8g_font_04b_03);
	//disp.setFont(u8g_font_4x6);
	//disp.setFont(u8g_font_5x8);
	disp.setFont(u8g_font_5x7);
	//   disp.setFont(u8g_font_6x10);
	//   disp.setFont(u8g_font_6x13);
	displayProperties.fontHeight = disp.getFontAscent() - disp.getFontDescent();
	displayProperties.fontWidth = disp.getStrWidth(" ");

	displayProperties.infoBarHeight = displayProperties.fontHeight;


	displayProperties.nRows = (disp.getHeight() - displayProperties.infoBarHeight) / displayProperties.fontHeight;
	displayProperties.nCols = disp.getWidth() / displayProperties.fontWidth; //Requires a monospace font, so not always reliable


	Serial.print("We have "); Serial.print(displayProperties.nRows); Serial.println(" slots for menu items.");
	Serial.println("Obviously we need one for the active item.");
	displayProperties.before = displayProperties.nRows / 2;
	Serial.print("Assign "); Serial.print(displayProperties.before); Serial.println(" rows before the active");
	displayProperties.after = displayProperties.nRows - displayProperties.before;
	if (isOdd(displayProperties.nRows % 2 )) displayProperties.before--;
	Serial.print("Leaving "); Serial.print(displayProperties.after); Serial.println(" to go after");
	Serial.print("Thats a total of "); Serial.println(1 + displayProperties.before + displayProperties.after);
	

	//Serial.print("Before :  "); Serial.println(displayProperties.before);
	//Serial.print("After  :  "); Serial.println(displayProperties.after);
	//Serial.print("nRows  :  "); Serial.println(displayProperties.nRows);
	//Serial.print("nCols  :  "); Serial.println(displayProperties.nCols);
	disp.setColorIndex(1); // Instructs the display to draw with a pixel on. 


	//displayTimer.begin(updateDisplay, 100000); //Periodically update the display, no need for looping. This might cause issues as this is a pretty massive ISR. Depends how fast the processor is compared to the work load
}

void updateEncoder() {
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

void updateMainDisplay() {
	disp.firstPage();
	do {
		drawInfoBar();
		//     Serial.print("\tMenu is 0x");Serial.println((int)menu,HEX);
		menu->draw();
		//     Serial.println(")");
	} while (disp.nextPage());
}

void updateDisplay() {
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


void drawInfoBar(void) {

	uint8_t top = disp.getHeight() - displayProperties.infoBarHeight;

	disp.setFontPosBaseline();

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
	disp.drawStr(displayProperties.infoBarHeight + 1, disp.getHeight(), buf);

	//Line seperating the infoBar
	disp.drawLine(0, top, disp.getWidth(), top);

	//Drive state icon
	if (systemState.driveState == play) {
		disp.drawTriangle(0, disp.getHeight(), 0, disp.getHeight() - displayProperties.infoBarHeight + 1, (displayProperties.infoBarHeight), disp.getHeight() - displayProperties.infoBarHeight / 2);
	} else if (systemState.driveState == pause) {
		disp.drawBox(0, disp.getHeight() - displayProperties.infoBarHeight + 2, (displayProperties.infoBarHeight - 1) / 2, displayProperties.infoBarHeight);
		disp.drawBox((displayProperties.infoBarHeight - 1) / 2 + 1, disp.getHeight() - displayProperties.infoBarHeight + 2, (displayProperties.infoBarHeight - 1) / 2, displayProperties.infoBarHeight);
	} else if (systemState.driveState == stop) {
		disp.drawBox(0, disp.getHeight() - displayProperties.infoBarHeight + 2, (displayProperties.infoBarHeight), displayProperties.infoBarHeight);
	}

	bytesToString(systemState.freeSpace, buf, 10);
	disp.drawStr(disp.getWidth() - disp.getStrWidth(buf), disp.getHeight(), buf);
}

