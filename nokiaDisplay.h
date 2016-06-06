#pragma once
#include "debounce.h"
#include "pinout.h"
class display_t : public U8GLIB_PCD8544 {
public:
	display_t(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE);

	void update();

	void updateEncoder();
	void updateMainDisplay();
	void drawInfoBar();

	debounceISR_proto(encButt);
	debounceISR_proto(playBut);
	debounceISR_proto(backBut);


};

extern display_t display;