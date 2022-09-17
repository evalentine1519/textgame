#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 8);
const int startPin = 6;
const int movePin = 2;
const int speedPin = A0;

int startState = 0;
int prevStartState = 0;
int moveState = 0;
int prevMoveState = HIGH;
bool halfway = false;
int topPos = 15;
int bottomPos = 15;
bool top = true;
int score = 0;
bool failed = false;
bool restartScreen = false;
int speedVal = 0;
int convertedSpeed = 0;

//screen data
String enemy;
String player;

//counters
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long updateDelay = 333;
unsigned long lastUpdateTop = 0;
unsigned long lastUpdateBottom = 0;
int gameSpeed = 2;


void setup() {
  lcd.begin(16, 2);
  //Serial.begin(9600);
  pinMode(startPin, INPUT);
  pinMode(movePin, INPUT);
  pinMode(speedPin, INPUT);
  enemy = "*";
  player = ">";
  //attachInterrupt(digitalPinToInterrupt(2), move_ISR, RISING);
}

void loop() {
  // Setup
  //speedVal = analogRead(speedPin);
  //Serial.println(speedVal);
  failed = false;
  restartScreen = false;
  topPos = 15;
  bottomPos = 15;
  score = 0;
  top = true;
  halfway = false; 
  screenWrite("Hold the start", "button to begin");
  speedVal = analogRead(speedPin);
  convertedSpeed = map(speedVal, 0, 1023, 33, 750);
  //screenWrite("Speed is:", String(convertedSpeed));
  
  //Start the game if the right-side button is pressed
  if(digitalRead(startPin) == HIGH){
    updateDelay = convertedSpeed;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(player);
    prevMoveState = LOW;
  while (!failed){
    moveState = digitalRead(movePin);
    if(moveState != prevMoveState){
      if(moveState == LOW){
        top = !top;
        characterUpdate(topPos, bottomPos, top);
      }
      prevMoveState = moveState;
    }
     if((millis() - lastUpdateTop) > updateDelay){
        characterUpdate(topPos, bottomPos, top);
        
        
        topPos--;
        lastUpdateTop = millis();
     }
     if(halfway == true) {
      if((millis() - lastUpdateBottom) > updateDelay){
         characterUpdate(topPos, bottomPos, top);
         bottomPos--;
         lastUpdateBottom = millis();
      }
     }

     //check if top has reached the end (add collision detection later)
     if(topPos == -1){
      topPos = 15;
      if(top){
        screenWrite("Game over!", "Score: ");
        lcd.print(score);
        failed = true;
        while(failed){
          startState = digitalRead(startPin);
          if(startState == HIGH){
             restartScreen = true;
             startState = LOW;
             break;
          }
        }
      } else{
        score++;
      }
     }

     //check if bottom has reached the end (add collision detection later)
     if(bottomPos == -1){
      bottomPos = 15;
      if(!top){
        screenWrite("Game over!", "Score: ");
        lcd.print(score);
        failed = true;
        while(failed){
          startState = digitalRead(startPin);
          if(startState == HIGH){
             restartScreen = true;
             startState = LOW;
             break;
          }
        }
      } else{
        score++;
      }
     }

     while(restartScreen){
      screenWrite("Press start to", "play again");
      while(restartScreen){
        startState = digitalRead(startPin);
        if(startState == HIGH){
          restartScreen = false;
        }
      }
     }

     //start bottom enemy once top has reached position 7
     if (topPos == 8){
      halfway = true;
     }

  }
}
delay(1000);
}

void screenWrite(String firstline, String secondline) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(firstline);
  lcd.setCursor(0,1);
  lcd.print(secondline);
}

void characterUpdate(int topPos, int bottomPos, bool top) {
  lcd.clear();
  if(top){
    lcd.setCursor(0,0);
    lcd.print(player);
  } else{
    lcd.setCursor(0,1);
    lcd.print(player);
  }

  lcd.setCursor(topPos, 0);
  lcd.print(enemy);
  if(halfway){
    lcd.setCursor(bottomPos, 1);
    lcd.print(enemy);
  }
  
}

void movePlayer(){
  top = !top;
  lcd.clear();
  if(top == true){
    lcd.setCursor(0,0);
  } else{
    lcd.setCursor(0,1);
  }
  lcd.print(player);
  lcd.setCursor(topPos, 0);
  lcd.print(enemy);
  if(halfway){
  lcd.setCursor(bottomPos, 1);
  lcd.print(enemy);
  }
  delay(5);
}
