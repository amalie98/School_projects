const int GREEN_PIN = 8;
const int YELLOW_PIN = 9;
const int RED_PIN = 10;
const int PUSH_BUTTON = 13;

const int S_CARS_DRIVE = 0; //disse skal være i switch()
const int S_PREPARE_TO_DRIVE = 1;
const int S_CARS_STOP = 2;
const int S_PREPARE_TO_STOP = 3;

int state = S_CARS_DRIVE;

unsigned long nextTimeout; 
//.......................................................................

void setTrafficLights(int phase)
{
  if(phase == 1)
  {
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  }
  if(phase == 2)
  {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
  }
  if(phase == 3)
  {
   digitalWrite(RED_PIN, LOW);
   digitalWrite(YELLOW_PIN, LOW);
   digitalWrite(GREEN_PIN, HIGH); 
  }
  if(phase == 4)
  {
   digitalWrite(RED_PIN, LOW);
   digitalWrite(YELLOW_PIN, HIGH);
   digitalWrite(GREEN_PIN, LOW); 
  }
}

boolean isButtonPressed() //kan gjøre endringer på false/true
{
  int buttonState = digitalRead(PUSH_BUTTON);

  boolean buttonPressed;
  if(buttonState == HIGH)
  {
    buttonPressed = true;
  }
  else
  {
    buttonPressed = false;
  }
  return buttonPressed;
}

void startTimer(unsigned long duration)
{
  nextTimeout = millis() + duration;
}

boolean timerHasExpired()
{
  boolean hasExpired = false;
  if(millis() > nextTimeout)
  {
    hasExpired = true;
  }
  else
  {
    hasExpired = false;
  }
  return hasExpired;
}

//-------------------------------------------------------------------------
void setup() {
  
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);

  pinMode(PUSH_BUTTON, INPUT);

  Serial.begin(9600);
}
//---------------------------------------------------------------------------
void loop() {

Serial.println(isButtonPressed());

  switch(state)
  {
    case S_CARS_DRIVE: 
    {
     setTrafficLights(3);
     if(isButtonPressed() == true)
     {
      setTrafficLights(4);
      startTimer(2000);
      state = S_PREPARE_TO_STOP;
     }     
    }
    break;
  
    case S_PREPARE_TO_STOP:
    {
     setTrafficLights(4);
     if(timerHasExpired())
     {
      setTrafficLights(1);
      startTimer(5000);
      state = S_CARS_STOP;
     }
    }
    break;

    case S_CARS_STOP:
    {
     setTrafficLights(1);
     if(timerHasExpired())
     {
      setTrafficLights(2);
      startTimer(1000);
      state = S_PREPARE_TO_DRIVE;
     }
    }
    break;

    case S_PREPARE_TO_DRIVE:
    {
     setTrafficLights(2);
     if(timerHasExpired())
     {
      setTrafficLights(3);
      state = S_CARS_DRIVE;
     }
    }
    break; 
  }
}

