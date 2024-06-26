#include "Arduino.h"
#include "Led.h"
#include "Button.h"


Button sequenceButtons[4] = {A0,A1,A2,4}; //,A3,4,2};
Led sequenceLeds[4] = {3,5,6,9}; //,9,10,11};

byte sequenceOptionsLength;
byte lastPin = 0;

byte sequence[100] {0};
byte sequenceLength;
byte currentSequenceLength = 0;

#define between_color_delay 500
#define color_show_delay 1000


bool CheckPlayerSequence();
void AddSequenceDifficulty();
void ShowCurrentSequence();

void setup()
{
  sequenceOptionsLength = sizeof(sequenceLeds) / sizeof(Led);
  sequenceLength =  sizeof(sequence) / sizeof(byte);
  
  Serial.begin(9600);
  //lcd_1.begin(16, 2);
  //lcd_1.setCursor(0, 1);
  //lcd_1.print("hello worloooooooood");
  
  AddSequenceDifficulty();
}


void loop()
{ 
  ShowCurrentSequence();
  
  if(CheckPlayerSequence())
  {
    AddSequenceDifficulty();
  }
  else
  {
    Serial.println("Wrong color. resetting game");
	currentSequenceLength = 0;
  }
  
  delay(1500);
  Serial.println("Starting new game");
  
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
}

void ShowCurrentSequence()
{
  for(int e = 0; e < currentSequenceLength; e++)
  {
    sequenceLeds[sequence[e]].on();
    delay(color_show_delay);
    sequenceLeds[sequence[e]].off();
    delay(between_color_delay);
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
            element++;
            Serial.println("ok");
            delay(500);
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


/*
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





















