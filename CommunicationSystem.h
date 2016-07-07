#pragma once
#include "pinout.h"
#include "computerInterface.h"
#include "storageInterface.h"
#include "namedFile.h"
#include "300BaudCUTS.h"

class CommunicationSystem {
public:
	void update();
	bool sendFile(const String path);
	CommunicationSystem();

	ComputerInterface * computerInterface;
	storageInterface * storage;

private:
	static threeHundredBaudCUTS thb;
	namedFile named;


	


};