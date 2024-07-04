#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "Wire.h"
#include "EEPROM.h"

#include "Led.h"
#include "Button.h"

bool CheckPlayerSequence();
void AddSequenceDifficulty();
void ShowCurrentSequence(byte);
byte SelectMode();

bool NewRecord();
void LoadScoreData();
void SaveHighScore();


#define between_color_delay 200
#define color_show_delay 400

#define game_modes_number 3

#define sequence_options_length 4

#define game_sequence_length 100


//red, Green, blue, yellow
int musicalNotes[sequence_options_length] = {440,622,659,554 };
Button sequenceButtons[sequence_options_length] = {2,4,6,8}; 
Led sequenceLeds[sequence_options_length] = {3,5,7,9}; 

LiquidCrystal_I2C lcd(0x27,  16, 2);
byte buzzer = 11;


byte currentSequenceLength = 0;
byte sequence[game_sequence_length] {0};


struct Settings{
  bool Muted;
};

struct Player{
  byte GameMode;
  byte Score;
  char name[10];
};




Player currentPlayer;

Player modeHighScore[game_modes_number];










void setup()
{  

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  
  LoadScoreData();

  currentPlayer = Player{
    0, 0, "Guest"
  };

  AddSequenceDifficulty();
}




void loop()
{ 
  ShowCurrentSequence(currentPlayer.GameMode);
  
  if(CheckPlayerSequence())
  {
    AddSequenceDifficulty();
  }
  else
  {
    Serial.println("Wrong color. resetting game");
    if( NewRecord()){
      //write name 
      SaveHighScore();
    }

    for(int i = 0; i < 3; i++){
      Serial.println(modeHighScore[i].name);
      Serial.print(modeHighScore[i].Score);
    }

	  currentSequenceLength = 0;
  }



  lcd.setCursor(0,0);
  
  lcd.print(currentSequenceLength);
  

  
  delay(1500);
  Serial.println("Starting new game");
 // }
}

#pragma region ScreenMenu Methods

byte SelectMode(){
  return 0;
}
#pragma endregion ScreenMenu Methods

#pragma region HighScore saving methods

void LoadScoreData()
{
  for(int i = 0; i < game_modes_number; i++)
  {
    int startIndex = (i + 1) * 32;
    modeHighScore[i].GameMode = EEPROM.read(startIndex);
    modeHighScore[i].Score = EEPROM.read(startIndex + 1);

    for(int n = 0; n < 10; n++){
      modeHighScore[i].name[n] = EEPROM.read(startIndex + 2 + n);
    }
  }
}


bool NewRecord()
{
  int startIndex = (currentPlayer.GameMode + 1) * 32;
  if(currentSequenceLength > EEPROM.read(startIndex)){
    return true;
  } else{
    return false;
  }
}

void SaveHighScore(){
  int startIndex = (currentPlayer.GameMode + 1) * 32;
  for(int i = startIndex; i < startIndex + 31; i++ ){
    EEPROM.write(startIndex,0);
  }

  modeHighScore[currentPlayer.GameMode] = currentPlayer;


    EEPROM.write(startIndex , modeHighScore[currentPlayer.GameMode].GameMode);
    EEPROM.write(startIndex + 1 , modeHighScore[currentPlayer.GameMode].GameMode);
    for(int n = 0; n < 10; n++){
      EEPROM.write(startIndex + 2 + n ,modeHighScore[currentPlayer.GameMode].name[n]);
    }

}

#pragma endregion HighScore saving methods

#pragma region Sequence methods
void AddSequenceDifficulty()
{
  Serial.println("Adding difficulty");
  sequence[currentSequenceLength] = random(0,sequence_options_length);
  currentSequenceLength++;
  
  if(currentSequenceLength >= game_sequence_length)
  {
    Serial.println("Max score reached");
    currentSequenceLength = 0;
  }	 

  //tone(buzzer, 400, 100); 
}

void ShowCurrentSequence(byte mode)
{
  switch(mode)
  {
    case 0:
      for(int e = 0; e < currentSequenceLength; e++)
      {
        sequenceLeds[sequence[e]].OnWithSound(true,buzzer,musicalNotes[sequence[e]], 100);
        delay(color_show_delay);
        sequenceLeds[sequence[e]].Off();
        delay(between_color_delay);
      } 
    break;

    case 1:
      for(int e = 0; e < currentSequenceLength; e++)
      {
        for(int l = 0; l < sequence_options_length; l++){
          if(l != sequence[e]){
            sequenceLeds[l].On();  
          }
        }
        delay(color_show_delay);

        for(int l = 0; l < sequence_options_length; l++){
          if(l != sequence[e]){
            sequenceLeds[l].Off();  
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
    for(int i = 0; i < sequence_options_length; i++)
      {
        if(sequenceButtons[i].isPressed())
        {
          Serial.println("entrou");
          if(i == sequence[element])
          {
            sequenceLeds[i].OnWithSound(true,buzzer,musicalNotes[i], 100);
            delay(10);
            while(sequenceButtons[i].isPressed()){
              delay(10);
            }
            element++;
            sequenceLeds[i].Off();
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

#pragma endregion Sequence methods






