#pragma once

#include "baseMenu.h"
#include "common.h"

class fileMenu_t : public virtual baseMenu_t{
  public:
    fileMenu_t ( menu_t * parent );
    void load  ( );
	char * getString(int index);
    void select( );
	FatFile dir;
	uint16_t lastOpened = 0;
};