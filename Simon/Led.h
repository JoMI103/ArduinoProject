#ifndef Jomi_LED_H
#define Jomi_LED_H

#include <Arduino.h>
class Led {
  
  private:
    byte pin;
    
  public:
    Led(byte pin);
    void Init();
    void On();
    void OnWithSound(bool,byte,int,int);
    void Off();
};
#endif
