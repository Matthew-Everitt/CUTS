#include "fileMenu.h"
#include "baseMenu.h"
#include "menu.h"
extern menu_t * menu;
extern SdFat SD;



fileMenu_t::fileMenu_t(menu_t *  parent) : baseMenu_t::baseMenu_t(parent) {}

void fileMenu_t::load(void) {
	Serial.println("Loading some files maybe?");
	const char * path = "/test/";
	this->dir.open(path);
	this->dir.rewind();
	this->nEntries = 0;
	FatFile file;


	//For now just rely on there being enough space in the index array. Obviously fix this, at least to the level of stopping to avoid overflow
	while (file.openNext(&(this->dir), O_READ)) {
		////Debug logging
		//Serial.print("Entry ");
		//Serial.print(this->nEntries);
		//Serial.print(" ( \"");
		//file.getName(this->buffer, this->bufferLen);
		//Serial.print(this->buffer);
		//Serial.print("\" ) at index ");
		//Serial.println(file.dirIndex());

		//Actual things
		this->fileIndicies[this->nEntries] = file.dirIndex();
		this->nEntries++;
		file.close();

		if (this->nEntries >= this->N_FILE_INDICIES_TO_CACHE) {
			break;
		}
	}
}

const char * fileMenu_t::getString(int index) {

	File file;
	file.open(&(this->dir), this->fileIndicies[index], O_READ);

	file.getName(this->buffer, this->bufferLen);
	file.close();

	return this->buffer;
}

void fileMenu_t::select() {
	Serial.println("File selected");
	File file;
	file.open(&(this->dir), this->fileIndicies[this->selected], O_READ);
	if (file.isDir()) {
		Serial.println("Enter dir");
	} else {
		Serial.println("Open file");
	}
}

