#ifndef Jomi_LED_H
#define Jomi_LED_H

#include <Arduino.h>
class Led {
  
  private:
    byte pin;
    
  public:
    Led(byte pin);
    void init();
    void on();
    void on(byte,int,int);
    void off();
};
#endif
