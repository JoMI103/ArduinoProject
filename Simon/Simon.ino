#include "Arduino.h"
#include "Led.h"
#include "Button.h"

#include "LiquidCrystal_I2C.h"
#include  "Wire.h"

#define between_color_delay 200
#define color_show_delay 400

LiquidCrystal_I2C lcd(0x27,  16, 2);

byte buzzer = 11;

int musicalNotes[4] = {440,622,659,554 };
Button sequenceButtons[4] = {2,4,6,8}; //red,Green, blue, yellow
Led sequenceLeds[4] = {3,5,7,9}; 

byte sequenceOptionsLength;

byte currentSequenceLength = 0;
byte sequenceLength;
byte sequence[100] {0};


byte gameMode;

bool CheckPlayerSequence();
void AddSequenceDifficulty();
void ShowCurrentSequence(byte);
byte SelectMode();

void setup()
{
  sequenceOptionsLength = sizeof(sequenceLeds) / sizeof(Led);
  sequenceLength =  sizeof(sequence) / sizeof(byte);
  

  lcd.init();
  lcd.backlight();

  AddSequenceDifficulty();

  gameMode = 0;
}


void loop()
{ 
  gameMode = SelectMode();

  ShowCurrentSequence(gameMode);
  
  if(CheckPlayerSequence())
  {
    AddSequenceDifficulty();
  }
  else
  {
    Serial.println("Wrong color. resetting game");
	currentSequenceLength = 0;
  }



  lcd.setCursor(0,0);
  
  lcd.print(currentSequenceLength);
  

  
  delay(1500);
  Serial.println("Starting new game");
 // }
}

byte SelectMode(){
  return 0;
}

void AddSequenceDifficulty()
{
  Serial.println("Adding difficulty");
  sequence[currentSequenceLength] = random(0,sequenceOptionsLength);
  currentSequenceLength++;
  
  if(currentSequenceLength >= sequenceLength)
  {
    Serial.println("Max score reached");
    currentSequenceLength = 0;
  }	 

  tone(buzzer, 400, 100); 
}

void ShowCurrentSequence(byte mode)
{
  switch(mode)
  {
    case 0:
      for(int e = 0; e < currentSequenceLength; e++)
      {
        sequenceLeds[sequence[e]].on(buzzer,musicalNotes[sequence[e]], 100);
        delay(color_show_delay);
        sequenceLeds[sequence[e]].off();
        delay(between_color_delay);
      } 
    break;

    case 1:
      for(int e = 0; e < currentSequenceLength; e++)
      {
        for(int l = 0; l < sequenceOptionsLength; l++){
          if(l != sequence[e]){
            sequenceLeds[l].on();  
          }
        }
        delay(color_show_delay);

        for(int l = 0; l < sequenceOptionsLength; l++){
          if(l != sequence[e]){
            sequenceLeds[l].off();  
          }
        }
        delay(between_color_delay);
      } 
    break;
  }
}

bool CheckPlayerSequence()
{
  byte currentOption = 0;
  byte element = 0;
  
  while(element < currentSequenceLength)
  {
    for(int i = 0; i < sequenceOptionsLength; i++)
      {
        if(sequenceButtons[i].isPressed())
        {
          Serial.println("entrou");
          if(i == sequence[element])
          {
            sequenceLeds[i].on(buzzer,musicalNotes[i], 100);
            delay(10);
            while(sequenceButtons[i].isPressed()){
              delay(10);
            }
            element++;
            sequenceLeds[i].off();
            Serial.println("ok");
           
          }
          else
          {
            Serial.println("Errou");
            return false;
          }
        }
      }
    
    
    delay(10);
  }
  
	return true;
}


