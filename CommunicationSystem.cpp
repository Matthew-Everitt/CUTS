#include "CommunicationSystem.h"

threeHundredBaudCUTS CommunicationSystem::thb(inputPin, outputPin);

void CommunicationSystem::update() {
	if (computerInterface->newByteAvaliable) {
#ifdef rawBytes
		Serial.print("rawByte,");
		Serial.println(computerInterface->data, HEX);
#endif
		storage->RX(computerInterface->data);
		computerInterface->newByteAvaliable = false;
	}
}

bool CommunicationSystem::sendFile(const String path) { //Take file ref as parammter rather than path?
	//The computer interface provides a sendByte function (which should go via the comms format (blocks etc), so we just need to keep sending bytes to that while we can.
	
	//For each byte in file
	//this->computerInterface->sendByte();
	
	return false;
}

CommunicationSystem::CommunicationSystem() {
	this->computerInterface = &this->thb;
	this->storage = &this->named;

}
