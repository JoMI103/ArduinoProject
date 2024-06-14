#include "Led.h"
#include "Button.h"


Button sequenceButtons[6] = {A0,A1,A2,A3,4,2};
Led sequenceLeds[6] = {3,5,6,9,10,11};

int const sequenceOptions = 6;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  ReadSequenceButtons();
  delay(10);
}


void ReadSequenceButtons()
{
  int read = 0;
	
  for(int i = 0; i < sequenceOptions; i++)
  {
    if(sequenceButtons[i].isPressed()){
      sequenceLeds[i].on();
    }else{
      sequenceLeds[i].off();
    }
  } 
}

/*


class Led {
  
  private:
  	byte pin;
  
  public: 
    Led(byte pin) {
      this->pin = pin;
      init();
    }
  
    void init() {
      pinMode(pin, OUTPUT);
    }
  
    void on() {
      digitalWrite(pin, HIGH);
    }
    void off() {
      digitalWrite(pin, LOW);
            
	}
};


class Button {
  
  private:
    byte pin;
    byte state;
    byte lastReading;
  
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 20;
  
  public:
    Button(byte pin) {
      this->pin = pin;
      lastReading = LOW;
      init();
    }
    void init() {
      pinMode(pin, INPUT);
      update();
    }
  
    void update() {
      byte newReading = digitalRead(pin);
      state = newReading;
      return;
      if (newReading != lastReading) {
        lastDebounceTime = millis();
      }
      if (millis() - lastDebounceTime > debounceDelay) {
        state = newReading;
      }
      lastReading = newReading;
    }
  
    byte getState() {
      update();
      return state;
    }
  
    bool isPressed() {
      return (getState() == HIGH);
    }
};


#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd_1(0);


//Green,Yellow,Orange,Blue,Red,White
Button sequenceButtons[6] = {A0,A1,A2,A3,4,2};
Led sequenceLeds[6] = {3,5,6,9,10,11};

int sequenceOptions;

byte lastPin = 0;


byte sequence[100] {0};
byte maxSequenceLength;
byte sequenceCurrentLength = 0;



void setup()
{
  randomSeed(analogRead(5));
  
  sequenceOptions = sizeof(sequenceLeds) / sizeof(Led);
  maxSequenceLength =  sizeof(sequence) / sizeof(Led);
  
  Serial.begin(9600);
  lcd_1.begin(16, 2);
  lcd_1.setCursor(0, 1);
  lcd_1.print("hello worloooooooood");
  
  AddSequenceDifficulty();
}


void loop()
{
  //ReadSequenceButtons();
  
  ShowCurrentSequence();
  
  if(CheckSequence() == 1)
  {
    AddSequenceDifficulty();
    Serial.println("Nova dificuldade");
  }
  else
  {
    Serial.println("Reset");
	sequenceCurrentLength = 0;
  }
  
  delay(500);
  
}


void AddSequenceDifficulty()
{
  sequence[sequenceCurrentLength] = random(0,sequenceOptions);
  sequenceCurrentLength++;
  if(sequenceCurrentLength >= maxSequenceLength){
    //Winwin
    sequenceCurrentLength = 0;
  }
	  
}

void ShowCurrentSequence()
{
  for(int e = 0; e < sequenceCurrentLength; e++){
    sequenceLeds[sequence[e]].on();
    delay(200);
    sequenceLeds[sequence[e]].off();
    delay(100);
  }
  
}

int CheckSequence()
{
  byte currentOption = 0;
  int e = 0;
  while(e < sequenceCurrentLength)
  {
    for(int i = 0; i < sequenceOptions; i++)
      {
        if(sequenceButtons[i].isPressed()){
          Serial.println("entrou");
          if(i == sequence[e])
          {
            e++;
            Serial.println("ok");
            delay(100);
          }
          else
          {
            Serial.println("Errou");
            return 0;
          }
        }
      }
    
    
    delay(10);
  }
  for(int e = 0; e < sequenceCurrentLength; e++)
  {
      
       	
    }
  
	return 1;
}



void ReadSequenceButtons()
{
  int read = 0;
	
  for(int i = 0; i < sequenceOptions; i++)
  {
    if(sequenceButtons[i].isPressed()){
      sequenceLeds[i].on();
    }else{
      sequenceLeds[i].off();
    }
  } 
}






















*/


