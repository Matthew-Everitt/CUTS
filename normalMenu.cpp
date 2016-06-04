#include "normalMenu.h"
#include "baseMenu.h"

extern menu_t * menu;

normalMenu_t::normalMenu_t(menu_t *  parent) : baseMenu_t::baseMenu_t(parent) {}


char * normalMenu_t::getString(int index) {
	return this->strings[index];
}
void normalMenu_t::setMenus(menu_t ** menus) {
	this->menus = menus;
}

void normalMenu_t::setStrings(char ** strings) {
	this->strings = strings;
}
void normalMenu_t::select() {
	Serial.println("Changing menu (normal)");
	menu = this->menus[selected];
	menu->load();
}

void normalMenu_t::load() {}