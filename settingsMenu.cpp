#include "settingsMenu.h"
#include "common.h"

extern U8GLIB_PCD8544 disp;
extern settings_t settings;
extern displayProperties_t displayProperties;

settingsMenu_t::settingsMenu_t(menu_t *  parent) : baseMenu_t::baseMenu_t(parent) {}

const char * settingsMenu_t::getString(int index) {

	const char * name;
	const char * state;

	switch (index) {
	case OVERWRITE:
		name = "Overwrite";
		state = this->boolToString(settings.overwrite);
		break;
	case DETOKENISE:
		name = "Detokenise";
		state = this->boolToString(settings.detokenise);
		break;
	case TOKENISE:
		name = "Tokenise";
		state = this->boolToString(settings.tokenise);
		break;
	case SEND_NAMED:
		name = "Send named";
		state = this->boolToString(settings.sendNamed);
		break;
	default:
		name = "Error";
		state = "Error";
	}
	uint8_t width = displayProperties.nCols - strlen(name);
	snprintf(this->buffer, this->bufferLen, "%s%*s", name, width, state);
	return this->buffer;
}

void settingsMenu_t::select() {
	switch (this->selected) {
	case OVERWRITE:
		settings.overwrite = !settings.overwrite;
		break;
	case DETOKENISE:
		settings.detokenise = !settings.detokenise;
		break;
	case TOKENISE:
		settings.tokenise = !settings.tokenise;
		break;
	case SEND_NAMED:
		settings.sendNamed = !settings.sendNamed;
		break;
	default:
		Serial.print(this->selected); Serial.print(" is not a known setting @ line "); Serial.print(__LINE__); Serial.print(" of "); Serial.print(__FILE__);
	}
}

void settingsMenu_t::load() {
	this->nEntries = N_SETTINGS;
}

const char * settingsMenu_t::boolToString(bool b) {
	if (b) {
		return "true";
	} else {
		return "false";
	}
}

