 #include "Led.h"
  
Led::Led(byte pin) {
  this->pin = pin;
  init();
}
  
void Led::init() {
  pinMode(pin, OUTPUT);
}
  
void Led::on() {
  digitalWrite(pin, HIGH);
}

void Led::on(byte buzzer, int note,int duration) {
  tone(buzzer,note,duration);
  digitalWrite(pin, HIGH);
}

void Led::off() {
  digitalWrite(pin, LOW);            
}