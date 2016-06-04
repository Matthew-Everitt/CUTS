#include "settingsMenu.h"

settingsMenu_t::settingsMenu_t(menu_t *  parent) : baseMenu_t::baseMenu_t(parent) {}

const char * settingsMenu_t::getString(int index){
	return "Hello!";
}

void settingsMenu_t::select(){
}

void settingsMenu_t::load(){
}

