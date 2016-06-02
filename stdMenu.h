#pragma once

#include "menu.h"
class baseMenu_t : public virtual menu_t{
  public:
    baseMenu_t ( menu_t *  parent );
    void setMenus( menu_t ** menus );
    void setNEntries( int n );
    void setStrings( char ** strings );
    void changeSelected( int s );
    void draw( );
    void select( );
    void load  ( );
    void unload( );

	virtual char * getString(int i) = 0;
	char ** strings;

  private:
    int nEntries;
    menu_t ** menus;
    int selected = 0;

};