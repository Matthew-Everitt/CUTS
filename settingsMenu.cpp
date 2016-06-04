#include "settingsMenu.h"
#include "common.h"

extern U8GLIB_PCD8544 disp;
extern settings_t settings;

settingsMenu_t::settingsMenu_t(menu_t *  parent) : baseMenu_t::baseMenu_t(parent) {}

const char * settingsMenu_t::getString(int index){

	const char * name;
	const char * state;

	switch (index) {
	case 0:
		name = "Overwrite";
		state = this->boolToString(settings.overwrite);
		break;
	case 1:
		name = "Detokenise";
		state = this->boolToString(settings.detokenise);
		break;
	default:
		name = "Error";
		state = "Error";
	}
	uint8_t width = 0;
	snprintf(this->buffer, this->bufferLen, "%s%*s",name,width,state );
	return this->buffer;
}

void settingsMenu_t::select(){
}

void settingsMenu_t::load(){
}

const char * settingsMenu_t::boolToString(bool b)
{
	if (b) {
		return "true";
	} else {
		return "false";
	}
}

