#include "fileSendMenu.h"

extern menu_t * menu;

fileSendMenu_t::fileSendMenu_t(menu_t *  parent) : baseMenu_t::baseMenu_t(parent) {}

const char * fileSendMenu_t::getString(int index) {
	switch (index) {
	case SEND_DEFAULT:
		return "Send default";
	case SEND_NAMED_NOT_TOKENISED:
		return "Named/Untokenised";
	case SEND_NAMED_TOKENISED:
		return "Named/Tokenised";
	case SEND_UNAMED_NOT_TOKENISED:
		return "Unamed/Untokenised";
	case SEND_UNAMED_TOKENISED:
		return "Unamed/Tokenised";
	}
}

void fileSendMenu_t::select() {
	menu = this->parent;
	menu->load();
}

void fileSendMenu_t::load() {
	this->nEntries = N_SEND_OPTIONS;
}

