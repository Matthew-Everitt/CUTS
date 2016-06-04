#pragma once

#include "baseMenu.h"
#include "common.h"

class fileMenu_t : public virtual baseMenu_t {
public:
	fileMenu_t(menu_t * parent);
	void load();
	const char * getString(int index);
	void select();

private:
	static const int  N_FILE_INDICIES_TO_CACHE = 100;
	uint16_t fileIndicies[N_FILE_INDICIES_TO_CACHE];

	static const int bufferLen = 128;
	char buffer[bufferLen];

	FatFile dir;
};