#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <VarSpeedServo.h>

const int pingPin = 13;
int inPin = 12;
int buttonPin1 = 2;
int buttonPin2 = 3;
int LDR = A0;
int LDRReading = 0;
int led1 = 6;
int led2 = 7;
int buzzer = 8;
int buttonState = 0;
int emergencybuttonState = 0;
int count_value = 0;
int prestate = 0;
int lastState = HIGH;
int currentState;
String outputstate = "";
bool daydoor = false;
bool nightdoor = false;
bool stopdoor = false;
bool flooddor = false;
bool isOpen = false;
const int servoPin1 = 9;
int speed = 5;
VarSpeedServo myservo1;
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pingPin, OUTPUT);
  pinMode(inPin, INPUT);
  lcd.begin();
  lcd.backlight();
  myservo1.attach(servoPin1);
}

void loop()
{

  outputstate = " ";
  check();
  updatelcd();
  waitforcom();


}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}

void check() {
  long duration, cm;
  currentState = digitalRead(buttonPin1);
  LDRReading = analogRead(LDR);

  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  duration = pulseIn(inPin, HIGH);

  cm = microsecondsToCentimeters(duration);
  if (cm < 10) {
    outputstate += " flood! ";
    flooddor = true;
  }
  else if (cm >= 10) {
    flooddor = false;
  }
  if (LDRReading > 300) {
    outputstate += " Night! ";
    nightdoor = true;
    daydoor = false;
  }
  else {
    outputstate += " Day! ";
    daydoor = true;
    nightdoor = false;
  }
}

void updatelcd() {
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("state:    ");
  lcd.setCursor(0, 2);
  lcd.print(outputstate);
  delay(200);
}

void waitforcom() {
  buttonState = digitalRead(buttonPin1);

  if (buttonState == HIGH && prestate == 0) {
    count_value++;
    delay(100);
    prestate = 1;
  } else if (buttonState == LOW) {
    prestate = 0;
  }
  if (count_value % 2 == 1) {
    checkenvi();
  }
  else if (count_value % 2 == 0) {
    checkenvi();
  }

  if (flooddor) {
    opendoor50();
    //led buzzer
    isOpen = true;
  }
}


void checkenvi() {

  if (isOpen) {
    closedoor();
    isOpen = false;
  }
  else if (!isOpen && daydoor && !nightdoor) {
    opendoor100();
    isOpen = true;
  }
  else if (!isOpen && !daydoor && nightdoor) {
    opendoor50();
    //led
    isOpen = true;
  }
}

void opendoor50() {
  //servohere
}

void opendoor100() {
  //servohere
}

void closedoor() {
  //servohere
}
