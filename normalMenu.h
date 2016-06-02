#pragma once

#include "baseMenu.h"
#include "common.h"

class normalMenu_t : public virtual baseMenu_t {
	public:
		normalMenu_t(menu_t * parent);
		char * getString(int index);
		void select();
		void load();
};
