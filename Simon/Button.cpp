 #include "Button.h"

Button::Button(byte pin) {
  this->pin = pin;
  lastReading = LOW;
  init();
}
void Button::init() {
  pinMode(pin, INPUT_PULLUP);
  //pinMode(pin, INPUT);
  update();
}

void Button::update() {
  byte newReading = digitalRead(pin);
  
  if (newReading != lastReading) {
    lastDebounceTime = millis();
  }
  if (millis() - lastDebounceTime > debounceDelay) {
    state = newReading;
  }
  lastReading = newReading;
}

byte Button::getState() {
  update();
  return state;
}

bool Button::isPressed() {
  

  return (digitalRead(pin) == LOW);

  //return (getState() == HIGH);
}
