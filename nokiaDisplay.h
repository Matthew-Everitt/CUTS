#pragma once
#include "debounce.h"
#include "pinout.h"
class display_t {
public:

	//static U8GLIB_PCD8544 disp;

	display_t();
	//void setup();
	void update();

	void updateEncoder();
	void updateMainDisplay();
	void drawInfoBar();

	debounceISR_proto(encButt);
	debounceISR_proto(playBut);
	debounceISR_proto(backBut);
};