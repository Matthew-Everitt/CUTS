#include "fileMenu.h"
#include "baseMenu.h"
#include "menu.h"
extern menu_t * menu;
extern SdFat SD;

int compareUint16s(const void * a, const void * b) {
	return (*(uint16_t*)a - *(uint16_t*)b);
}


fileMenu_t::fileMenu_t(menu_t *  parent) : baseMenu_t::baseMenu_t(parent) {}

void fileMenu_t::load(void) {
	Serial.println("Loading some files maybe?");

	if (!dir.isOpen()) {
		this->dir.open("/");
		this->path[0] = '/';
		this->path[1] = '\0';
		this->pathEnd = this->path + 1;
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
		this->dir.close();
		menu = this->parent;
		menu->load();
	}
	else { //Otherwise move to the parent dir
		Serial.println("Moving to parent directory");
		this->getParentDir();
		Serial.print("Parent dir is ");
		Serial.println(this->path);
		FatFile nextDir;
		bool r;
		//nextDir.openParent(&(this->dir);
		r = nextDir.open(SD.vwd(), this->path, O_READ);

		Serial.println(r ? "Ok" : "Fail");

		Serial.print("Moving from \"");
		this->dir.getName(this->buffer, this->bufferLen);
		Serial.print(this->buffer);

		Serial.print("\" to \"");
		nextDir.getName(this->buffer, this->bufferLen);
		Serial.print(this->buffer);
		Serial.println("\"");


		uint16_t index = this->dir.dirIndex();

		Serial.print("The directory we are leaving is index ");
		Serial.print(index);
		Serial.print(" in its parent directory (");
		Serial.print(this->buffer);
		Serial.println(")");
		//rebuild table
		this->dir.close();
		this->dir = nextDir;
		this->load();
		for (uint16_t i = 0; i < this->nEntries; i++) {
			Serial.print(i); Serial.print(",0x"); Serial.print((int)(this->fileIndicies + i), HEX); Serial.print(","); Serial.println(this->fileIndicies[i]);
		}

		uint16_t *pos = (uint16_t*)bsearch(&index, this->fileIndicies, this->nEntries, sizeof(uint16_t), compareUint16s);
		Serial.print("bsearch puts it at 0x");
		Serial.println((int)pos, HEX);

		this->selected = pos - this->fileIndicies;
		Serial.print("Which puts the selected as ");
		Serial.println(this->selected);

		
	}
}

const char * fileMenu_t::getString(int index) {

	File file;
	file.open(&(this->dir), this->fileIndicies[index], O_READ);

	file.getName(this->buffer, this->bufferLen);
	if (file.isDir()) {
		size_t len = strlen(buffer);
		buffer[len] = '/';
		buffer[len + 1] = '\0';
	}
	file.close();

	return this->buffer;
}

void fileMenu_t::select() {
	Serial.print("File ");
	Serial.print(this->selected);
	Serial.print(" selected:  ");
	File file;
	file.open(&(this->dir), this->fileIndicies[this->selected], O_READ);

	if (file.isDir()) {
		Serial.print("Enter dir ");

		file.getName(this->buffer, this->bufferLen);
		Serial.println(this->buffer);

		Serial.print("Path changing from \"");
		Serial.print(this->path);
		Serial.print("\" to \"");

		size_t toCopy = min((this->pathLen - (this->pathEnd - this->path)), strlen(buffer));
		strncpy(this->pathEnd, buffer, toCopy);


		this->pathEnd += toCopy;
		*(this->pathEnd) = '/';
		this->pathEnd++;
		*(this->pathEnd) = '\0';


		Serial.print(this->path);
		Serial.println("\"");

		FatFile nextDir;

		nextDir.open(&(this->dir), this->fileIndicies[this->selected], O_READ);

		this->dir.close();

		this->dir = nextDir;
		this->selected = 0;
		this->load();
	}
	else {
		Serial.println("Open file");
	}
	file.close();
	

}

void fileMenu_t::getParentDir(void) {
	this->pathEnd -= 2;
	while (*(this->pathEnd) != '/') {

		//Cover our asses, if we hit the start of the buffer just make it look right and return
		if (this->pathEnd <= this->path) {
			*(this->path) = '/';
			*(this->path + 1) = '\0';
			this->pathEnd = this->path + 1;
			return;
		}

		this->pathEnd--;
	}
	this->pathEnd++; //Include the '/'
	*(this->pathEnd) = '\0';
	Serial.print("Path changing to\"");
	Serial.print(this->path);
	Serial.println("\"");
	return;
}

