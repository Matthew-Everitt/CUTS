#include "baseMenu.h"
#include "menu.h"
#include "common.h"
#include "nokiaDisplay.h"
extern menu_t * menu;

baseMenu_t::baseMenu_t(menu_t *  parent) {
	this->parent = parent;
}


void baseMenu_t::setNEntries(int n) {
	this->nEntries = n;
}

void baseMenu_t::changeSelected(int s) {
	Serial.print("\tGoing from ");
	Serial.print(this->selected);
	Serial.print(" to ");

	this->selected += s;

	Serial.println(this->selected);

	if (this->selected < 0) this->selected += this->nEntries;
	Serial.print("\tWrapped to ");
	this->selected = (this->selected) % (this->nEntries);
	Serial.println(this->selected);
}

void baseMenu_t::draw() {
	//   Serial.print("Selected is ");
	//   Serial.println(this->selected);
	int count = min(displayProperties.nRows - 1, this->nEntries);
	int start = (int)(this->selected - displayProperties.before); //Have some before the selected.
	int correction = min(0, (int)(this->nEntries - (this->selected + displayProperties.after))); //Step back to get the 

  //   Serial.print( count );
  //   Serial.println( " lines to print.");
  //   Serial.print("Going from ");Serial.print(start);Serial.print(" + "); Serial.println(-correction);
	start += correction;
	start = max(0, start);
	int linger = 15; // Time to wait at the start / end of a string, in pixels. About 15 is ok?
	int scrollStepTimeInMilliseconds = 150;
	unsigned long scroll = millis();
	for (byte i = 0; i < count; i++) {


		Serial.println(this->getString(i + start));
		long  d = disp.getStrWidth(this->getString(i + start)) + 1;
		Serial.print("Is "); Serial.print(d); Serial.println(" pixels long.");
		long overflow = d - disp.getWidth();
		int pos = 1;
		Serial.print("Thats "); Serial.print(overflow); Serial.println(" pixels longer than the screen is wide.");
		if (overflow > 0) {
			Serial.print(overflow);
			Serial.print(",");
			Serial.print(scroll);
			Serial.print(",");
			int displacement = (scroll / scrollStepTimeInMilliseconds) % (overflow + 2 * linger) - linger;
			Serial.print(displacement);
			if (displacement < 0) {
				pos = 1;
				Serial.println(",Start");
			}
			else if (displacement > overflow) {
				pos = 1 - overflow;
				Serial.println(",End");
			}
			else {
				pos = 1 - displacement;
				Serial.println(",Scroll");
			}
			//       pos = 1;
		}
		if (start + i == this->selected) {
			disp.drawBox(0, (i)*displayProperties.fontHeight + 1, d, displayProperties.fontHeight);
			disp.setDefaultBackgroundColor();
			//       Serial.print("*");
		}
		//     Serial.println(this->strings[i + start]);
		disp.drawStr(pos, (i + 1)*displayProperties.fontHeight, this->getString(i + start));
		disp.setDefaultForegroundColor();
		Serial.println();
	}
}




void baseMenu_t::unload() {
	if (this->parent != NULL) {
		menu = this->parent;
		menu->load();
	}
}
