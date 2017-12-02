const int BUTTON_CHANGE_PLAYER = 10; //blå knapp
const int BUTTON_STOP_AND_START = 9; //rød knapp

const int S_IDLE = 0;
const int S_PLAYER1_PLAYING = 1;
const int S_PLAYER2_PLAYING = 2;
const int S_STOPPED = 3;

boolean changePlayerButton = false;
boolean stopStartButton = false;

unsigned long nextTimeout = 0;

int currentState = S_IDLE;

//Time left for the two players, in seconds.
// Timer starts on 2 min left for both players
// 2 minutes => 5*60 = 120 seconds
const int PLAY_TIME = 120;
int timeLeftPlayer1 = PLAY_TIME;
int timeLeftPlayer2 = PLAY_TIME;

#include <LiquidCrystal.h>

//The variable used to communicate with the LCD.
// The LCD is connected to the pins 12, 11, 5, 4, 3 and 2
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Legg til DEBUG

void setup() {

  //Initialise the LCD
  lcd.begin(16, 2);
  setupLCD();
}

void loop() {
  countdownChessClock();
  isButtonStartStoppedPressed();
  isChangePlayerButtonPressed();
}

void countdownChessClock()
{
  switch(currentState)
  {
    case S_IDLE:
      if(stopStartButton)
      {
        startTimer(1000); //Set timer to 1 sec
        resetPlayerClocks();
        updateDisplay(timeLeftPlayer1, timeLeftPlayer2);
        changeStateTo(S_PLAYER1_PLAYING);
        
      }

    break;

    case S_PLAYER1_PLAYING:
      //Check if changePlayer button was pressed
      if(changePlayerButton)
      {
        changeStateTo(S_PLAYER2_PLAYING);
      }

      //Check if start/stop button was pressed
      if(stopStartButton)
        {
          changeStateTo(S_STOPPED); 
        }

      //Check if 1-second timer has expired, if so
      //countdown the time left for player1
      //and update the display, and restart the timer
      if(timerHasExpired())
      {
        timeLeftPlayer1--;
        updateDisplay(timeLeftPlayer1, timeLeftPlayer2);
        startTimer(1000);
      }

      //Check if the time has expired for player1
      if(timeLeftPlayer1 <= 0)
      {
        changeStateTo(S_STOPPED);
      }

    break;

    case S_PLAYER2_PLAYING:
    //Check if change player button was pressed
      if(changePlayerButton)
      {
        changeStateTo(S_PLAYER1_PLAYING);
      }

      //Check if start/stop button was pressed
      if(stopStartButton)
      {
        changeStateTo(S_STOPPED);
      }

      // Check if 1-second timer has expired, if so
      // count down the time left for player 1
      // and update the display, and restart the timer
      if(timerHasExpired())
      {
        timeLeftPlayer2--;
        updateDisplay(timeLeftPlayer1, timeLeftPlayer2);
        startTimer(1000);
      }

      //Check if the time was expired for player 1
      if(timeLeftPlayer2 <= 0)
      {
        changeStateTo(S_STOPPED);
      }
    break;

    // One of the players have used his/her time, or
    // the start/stop-button was pressed.    
    case S_STOPPED:
      if(stopStartButton)
      {
        setupLCD();
        resetPlayerClocks();
        changeStateTo(S_IDLE);
      }
    break;
  }
}

void isChangePlayerButtonPressed()
{
  int buttonState = digitalRead(BUTTON_CHANGE_PLAYER);
  if (buttonState == LOW)
  {
    changePlayerButton = true;
  }
}

void isButtonStartStoppedPressed()
{
  int buttonState = digitalRead(BUTTON_STOP_AND_START);
  if (buttonState == LOW)
  {
    stopStartButton = true;
  }
}

void changeStateTo(int newState)
{
  currentState = newState;
}

void startTimer(unsigned long duration)
{
  nextTimeout = millis() + duration;
}

boolean timerHasExpired()
{
  boolean hasExpired = false;
  if (millis() > nextTimeout)
  {
    hasExpired = true;
  }
  else
  {
    hasExpired = false;
  }
  return hasExpired;
}

void setupLCD() 
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PLAYER1  PLAYER2");
} 

void resetPlayerClocks()
{
  timeLeftPlayer1 = PLAY_TIMEupdateDisplay
  timeLeftPlayer2 = PLAY_TIME;
}

/*
   Update the display with the current time left for the two players.
   The time left are provided in the parameters timeLeftPlayer1 and
   timeLeftPlayer2, in seconds.
   @param timeLeftPlayer1 The number of seconds that player 1 has left
   @param timeLeftPlayer2 The number of seconds that player 2 has left
*/
void updateDisplay( int timeLeftPlayer1, int timeLeftPlayer2 )
{
  char timeString[17] = ""; //("") = tom
  lcd.setCursor(0, 1);

  sprintf(timeString, "  %02d:%02d  %02d:%02d  ",
          timeLeftPlayer1 / 60, timeLeftPlayer1 % 60,
          timeLeftPlayer2 / 60, timeLeftPlayer2 % 60);
  lcd.print( timeString );

}

