#pragma once
#include "debounce.h"
class display_t {
public:
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