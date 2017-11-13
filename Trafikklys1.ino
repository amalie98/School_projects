const int GREEN_PIN_1_3 = 13;
const int YELLOW_PIN_1_3 = 12;
const int RED_PIN_1_3 = 11;
const int GREEN_PIN_2_4 = 10;
const int YELLOW_PIN_2_4 = 9;
const int RED_PIN_2_4 = 8;
const int GREEN_PIN_F1_F3 = 7;
const int RED_PIN_F1_F3 = 6;
const int GREEN_PIN_F2_F4 = 5;
const int RED_PIN_F2_F4 = 4;
const int BUTTON_F1_F3 = 3;
const int BUTTON_F2_F4 = 2;
//const int BUTTON_OUT = 1;

const int S_1_3_DRIVE_2_4_STOP = 0;
const int S_1_3_PREPARE_TO_STOP_2_4_PREPARE_TO_DRIVE = 1;
const int S_1_3_STOP_2_4_DRIVE = 2;
const int S_1_3_PREPARE_TO_DRIVE_2_4_PREPARE_TO_STOP = 3;
const int S_RESET = 4;
const int S_PREPARE_TO_WALK_F1_F3 = 5;
const int S_WALK_F1_F3 = 6;
const int S_STOP_WALKING_F1_F3 = 7;
const int S_RESET_F1 = 8;
const int S_PREPARE_TO_WALK_F2_F4 = 9;
const int S_WALK_F2_F4 = 10;
const int S_STOP_WALKING_F2_F4 = 11;
const int S_RESET_F4 = 12;

int button_F1_F3_State;
int button_F2_F4_State;
int button_DEFAULT_state;

unsigned long nextTimeout = 0;

int currentState1 = S_1_3_DRIVE_2_4_STOP;
int currentState2 = S_PREPARE_TO_WALK_F1_F3;
int currentState3 = S_PREPARE_TO_WALK_F2_F4;

boolean pedestrianF1 = false;
boolean pedestrianF4 = false;


void setup()
{
  pinMode(GREEN_PIN_1_3, OUTPUT);
  pinMode(YELLOW_PIN_1_3, OUTPUT);
  pinMode(RED_PIN_1_3, OUTPUT);
  pinMode(GREEN_PIN_2_4, OUTPUT);
  pinMode(YELLOW_PIN_2_4, OUTPUT);
  pinMode(RED_PIN_2_4, OUTPUT);
  pinMode(GREEN_PIN_F1_F3, OUTPUT);
  pinMode(GREEN_PIN_F2_F4, OUTPUT);
  pinMode(RED_PIN_F1_F3, OUTPUT);
  pinMode(RED_PIN_F2_F4, OUTPUT);

  pinMode(BUTTON_F1_F3, INPUT);
  pinMode(BUTTON_F2_F4, INPUT);
  //pinMode(BUTTON_OUT, INPUT);

  Serial.begin(9600);
}


void loop()
{
  isButtonPressedF1();
  delay(1000);
  isButtonPressedF4();
  delay(1000);
}


void turnAllLedOff ()
{
  digitalWrite (GREEN_PIN_1_3, LOW);
  digitalWrite (YELLOW_PIN_1_3, LOW);
  digitalWrite (RED_PIN_1_3, LOW);
  digitalWrite (GREEN_PIN_2_4, LOW);
  digitalWrite (YELLOW_PIN_2_4, LOW);
  digitalWrite (RED_PIN_2_4, LOW);
  digitalWrite (GREEN_PIN_F1_F3, LOW);
  digitalWrite (RED_PIN_F1_F3, LOW);
  digitalWrite (GREEN_PIN_F2_F4, LOW);
  digitalWrite (RED_PIN_F2_F4, LOW);
}


void turnLedOn(int pin)
{
  digitalWrite (pin, HIGH);
}


void blinkLed(int pin)
{
  digitalWrite(pin, HIGH);
  delay(400);
  digitalWrite(pin, LOW);
  delay(400);
  digitalWrite(pin, HIGH);
  delay(400);
  digitalWrite(pin, LOW);
  delay(400);
  digitalWrite(pin, HIGH);
}


void setTrafficLights(int fase)
{
  switch (fase)
  {
    case 1:
      turnAllLedOff ();
      turnLedOn (GREEN_PIN_1_3);
      turnLedOn (RED_PIN_2_4);
      turnLedOn (RED_PIN_F1_F3);
      turnLedOn (RED_PIN_F2_F4);

      break;

    case 2:
      turnAllLedOff ();
      turnLedOn (YELLOW_PIN_1_3);
      turnLedOn (YELLOW_PIN_2_4);
      turnLedOn (RED_PIN_2_4);
      turnLedOn (RED_PIN_F1_F3);
      turnLedOn (RED_PIN_F2_F4);

      break;

    case 3:
      turnAllLedOff ();
      turnLedOn (RED_PIN_1_3);
      turnLedOn (GREEN_PIN_2_4);
      turnLedOn (RED_PIN_F1_F3);
      turnLedOn (RED_PIN_F2_F4);

      break;

    case 4:
      turnAllLedOff();
      turnLedOn (YELLOW_PIN_1_3);
      turnLedOn (RED_PIN_1_3);
      turnLedOn (YELLOW_PIN_2_4);
      turnLedOn (RED_PIN_F1_F3);
      turnLedOn (RED_PIN_F2_F4);

      break;

    case 5:
      turnAllLedOff ();
      turnLedOn (RED_PIN_1_3);
      turnLedOn (GREEN_PIN_2_4);
      turnLedOn (GREEN_PIN_F1_F3);
      turnLedOn (RED_PIN_F2_F4);

      break;

    case 6:
      turnAllLedOff();
      turnLedOn (YELLOW_PIN_1_3);
      turnLedOn (RED_PIN_1_3);
      turnLedOn (YELLOW_PIN_2_4);
      blinkLed (GREEN_PIN_F1_F3);
      turnLedOn (RED_PIN_F2_F4);

      break;

    case 7:
      turnAllLedOff();
      turnLedOn (GREEN_PIN_1_3);
      turnLedOn (RED_PIN_2_4);
      turnLedOn (RED_PIN_F1_F3);
      turnLedOn (GREEN_PIN_F2_F4);

      break;

    case 8:
      turnAllLedOff();
      turnLedOn (YELLOW_PIN_1_3);
      turnLedOn (YELLOW_PIN_2_4);
      turnLedOn (RED_PIN_2_4);
      turnLedOn (RED_PIN_F1_F3);
      blinkLed (GREEN_PIN_F2_F4);

      break;

    default:
      {
        turnAllLedOff();
        turnLedOn (YELLOW_PIN_1_3);
        turnLedOn (YELLOW_PIN_2_4);
      }

      break;
  }
}


void defaultState()
{
  switch (currentState1)
  {
    case S_1_3_DRIVE_2_4_STOP:
      {
        setTrafficLights(1);
        startTimer(5000);
        changeStateTo1(S_1_3_PREPARE_TO_STOP_2_4_PREPARE_TO_DRIVE);
      }

      break;

    case S_1_3_PREPARE_TO_STOP_2_4_PREPARE_TO_DRIVE:
      if (timerHasExpired())
      {
        setTrafficLights(2);
        startTimer(2000);
        changeStateTo1(S_1_3_STOP_2_4_DRIVE);
      }

      break;

    case S_1_3_STOP_2_4_DRIVE:
      if (timerHasExpired())
      {
        setTrafficLights(3);
        startTimer(5000);
        changeStateTo1(S_1_3_PREPARE_TO_DRIVE_2_4_PREPARE_TO_STOP);
      }

      break;

    case S_1_3_PREPARE_TO_DRIVE_2_4_PREPARE_TO_STOP:
      if (timerHasExpired())
      {
        setTrafficLights(4);
        startTimer(2000);
        changeStateTo1(S_RESET);
      }

      break;

    case S_RESET:
      if (timerHasExpired())
      {
        setTrafficLights(1);
        changeStateTo1(S_1_3_DRIVE_2_4_STOP);
      }

      break;

    default:
      turnAllLedOff();
      turnLedOn (GREEN_PIN_1_3);
      turnLedOn (GREEN_PIN_2_4);

      break;
  }
}



void fotovergang_F1()
{
  switch (currentState2)
  {
    case S_WALK_F1_F3:
      setTrafficLights(5);
      startTimer(5000);
      changeStateTo2(S_STOP_WALKING_F1_F3);


      break;

    case S_STOP_WALKING_F1_F3:
      if (timerHasExpired())
      {
        setTrafficLights(6);
        startTimer(2000);
        changeStateTo2(S_RESET_F1);
      }

      break;

    case S_RESET_F1:
      if (timerHasExpired())
      {
        defaultState();
      }

      break;

    default:
      {
        turnAllLedOff();
        turnLedOn (YELLOW_PIN_1_3);
        turnLedOn (YELLOW_PIN_2_4);
      }

      break;
  }
}


void fotovergang_F4()
{
  switch (currentState3)
  {
    case S_WALK_F2_F4:
      setTrafficLights(7);
      startTimer(5000);
      changeStateTo3(S_STOP_WALKING_F2_F4);


      break;

    case S_STOP_WALKING_F2_F4:
      if (timerHasExpired())
      {
        setTrafficLights(8);
        startTimer(2000);
        changeStateTo3(S_RESET_F4);
      }

      break;

    case S_RESET_F4:
      if (timerHasExpired())
      {
        //startTimer(2000);
        // defaultState(2);
        turnAllLedOff();
        turnLedOn (RED_PIN_1_3);

      }

      break;

    default:
      {
        turnAllLedOff();
        turnLedOn (RED_PIN_1_3);
        turnLedOn (RED_PIN_2_4);
      }

      break;
  }
}


void uteAvDrift()
{
  turnAllLedOff();
  turnLedOn (YELLOW_PIN_1_3);
  turnLedOn (YELLOW_PIN_2_4);
  delay(60000);
}


boolean isButtonPressedF1()
{
  int buttonState = digitalRead(BUTTON_F1_F3);
    boolean buttonPressed;
  if (buttonState == HIGH)
  {
    pedestrianF1 = true;
  }
  else
  {
    pedestrianF1 = false;
  }
  return pedestrianF1;

  Serial.println(pedestrianF1);
}


boolean isButtonPressedF4()
{
  int buttonState = digitalRead(BUTTON_F2_F4);
    boolean buttonPressed;
  if (buttonState == HIGH)
  {
    pedestrianF4 = true;
  }
  else
  {
    pedestrianF4 = false;
  }
  return pedestrianF4;

  Serial.println(pedestrianF4);
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


void startTimer(unsigned long duration)
{
  nextTimeout = millis() + duration;
}


void changeStateTo1(int newState1)
{
  currentState1 = newState1;
}


void changeStateTo2(int newState2)
{
  currentState2 = newState2;
}


void changeStateTo3(int newState3)
{
  currentState3 = newState3;
}

