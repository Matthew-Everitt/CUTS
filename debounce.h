#define debounceISR(BUTTON) void BUTTON##CALLBACK( void );\
void BUTTON##ISR( ){\
  static unsigned long lastPress = 0;\
  unsigned long now=millis();\
  unsigned long threshold = 250;\
  if ( now - lastPress > threshold ){\
    BUTTON##CALLBACK();\
  }\
  lastPress = now;\
}\
void BUTTON##CALLBACK( void )

#define BUTTON( PIN ) pinMode(PIN, INPUT_PULLUP);attachInterrupt( PIN, PIN##ISR, RISING )