 #include "Led.h"
  
Led::Led(byte pin) {
  this->pin = pin;
  Init();
}
  
void Led::Init() {
  pinMode(pin, OUTPUT);
}
  
void Led::On() {
  digitalWrite(pin, HIGH);
}

void Led::OnWithSound(bool muted,byte buzzer, int note,int duration) {
  digitalWrite(pin, HIGH);
  
  if(!muted) tone(buzzer,note,duration);
}

void Led::Off() {
  digitalWrite(pin, LOW);            
}