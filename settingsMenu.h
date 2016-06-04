#pragma once
#include "baseMenu.h"
class settingsMenu_t : public virtual baseMenu_t {
public:
	settingsMenu_t(menu_t * parent);
	const char * getString(int index);
	void select();
	void load();
};