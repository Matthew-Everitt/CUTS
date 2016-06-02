#include "fileMenu.h"
#include "stdMenu.h"

fileMenu_t::fileMenu_t ( menu_t *  parent ): stdMenu_t::stdMenu_t(parent) {}

void fileMenu_t::load ( void ){
  Serial.println("Loading some files maybe?");
}
