#include "U8glib.h"
#include <Encoder.h>
#include "debounce.h"

#include "common.h"
#include "nokiaDisplay.h"
#include "menu.h"
#include "stdMenu.h"
#include "fileMenu.h"

#include "systemState.h"

displayProperties_t displayProperties;
//I think DC & RST may be swapped
U8GLIB_PCD8544 disp( dispCLK ,  dispMOSI ,  dispSCE,  dispRST,  dispDC);
Encoder rotaryEncoder(encPin1,encPin2);

systemState_t systemState;

//current menu
menu_t *  menu;

#include "menuDefinitions.h"
debounceISR(encButt){
  Serial.println("BEEP");
  menu->select( );
}

debounceISR(playBut){
  Serial.println("Play/Pause");
  if ( systemState.driveState == play ){
    systemState.driveState = pause;
  }else{
    systemState.driveState=play;
  }
}


debounceISR(backBut){
  Serial.println("Back");
  menu->unload();
}


void setup() {  
  Serial.begin(9600);
  setupMenus();
  systemState.freeSpace = 39784563;
  pinMode(13,OUTPUT);

  BUTTON( encButt );
  BUTTON( playBut );
  BUTTON( backBut );
  
  menu = &topMenu;

  disp.setFont(u8g_font_04b_03r);
//   disp.setFont(u8g_font_6x10);
//   disp.setFont(u8g_font_6x13);
  displayProperties.fontHeight = disp.getFontAscent()-disp.getFontDescent();
  displayProperties.infoBarHeight = displayProperties.fontHeight;
  displayProperties.nRows = (disp.getHeight()-displayProperties.infoBarHeight)%displayProperties.fontHeight;
  displayProperties.before = displayProperties.nRows/2;
  displayProperties.after  = displayProperties.nRows - displayProperties.before;
  displayProperties.before--;
  
  Serial.print("Before :  ");Serial.println(displayProperties.before);
  Serial.print("After  :  ");Serial.println(displayProperties.after);
  Serial.print("nRows  :  ");Serial.println(displayProperties.nRows);
  disp.setColorIndex(1); // Instructs the display to draw with a pixel on. 

}

void updateEncoder(){
    static int previousP = 0;
  int value = rotaryEncoder.read();
  if ( value%4 == 0 ){
    int change = value/4 - previousP;
    //The encoder give 4 ticks per detent
    if (change != 0) {
      menu->changeSelected( change );
      previousP+=change;
    }
  }
}

void updateDisplay(){
    disp.firstPage();
  do {
    drawInfoBar();
//     Serial.print("\tMenu is 0x");Serial.println((int)menu,HEX);
    menu->draw();
//     Serial.println(")");
  } while( disp.nextPage() );
}

void loop() {  

  updateEncoder();
  updateDisplay();
  delay(100);




  systemState.percentage++;
  systemState.percentage=systemState.percentage%101;
}  

int bytesToString ( float bytes, char * buffer, int n){
  char * prefixes =" kMGT";
  int count = 0;
  while ( bytes >= 1.0 ){
    bytes/=1024.0;
    count++;
  }
  bytes*=1024.0;
  count--;
  return snprintf(buffer,n,"%6.2f%cB",bytes,prefixes[count]);
}


void drawInfoBar( void ){

  uint8_t top = disp.getHeight() - displayProperties.infoBarHeight;

  disp.setFontPosBaseline( );
  
  //Progress 
  char buf[10];
  char *space;
  if ( systemState.percentage==100 ){
    space="";
  }else if ( systemState.percentage<10 ){
    space="  ";
  }else{
    space=" ";
  }
  snprintf( buf, 10, "%s%i%%" ,space, systemState.percentage);
  disp.drawStr( displayProperties.infoBarHeight + 1, disp.getHeight(), buf);    

  //Line seperating the infoBar
  disp.drawLine( 0, top, disp.getWidth(), top);

  //Drive state icon
  if ( systemState.driveState == play ) {
    disp.drawTriangle(0, disp.getHeight() , 0, disp.getHeight() - displayProperties.infoBarHeight + 1,  (displayProperties.infoBarHeight),  disp.getHeight() - displayProperties.infoBarHeight/2 );
  } else if( systemState.driveState == pause ){
    disp.drawBox(0, disp.getHeight() - displayProperties.infoBarHeight + 2, (displayProperties.infoBarHeight-1)/2, displayProperties.infoBarHeight );
    disp.drawBox((displayProperties.infoBarHeight-1)/2 + 1, disp.getHeight() - displayProperties.infoBarHeight + 2, (displayProperties.infoBarHeight-1)/2, displayProperties.infoBarHeight );
  }else if( systemState.driveState == stop ){
    disp.drawBox(0, disp.getHeight() - displayProperties.infoBarHeight + 2, (displayProperties.infoBarHeight), displayProperties.infoBarHeight );
  }
  
  byte n = bytesToString( systemState.freeSpace, buf, 10);
  disp.drawStr( disp.getWidth() - disp.getStrWidth( buf ), disp.getHeight(), buf); 
  
  
}
