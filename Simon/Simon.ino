#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "Wire.h"
#include "EEPROM.h"

#include "Led.h"
#include "Button.h"

bool CheckPlayerSequence(byte);
void AddSequenceDifficulty(byte);
void ShowCurrentSequence(byte);
void MainMenu();

bool NewRecord();
void LoadScoreData();
void SaveHighScore();
void SaveRecordMenu();

void LcdPrint(char, char);
void LcdPrint(String, String);



#define between_color_delay 200
#define color_show_delay 400

#define game_modes_number 4

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


Settings currentSettings = {
  true
};


Player currentPlayer;

Player modeHighScore[game_modes_number];


void setup()
{
  
  byte seed = EEPROM.read(99);
  randomSeed(seed);
  seed ++;
  if(seed > 250){
    seed = 0;
  } 
  EEPROM.write(99, seed);

  Serial.begin(9600);
 
  for(int i = 0; i < game_modes_number; i++)
    {
      modeHighScore[i] = Player {
        i,0,"         "
      };
    }

  if(false){
    for(int i = 0; i < 250; i++){
      EEPROM.write(i, 0);
    }
  }

  
  
  lcd.init();
  lcd.backlight();
  
  

  

  currentPlayer = Player{
    0, 0, "         "
  };
}

void loop()
{ 
  LoadScoreData();
  MainMenu();
  delay(500);
  currentSequenceLength = 0;
  AddSequenceDifficulty(currentPlayer.GameMode);

  while(currentSequenceLength != 0)
  {
    LcdPrint(
      String("Score: ") + (currentSequenceLength - 1), 
      String("") + "HS:" + modeHighScore[currentPlayer.GameMode].name + ":" + (modeHighScore[currentPlayer.GameMode].Score - 1));


    ShowCurrentSequence(currentPlayer.GameMode);

    if(CheckPlayerSequence(currentPlayer.GameMode))
    {
      AddSequenceDifficulty(currentPlayer.GameMode);
    }
    else
    {
      LcdPrint("Wrong color", "resetting game");
      
      if( NewRecord()){
        SaveRecordMenu();
        currentPlayer.Score = currentSequenceLength;
        SaveHighScore();
      }

      for(int i = 0; i < 3; i++){
        Serial.println(modeHighScore[i].name);
        Serial.print(modeHighScore[i].Score);
      }

      currentSequenceLength = 0;
    }
    
    delay(500);
  }
  delay(1500);
}

#pragma region GetInputs
int GetInput(){
  for(int i = 0; i < sequence_options_length; i++)
    {
      if(sequenceButtons[i].isPressed())
      {
        sequenceLeds[i].OnWithSound(currentSettings.Muted,buzzer,musicalNotes[i], 100);
        delay(10);
        while(sequenceButtons[i].isPressed()){
          delay(10);
        }
        sequenceLeds[i].Off();
        return i;
        
      }
    }
    return -1;
}

int GetFreeInput(){
  for(int i = 0; i < sequence_options_length; i++)
    {
      if(sequenceButtons[i].isPressed())
      {
        
        delay(10);
        sequenceLeds[i].Off();
        return i;
        
      }
    }
    return -1;
}

#pragma endregion GetInputs

#pragma region LCD Methods

//red, Green, blue, yellow
void LcdPrint(char firstRow[], char secondRow[]){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(firstRow);
  
  lcd.setCursor(0,1);
  lcd.print(secondRow);
}

void LcdPrint(String firstRow, String secondRow){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(firstRow);
  
  lcd.setCursor(0,1);
  lcd.print(secondRow);
}


#pragma endregion LCD Methods

#pragma region ScreenMenu Methods




//nao tenho rogulho deste codigo mas da para o gasto :)
void MainMenu(){
  char sound;


  int selectedOption = -1;
  bool changed = true;
  int selectedOptionHSMenu = -1;
  bool changedHSMenu = true;
  int checkingModeRecord = 0;

  while(selectedOption != 0){

    if(changed)
    {
      if(currentSettings.Muted) sound = 'm'; else sound ='s';
      LcdPrint( String("R-Play ") +"G-Mode:" + currentPlayer.GameMode ,String("B-HighScores") + " Y-" + sound );
    }  
    changed = true;

    selectedOption = GetInput();

    switch(selectedOption){
      case 0: break;
      case 1: 
        currentPlayer.GameMode++;
        if(currentPlayer.GameMode >= game_modes_number)
        currentPlayer.GameMode = 0;

       break;
      case 2:

        selectedOptionHSMenu = -1;
        while(selectedOptionHSMenu != 0){
          if(changedHSMenu)
          {
            LcdPrint( String("") + "Mode:" + checkingModeRecord + " Record",
            String("") + modeHighScore[checkingModeRecord].name + ":" + (modeHighScore[checkingModeRecord].Score - 1) );
          }
          changedHSMenu = true;
          
          selectedOptionHSMenu = GetInput();


          switch(selectedOptionHSMenu)
          {
            case 0: selectedOption = -1; break;
            case 1:
              checkingModeRecord++;
              if(checkingModeRecord >= game_modes_number)
              checkingModeRecord = 0;

             break;
             case 3:{

              Serial.println(" ");
              Serial.println(" ");
              Serial.println(" ");
              Serial.println("new entry");
              Serial.println(" ");
                for(int i = 0; i < 100; i++){
                      Serial.print(EEPROM.read(i));
                      Serial.print(" ");
                      if((i+1) % 20 == 0) 
                      Serial.println("");
                    }
             } break;
            default: changedHSMenu = false; break;
          }

        }

      

       break;
      case 3: currentSettings.Muted = !currentSettings.Muted; break;

      default: changed = false; break;
    }
    delay(10);

  }

}


void SaveRecordMenu()
{
  delay(250);

  int nextDelay = 0;
  int selectedOption = -1;
  bool changed = true;

  int selectedChar = 0;
  char name[10] = {' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'};
  char final[16] = {0};


  while(selectedChar < 11)
  {
     
    if(changed)
    {

      for(int i = 0; i < 10; i++)
      {
        
        if(i == selectedChar){
          final[i] = ',';
          
        }
        if(i >= selectedChar){
          final[i+1] = name[i];  
        }else{
          final [i] = name[i];
        }
      }

      if(selectedChar == 10)
        final[10] = '-';
      
      if(selectedChar == 11)
        final[11] = '>';
      
      LcdPrint(String(final),String("") + "R:< " + "G:> " + "B:^ " + "Y:v" );
    }  

    delay(nextDelay);

    changed = true;

    selectedOption = GetFreeInput();

    switch(selectedOption){
      case 0: selectedChar--; if(selectedChar < 0) selectedChar = 0; nextDelay = 400; break;
      case 1:  selectedChar++;   nextDelay = 400; break;
      case 2: if(selectedChar < 9){
        
        if(name[selectedChar] <= 32)
        name[selectedChar] = 'z';
        else
        name[selectedChar]--;

        nextDelay = 50;

      } break;
      case 3: if(selectedChar < 9){
         if(name[selectedChar] >= 122)
         name[selectedChar] = ' ';
         else
         name[selectedChar]++;
         nextDelay = 100;

      } break;
      default: changed = false; break; 
    }

    delay(10);
  }

  for(int i = 0; i < 10; i++){
    currentPlayer.name[i] = name[i];
  }

}

#pragma endregion ScreenMenu Methods

#pragma region HighScore saving methods

void LoadScoreData()
{
  int startIndex = 0;
  for(int i = 0; i < game_modes_number; i++)
  {
    modeHighScore[i] = Player {
      i,0,"guest"
    };

    startIndex = i * 20;
    modeHighScore[i].GameMode = EEPROM.read(startIndex);
    modeHighScore[i].Score = EEPROM.read(startIndex + 1);

    for(int n = 0; n < 9; n++)
    {
      modeHighScore[i].name[n] = EEPROM.read(startIndex + 2 + n);
    }
    modeHighScore[i].name[9] = '\0';
  }
}

bool NewRecord()
{
  int startIndex = currentPlayer.GameMode * 20;

  if(currentSequenceLength > EEPROM.read(startIndex + 1))
  {
    return true;
  } 
  else
  {
    return false;
  }
}

void SaveHighScore()
{

  int startIndex = (currentPlayer.GameMode) * 20;
  for(int i = startIndex; i < startIndex + 20; i++ ){
    EEPROM.write(i,0);
  }

  modeHighScore[currentPlayer.GameMode].Score = currentPlayer.Score;
  modeHighScore[currentPlayer.GameMode].GameMode = currentPlayer.GameMode;
  for(int n = 0; n < 10; n++)
  {
    
    modeHighScore[currentPlayer.GameMode].name[n] = currentPlayer.name[n];
  }


  EEPROM.write(startIndex , modeHighScore[currentPlayer.GameMode].GameMode);
  EEPROM.write(startIndex + 1 , modeHighScore[currentPlayer.GameMode].Score);

  for(int n = 0; n < 10; n++)
  {
    EEPROM.write(startIndex + 2 + n ,modeHighScore[currentPlayer.GameMode].name[n]);
  }

}

#pragma endregion HighScore saving methods

#pragma region Sequence methods

void AddSequenceDifficulty(byte mode)
{
  if(mode == 3 && currentSequenceLength > 2){
    byte randomvalue= random(0,10);
    Serial.print(randomvalue);
    if(randomvalue > 4){
      Serial.print("New random number");
      byte ok =  random(0,currentSequenceLength-1);
      sequence[ok] = random(0,sequence_options_length);
    }
  }
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
    case 0: case 2: case 3:
      for(int e = 0; e < currentSequenceLength; e++)
      {
        sequenceLeds[sequence[e]].OnWithSound(currentSettings.Muted,buzzer,musicalNotes[sequence[e]], 100);
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

bool CheckPlayerSequence(byte mode)
{
  int element ;
  
  switch(mode)
  {
    case 0: 
    case 1: 
    case 3:
    {
      element = 0;
      while(element < currentSequenceLength)
      {
        for(int i = 0; i < sequence_options_length; i++)
          {
            if(sequenceButtons[i].isPressed())
            {
              if(i == sequence[element])
              {
                sequenceLeds[i].OnWithSound(currentSettings.Muted,buzzer,musicalNotes[i], 100);
                delay(10);
                while(sequenceButtons[i].isPressed()){
                  delay(10);
                }
                element++;
                sequenceLeds[i].Off();
              
              }
              else
              {
                return false;
              }
            }
          }
        
        
        delay(10);
      }
    }
    break;
    case 2:
    {
      element = currentSequenceLength - 1;
      while(element >= 0)
      {
        for(int i = 0; i < sequence_options_length; i++)
          {
            if(sequenceButtons[i].isPressed())
            {
              if(i == sequence[element])
              {
                sequenceLeds[i].OnWithSound(currentSettings.Muted,buzzer,musicalNotes[i], 100);
                delay(10);
                while(sequenceButtons[i].isPressed()){
                  delay(10);
                }
                element--;
                sequenceLeds[i].Off();
              
              }
              else
              {
                return false;
              }
            }
          }
        
        
        delay(10);
      }
    }
    break;
  }

  
  
	return true;
}

#pragma endregion Sequence methods

