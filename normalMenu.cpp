#include "normalMenu.h"
#include "stdMenu.h"

normalMenu_t::normalMenu_t(menu_t *  parent) : baseMenu_t::baseMenu_t(parent) {}


char * normalMenu_t::getString(int index) {
	return this->strings[index];
}