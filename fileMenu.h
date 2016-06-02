#pragma once

#include "stdMenu.h"
#include "common.h"

class fileMenu_t : public virtual stdMenu_t{
  public:
    fileMenu_t ( menu_t * parent );
    void load  ( );
//     void select( );
};