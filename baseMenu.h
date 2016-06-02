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
    void unload( );

	virtual void select() = 0;
	virtual char * getString(int i) = 0;

	char ** strings;
	menu_t ** menus;

    int nEntries;
    
    int selected = 0;

  private:
};