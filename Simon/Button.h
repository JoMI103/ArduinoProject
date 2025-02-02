#ifndef Jomi_Button_H
#define Jomi_Button_H
#include "Arduino.h"

class Button {

  private:
    byte pin;
    byte state;
    byte lastReading;
  
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 20;
  
  public:
    Button(byte pin); 
    void init();
    void update();
    byte getState();
    bool isPressed();
    
};

#endif