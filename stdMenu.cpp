#include "stdMenu.h"
#include "menu.h"
#include "common.h"
#include "nokiaDisplay.h"
extern menu_t * menu;

stdMenu_t::stdMenu_t ( menu_t *  parent ){
  this -> parent = parent;
}

void stdMenu_t::setMenus( menu_t ** menus ){
  this->menus = menus;
}

void stdMenu_t::setNEntries( int n ){
  this -> nEntries = n;
}

void stdMenu_t::setStrings( char ** strings){
  this -> strings = strings;
}
void stdMenu_t::changeSelected( int s ){
  Serial.print( "\tGoing from "  );
  Serial.print( this->selected );
  Serial.print( " to "           );
  
  this->selected += s;
  
  Serial.println(  this->selected );
  
  if (this->selected < 0) this->selected += this->nEntries;
  Serial.print( "\tWrapped to " );
  this->selected = (this->selected)%(this->nEntries);
  Serial.println( this->selected);
}

void stdMenu_t::draw( ){
//   Serial.print("Selected is ");
//   Serial.println(this->selected);
  int count = min( displayProperties.nRows - 1, this->nEntries ) ;
  int start =(int)(this->selected - displayProperties.before); //Have some before the selected.
  int correction = min(0,(int)(this->nEntries - (this->selected + displayProperties.after)) ); //Step back to get the 
  
//   Serial.print( count );
//   Serial.println( " lines to print.");
//   Serial.print("Going from ");Serial.print(start);Serial.print(" + "); Serial.println(-correction);
  start += correction;
  start = max( 0, start );
  int linger=15; // Time to wait at the start / end of a string, in pixels. About 15 is ok?
  int scrollStepTimeInMilliseconds=150;
  unsigned long scroll = millis();
  for ( byte i = 0 ; i < count ; i++ ){
    int d = disp.getStrWidth(this->strings[i + start]) + 1;
    int overflow = d-disp.getWidth();
    int pos = 1;
   
    if (overflow > 0){
      //Serial.print(overflow);
      //Serial.print(",");
      //Serial.print(scroll);
      //Serial.print(",");
      int displacement = (scroll/scrollStepTimeInMilliseconds)%(overflow+2*linger) - linger;
      //Serial.print(displacement);
      if ( displacement < 0 ){
	pos = 1;
	//Serial.println(",Start");
      } else if ( displacement > overflow ){
	pos = 1 - overflow;
	//Serial.println(",End");
      } else {
	pos = 1 - displacement;
	//Serial.println(",Scroll");
      }
//       pos = 1;
    }
    if ( start + i == this->selected ) {
      disp.drawBox(0, (i)*displayProperties.fontHeight + 1, d, displayProperties.fontHeight);
      disp.setDefaultBackgroundColor();
//       Serial.print("*");
    }
//     Serial.println(this->strings[i + start]);
    disp.drawStr( pos, (i+1)*displayProperties.fontHeight, this->strings[i + start]);
    disp.setDefaultForegroundColor();
  }
}
void stdMenu_t::select( ){
  //Serial.println("Changing menu");
  menu = this->menus[selected];
  menu->load();
}
void stdMenu_t::load  ( ){
}
void stdMenu_t::unload( ){
  if ( this->parent != NULL ){
    menu = this->parent;
    menu->load();
  }
};
