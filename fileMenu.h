#pragma once

#include "baseMenu.h"
#include "common.h"

int compareUint16s(const void * a, const void * b);

class fileMenu_t : public virtual baseMenu_t {
public:
	fileMenu_t(menu_t * parent);
	void load();
	void unload();
	const char * getString(int index);
	void select();

private:
	static const int  N_FILE_INDICIES_TO_CACHE = 100;
	uint16_t fileIndicies[N_FILE_INDICIES_TO_CACHE];

	static const int pathLen = 128;
	char path[pathLen+5];
	char * pathEnd = path;

	static const int bufferLen = 128;
	char buffer[bufferLen];

	void getParentDir(void);

	FatFile dir;
};