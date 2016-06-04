#pragma once
#include "baseMenu.h"

class fileSendMenu_t : public virtual baseMenu_t {
public:
	fileSendMenu_t(menu_t * parent);
	const char * getString(int index);
	void select();
	void load();
private:
	enum {
		SEND_DEFAULT,
		SEND_NAMED_TOKENISED,
		SEND_UNAMED_TOKENISED,
		SEND_NAMED_NOT_TOKENISED,
		SEND_UNAMED_NOT_TOKENISED,
	};

	static const int bufferLen = 64; //Display is only 64 pixels wide, so this is pesimistic, but we have RAM, and this isn't the kind of problem that matters unless we need to move to new hardware, in which case we need to think long and hard about these kind of things
	char buffer[bufferLen];
};
