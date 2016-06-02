#pragma once

#include "stdMenu.h"
#include "common.h"

class fileMenu_t : public virtual baseMenu_t{
  public:
    fileMenu_t ( menu_t * parent );
    void load  ( );
	char * getString(int index);
//     void select( );
};