#pragma once

#include "menu.h"
class stdMenu_t : public virtual menu_t{
  public:
    stdMenu_t ( menu_t *  parent );
    void setMenus( menu_t ** menus );
    void setNEntries( int n );
    void setStrings( char ** strings);
    void changeSelected( int s );
    void draw( );
    void select( );
    void load  ( );
    void unload( );
  private:
    int nEntries;
    char ** strings;
    menu_t ** menus;
    int selected = 0;

};