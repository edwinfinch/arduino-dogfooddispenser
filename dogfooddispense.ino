/*
Dog Food Dispenser
Edwin Finch
Open source, of course :)

#sixseasonsandamovie
*/

#include <Servo.h>

#define SWITCH 13
#define BUTTON 12
#define RGB_RED 11
#define RGB_GREEN 10
#define RGB_BLUE 9
#define LED 6
#define SERVO 5
#define PIEZO 3
#define POT 2

int meal;
int currentMeal;
boolean requestedAlready;
boolean buttonPressed;
boolean previousTimeSet;
unsigned long previousTime;

Servo myservo;

void beep(unsigned char delayms){
  analogWrite(PIEZO, 200);
  delay(delayms);
  analogWrite(PIEZO, 0);
  delay(delayms); 
}

void setup()
{
  pinMode(SWITCH, INPUT);
  pinMode(BUTTON, INPUT);
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(PIEZO, OUTPUT);
  pinMode(POT, INPUT);
  Serial.begin(9600);
  Serial.println("==============BOOTING...=============");
  delay(500); //I just like the effect of making it look like there's actually something happening behind the lag.
  Serial.println("Dog Food Dispenser");
  delay(500);
  Serial.println("By Edwin Finch");
  delay(500);
  Serial.println("==============BOOTED.================");
  
  beep(1000);
  
  myservo.attach(5);
  myservo.write(0);
}

void servoCall(int value){
  if(value == 30){
    int i = 6;
    //To prevent the food shooting high up into the air, slow the acceleration down
  while(i > 0){
      myservo.write(value - (i*5));
      Serial.print("Servo written to point: "); 
      Serial.print(value - (i*5));
      Serial.println("/180");
      delay(50);
      i--;
    }
  }
  else{
    myservo.write(value);
    Serial.print("Servo written to point: "); 
    Serial.print(value);
    Serial.println("/180");
  }
}

void mealRequest(int meal){
  int betweenSlices = 11000;
    if(requestedAlready == 0){
      requestedAlready = 1;
      Serial.print("Recieved request for meal, with meal value of ");
      Serial.println(meal);
      digitalWrite(RGB_RED, HIGH);
      digitalWrite(RGB_GREEN, LOW);
      digitalWrite(RGB_BLUE, LOW);
      if(meal == 1){
        Serial.println("Meal slice! Sending servo request.");
        Serial.println("==================BEGIN==================");
        delay(100);
        beep(1000);
        servoCall(30);
        delay(1000);
        servoCall(0);
        Serial.println("===================END===================");
        requestedAlready = 0;
      }
      else if(meal == 2){
        Serial.println("Half meal! Sending servo request.");
        Serial.println("==================BEGIN==================");
        delay(100);
        beep(100);
        beep(100);
        servoCall(30);
        delay(100);
        servoCall(0);
        delay(betweenSlices);
        servoCall(30);
        delay(1000);
        servoCall(0);
        Serial.println("===================END===================");
        requestedAlready = 0;
      }
      else if(meal == 3){
        Serial.println("Full meal! Sending servo request.");
        Serial.println("==================BEGIN==================");
        delay(100);
        beep(100);
        beep(100);
        beep(100);
        servoCall(30);
        delay(50);
        servoCall(0);
        delay(betweenSlices);
        servoCall(30);
        delay(50);
        servoCall(0);
        delay(betweenSlices);
        servoCall(30);
        delay(50);
        servoCall(0);
        Serial.println("===================END===================");
        requestedAlready = 0;
      }
   }
   else if(requestedAlready == 1){
     Serial.println("Error: Already requested!");
   }
}

void loop()
{
  if(digitalRead(SWITCH) == 1){
    int potValue = analogRead(POT);
    unsigned long currentTime = millis();
    
    if(potValue > -1 && potValue < 361){
      digitalWrite(RGB_RED, HIGH);
      digitalWrite(RGB_GREEN, HIGH);
      digitalWrite(RGB_BLUE, HIGH);
      meal = 1;
    }
    else if(potValue > 360 && potValue < 660){
      digitalWrite(RGB_RED, LOW);
      digitalWrite(RGB_GREEN, LOW);
      digitalWrite(RGB_BLUE, HIGH);
      meal = 2;
    }
    else if(potValue > 660){
      digitalWrite(RGB_RED, LOW);
      digitalWrite(RGB_GREEN, HIGH);
      digitalWrite(RGB_BLUE, LOW);
      meal = 3;
    }
    
    if(digitalRead(BUTTON) == 1){
      if(buttonPressed == 0){
        Serial.println("Sending request for meal.");
        mealRequest(meal);
        buttonPressed = 1;
      }
    }
    else if(digitalRead(BUTTON) == 0){
      buttonPressed = 0;
    }
  }
  else if(digitalRead(SWITCH) == 0){
      digitalWrite(RGB_RED, LOW);
      digitalWrite(RGB_GREEN, LOW);
      digitalWrite(RGB_BLUE, LOW);
  }
}
