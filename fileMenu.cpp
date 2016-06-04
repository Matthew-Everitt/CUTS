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
	this->lastOpened = 0;
	FatFile file;

	char buffer[128]; //For debug

	//For now just rely on there being enough space in the index array. Obviously fix this, at least to the level of stopping to avoid overflow
	while (file.openNext(&(this->dir), O_READ)) {
		//Debug logging
		Serial.print("Entry ");
		Serial.print(this->nEntries);
		Serial.print(" ( \"");
		file.getName(buffer, 128);
		Serial.print(buffer);
		Serial.print("\" ) at index ");
		Serial.println(file.dirIndex());

		//Actual things
		this->fileIndicies[this->nEntries] = file.dirIndex();
		this->nEntries++;
		file.close();
	}
}

const char * fileMenu_t::getString(int index) {

	File file;

	static char buffer[128];
	//TODO: subclass FatFile and add an openPrevious so we can work backwards from where we are, never giving up out position (unless 0 is closer than we currently are I suppose)
	//That way we can calculate how many we need to move, and the direction, apply the correct # of openNext / openPrevious commands, and finally read the filename we want

	//Other options :  
	//		*	have a set # of cached results, check there first - might be a nice library for this?
	//		*	Pagify the results using a cache of strings, only traverse the file to update that cache when a new page is requested
	//		*	Have a fixed size cache of strings, ignore anything outside that - who needs more than N files per dir?
	//		*	Build a lookup table of requested index vs. FAT index. Should be 16 bits per entry, need to allow a decent # - big table. Not unreasonable if we limit to lets say 100 files per dir.

	//		*	Lookup table as above, but pageified / cached like the other ideas, but using less RAM than strings. Seems reasonably easy to have an array of N, from m to m+N, and update (shift) by N/2 when we reach the edge. That way we always have a reasonable margin to move back and forth without having to recalculate. To make life easier we may be able to use get/setPos on the dir file to record where the transitions are, which can be calculated when we find the # of files at load.

	file.open(&dir, this->fileIndicies[index], O_READ);
	file.getName(buffer, 128);
	file.close();

	this->lastOpened = index;
	return buffer;
}

void fileMenu_t::select() {
	Serial.println("Changing menu (file)");
	//menu = this->menus[this->selected];
	//menu->load();
}

