#include "fileSendMenu.h"

fileSendMenu_t::fileSendMenu_t(menu_t *  parent) : baseMenu_t::baseMenu_t(parent) {}

const char * fileSendMenu_t::getString(int index) {
	return "FILES!";
}

void fileSendMenu_t::select() {}

void fileSendMenu_t::load() {}
