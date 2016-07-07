#include "fileSendMenu.h"
#include "CommunicationSystem.h"
#include "fileMenu.h"
extern menu_t * menu;
//extern ComputerInterface * computerInterface;
extern CommunicationSystem communicationSystem; //We sadly need to have quite a bit global

fileSendMenu_t::fileSendMenu_t(menu_t *  parent) : baseMenu_t::baseMenu_t(parent) {}

const char * fileSendMenu_t::getString(int index) {
	switch (index) {
	case SEND_DEFAULT:
		return "Send default";
	case SEND_NAMED_NOT_TOKENISED:
		return "Named/Untokenised";
	case SEND_NAMED_TOKENISED:
		return "Named/Tokenised";
	case SEND_UNAMED_NOT_TOKENISED:
		return "Unamed/Untokenised";
	case SEND_UNAMED_TOKENISED:
		return "Unamed/Tokenised";
	}
}

void fileSendMenu_t::select() {
	Serial.print("Sending file ");

	FatFile file;

	file.open(&this->parent->dir, this->parent->getString(this->parent->selected), O_READ);
	file.printName();
	Serial.println();

	communicationSystem.computerInterface->sendByte(0x42);
	communicationSystem.computerInterface->endTransmission();
	//delay(500);
	menu = this->parent;
	menu->load();
}

void fileSendMenu_t::load() {
	this->nEntries = N_SEND_OPTIONS;
}

