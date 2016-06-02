#include "fileMenu.h"
#include "stdMenu.h"

fileMenu_t::fileMenu_t ( menu_t *  parent ): baseMenu_t::baseMenu_t(parent) {}

void fileMenu_t::load ( void ){
  Serial.println("Loading some files maybe?");
}

char * fileMenu_t::getString(int index) {
	return "I'm a kitty!";
}