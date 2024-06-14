 #include "Button.h"

Button::Button(byte pin) {
  this->pin = pin;
  lastReading = LOW;
  init();
}
Button::void init() {
  pinMode(pin, INPUT);
  update();
}

Button::void update() {
  byte newReading = digitalRead(pin);
  
  if (newReading != lastReading) {
    lastDebounceTime = millis();
  }
  if (millis() - lastDebounceTime > debounceDelay) {
    state = newReading;
  }
  lastReading = newReading;
}

Button::byte getState() {
  update();
  return state;
}

Button::bool isPressed() {
  return (getState() == HIGH);
}
