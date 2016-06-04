#include "fileMenu.h"
#include "baseMenu.h"
#include "menu.h"
extern menu_t * menu;
extern SdFat SD;

int compareInts(const void * a, const void * b) {
	return (*(int*)a - *(int*)b);
}


fileMenu_t::fileMenu_t(menu_t *  parent) : baseMenu_t::baseMenu_t(parent) {}

void fileMenu_t::load(void) {
	Serial.println("Loading some files maybe?");

	if (!dir.isOpen()) {
		this->dir.open("/");
	}


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


void fileMenu_t::unload(void) {
	if (this->dir.isRoot()) { //If we've run out of directories load the previous menu
		Serial.println("Moving back to menus");
		menu = this->parent;
		menu->load();
	}
	else { //Otherwise move to the parent dir
		Serial.println("Moving to parent directory");

		FatFile nextDir;
		bool r;
		//nextDir.openParent(&(this->dir);
		r = nextDir.open(&(this->dir), "..", O_READ);

		Serial.println(r ? "Ok" : "Fail");

		Serial.print("Moving from \"");
		this->dir.getName(this->buffer, this->bufferLen);
		Serial.print(this->buffer);

		Serial.print("\" to \"");
		nextDir.getName(this->buffer, this->bufferLen);
		Serial.print(this->buffer);
		Serial.println("\"");


		uint16_t index = this->dir.dirIndex();
		uint16_t *pos = (uint16_t*)bsearch(&index, this->fileIndicies, this->nEntries, sizeof(uint16_t), compareInts);

		this->selected = this->fileIndicies - pos;

		this->dir.close();
		this->dir = nextDir;

		this->load();
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
	Serial.print("File ");
	Serial.print(this->selected);
	Serial.print(" selected:  ");
	File file;
	file.open(&(this->dir), this->fileIndicies[this->selected], O_READ);

	file.getName(this->buffer, this->bufferLen);
	Serial.println(this->buffer);

	Serial.print("Path changing from \"");
	Serial.print(this->path);
	Serial.print("\" to \"");

	size_t toCopy = min( (this->pathLen - (this->pathEnd - this->path)) , strlen(buffer) );
	strncpy(this->pathEnd, buffer, toCopy);


	this->pathEnd += toCopy;
	*(this->pathEnd) = '/';
	this->pathEnd++;
	*(this->pathEnd) = '\0';
	

	Serial.print(this->path);
	Serial.println("\"");

	if (file.isDir()) {
		Serial.println("Enter dir");

		FatFile nextDir;

		nextDir.open(&(this->dir), this->fileIndicies[this->selected], O_READ);

		this->dir.close();

		this->dir = nextDir;

		this->load();
	}
	else {
		Serial.println("Open file");
	}
	file.close();
	this->selected = 0;

}

