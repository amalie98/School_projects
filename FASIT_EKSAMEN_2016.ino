/**
   This project implements a chess clock used for the game Chess.
   The clock is used to keep track on the time spent by each of the two
   players.
   Two buttons and a LCD-display is being used. The two buttons are
   being used as follows:

   Button 1 is the start/stop-button. This buttin is pressed to start
            the game and to stop the game.

   Button 2 is the button that the players hit when they have prformed
            their move.I.e. when player 1 has performed his move, he press
            this button, his remaining time is stopped, the player 2 is
            starting it1s countdown

   The remaining playing time for the two players are stored/represented
   with the two global variables timeLeftPlayer1 and timeLeftPlayer2.
   The time is stored in seconds. This way, it is sufficient to decrement
   the variables once every 1 second, and it is easy to detect when time has
   expired.

   The solution is implemented using the state machine concept used in
   Assignment 4 (trafficlights). Most of the code has been reused, like
   the debug-option, the changeState()-function and the timer-solution.

   In order to represent the time on the format MM:SS, we need to extract
   minutes and seconds from the variables holding the remainding time.
   This can be done by using integer division and the modulo-operator:

       int player1Minutes = timeLeftPlayer1/60;
       int player1Seconds = timeLeftPlayer1%60;

   and should be done in the function that updates the display.
*/
#include <LiquidCrystal.h>

// The variable used to communicate with the LCD.
// The LCD is connected to the pins 12, 11, 5, 4, 3 and 2

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Constants and variables used for the 2 buttons
const int NUMBER_OF_BUTTONS = 2;
int buttonPins[] = {9, 10};   // The pin numbers for the two buttons
int prevButtonStates[NUMBER_OF_BUTTONS]; // Used to eliminate bouncing

const int START_STOP_BUTTON = 0;
const int SWITCH_PLAYER_BUTTON = 1;

// Constants and variables used for states in the state machine
const int S_IDLE = 0;
const int S_PLAYER1_PLAYING = 1;
const int S_PLAYER2_PLAYING = 2;
const int S_STOPPED = 3;

int currentState;

// Table of strings representing the different states. This table is used
// by the function printState() for debugging.
String stateStr[] = {
  "S_IDLE", "S_PLAYER1_PLAYING", "S_PLAYER2_PLAYING", "S_STOPPED"
};

// A flag used to enable and disable debug-information to be printed
// to the Serial Monitor window
const boolean DEBUG = true;

// Time for next timeout, in millis, used by the timer
long nextTimeout = 0;

// Time left for the two players, in seconds.
// Timer starts on 5 min left for both players
// 5 minutes => 5*60 s = 300 seconds
const int PLAY_TIME = 300;
int timeLeftPlayer1 = PLAY_TIME;
int timeLeftPlayer2 = PLAY_TIME;

void setup() {
  if (DEBUG)
  {
    Serial.begin(9600);
  }

  // Initialise the buttons
  initButtons();

  // Initialise the LCD
  lcd.begin(16, 2);
  initDisplay();

  // Set initial state
  currentState = S_IDLE;
}

void loop() {

  boolean startStopButtonPressed = isButtonPressed(START_STOP_BUTTON);
  boolean changePlayerButtonPressed = isButtonPressed(SWITCH_PLAYER_BUTTON);

  switch (currentState)
  {
    // Waiting to start the game
    case S_IDLE:
      if ( startStopButtonPressed )
      {
        startTimer( 1000 ); //Set timer to 1 sec
        resetPlayerClocks();
        updateDisplay( timeLeftPlayer1, timeLeftPlayer2 );
        changeStateTo( S_PLAYER1_PLAYING );
      }
      break;

    // Player 1 is thinking, preparing his move
    case S_PLAYER1_PLAYING:
      // Check if change player button was pressed
      if (changePlayerButtonPressed)
      {
        changeStateTo( S_PLAYER2_PLAYING );
      }
      
      // Check if start/stop button was pressed
      if (startStopButtonPressed)
      {
        changeStateTo( S_STOPPED );
      }
      
      // Check if 1-second timer has expired, if so
      // count down the time left for player 1
      // and update the display, and restart the timer
      if (timerHasExpired())
      {
        timeLeftPlayer1--;
        updateDisplay( timeLeftPlayer1, timeLeftPlayer2 );
        startTimer( 1000 );
      }

      // Check if the time has expired for player 1
      if (timeLeftPlayer1 <= 0 )
      {
        changeStateTo( S_STOPPED );
      }

      break;

    // Player 2 is thinking, preparing his move
    case S_PLAYER2_PLAYING:
     // Check if change player button was pressed
      if (changePlayerButtonPressed)
      {
        changeStateTo( S_PLAYER1_PLAYING );
      }
      
      // Check if start/stop button was pressed
      if (startStopButtonPressed)
      {
        changeStateTo( S_STOPPED );
      }
      
      // Check if 1-second timer has expired, if so
      // count down the time left for player 1
      // and update the display, and restart the timer
      if (timerHasExpired())
      {
        timeLeftPlayer2--;
        updateDisplay( timeLeftPlayer1, timeLeftPlayer2 );
        startTimer( 1000 );
      }

      // Check if the time has expired for player 1
      if (timeLeftPlayer2 <= 0 )
      {
        changeStateTo( S_STOPPED );
      }
      break;

    // One of the players have used his/her time, or
    // the start/stop-button was pressed.
    case S_STOPPED:
      if (startStopButtonPressed)
      {
        initDisplay();
        resetPlayerClocks();
        changeStateTo(S_IDLE);
      }
      break;
  }
}

/**
   Initialises the buttons.
*/
void initButtons()
{
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++)
  {
    pinMode( buttonPins[i], INPUT );
    prevButtonStates[i] = LOW;
  }
}

/**
   Resets the player clocks to 5 minutes
*/
void resetPlayerClocks()
{
  timeLeftPlayer1 = PLAY_TIME;
  timeLeftPlayer2 = PLAY_TIME;
}
/**
   Shows the static part on the LCD-screen.
*/
void initDisplay()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Player1  Player2");
}

/**
   Update the display with the current time left for the two players.
   The time left are provided in the parameters timeLeftPlayer1 and
   timeLeftPlayer2, in seconds.
   @param timeLeftPlayer1 The number of seconds that player 1 has left
   @param timeLeftPlayer2 The number of seconds that player 2 has left
*/
void updateDisplay( int timeLeftPlayer1, int timeLeftPlayer2 )
{
  char timeString[17] = "";
  lcd.setCursor(0, 1);

  sprintf(timeString, "  %02d:%02d  %02d:%02d  ",
          timeLeftPlayer1 / 60, timeLeftPlayer1 % 60,
          timeLeftPlayer2 / 60, timeLeftPlayer2 % 60);
  lcd.print( timeString );

}

/**
   Returns true if the button was pressed. Detects the
   state change from not-pressed to pressed (edge detection).
   The function also deals with the problem of bouncing.

   @param btnNumber the number of the button to check (corresponds
                    to the index in the arrays defined for button pins
                    and previous button state.
*/
boolean isButtonPressed(int btnNumber)
{
  boolean result = false;
  int btnState1 = digitalRead( buttonPins[btnNumber] );
  delay(50);
  int btnState2 = digitalRead( buttonPins[btnNumber] );

  if (btnState1 == btnState2)   //NB!! == ikke =
  {
    if ( HIGH == btnState1)
    {
      if ( prevButtonStates[btnNumber] != btnState1 )
      {
        result = true;
      }
    }
    prevButtonStates[btnNumber] = btnState1;
  }
  else
  {
    result = false;
  }
  return result;
}

//----- Timer functions ----

/**
   Checks if the timer has expired. If so, true is returned.
   If the timer has not yet "expired", false is returned.

   @return true if timer expired, false if not.
*/
boolean timerHasExpired()
{
  boolean result = false;
  if (millis() > nextTimeout)
  {
    result = true;
  }
  else
  {
    result = false;
  }
  return result;
}

/**
   Starts the timer and set the timer to expire after
   the number of milliseconds given by the parameter timeout.

   @param timeout The number of milliseconds until the timer will expire.
*/
void startTimer(int timeout)
{
  nextTimeout = millis() + timeout;
}



// ---- State-change functions -----

/**
   Changes the currentState to the new state provided as parameter

   @param newState the state to change to
   @return the new current state
*/
int changeStateTo(int newState)
{
  if (DEBUG)
  {
    Serial.print("Changed state from ");
    printState( currentState );
    Serial.print(" to ");
    printState( newState );
    Serial.println();
  }
  currentState = newState;
  return currentState;
}

/**
   Prints the state given by the parameter someState as a String
   to the Serial Monitor.

   @param someState the state to be printed as String
*/
void printState(int someState)
{
  Serial.print(stateStr[someState]);
}
