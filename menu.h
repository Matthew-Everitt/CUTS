#pragma once

class menu_t{
  public:
    virtual void draw( void ) = 0;             // Update the display.
    virtual void select(void) = 0;             // Encoder button clicked.
    virtual void changeSelected(int) = 0;      // Encoder turned by (int) clicks.
    virtual void load( void ) = 0;             // Called when this menu is loaded.
	virtual void unload(void) = 0;             // Called when back button is presssed.
    menu_t * parent;                           // We need to know where to go when back is pressed
};

