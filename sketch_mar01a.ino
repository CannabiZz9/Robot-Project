#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <VarSpeedServo.h> 

const int pingPin = 13;
int inPin = 12;
int buttonPin = 2;
int LDR = A0;  
int LDRReading = 0;
int led = 6;
int buzzer = 7;
int buttonState = 0;       
int count_value =0;
int prestate =0;
int lastState = HIGH;
int currentState;
int temp =0;
String outputstate = "";
bool daydoor = false;
bool nightdoor = false;
bool stopdoor = false;
bool flooddor = false;
const int servoPin1 = 8;
int speed = 5;
VarSpeedServo myservo1;
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
Serial.begin(9600);
pinMode(buttonPin, INPUT_PULLUP);
pinMode(led, OUTPUT);
pinMode(buzzer, OUTPUT);
lcd.begin();
lcd.backlight();
myservo1.attach(servoPin1);  
}

void loop()
{
long duration, cm;
currentState = digitalRead(buttonPin);
LDRReading = analogRead(LDR);
pinMode(pingPin, OUTPUT);
digitalWrite(pingPin, LOW);
delayMicroseconds(2);
digitalWrite(pingPin, HIGH);
delayMicroseconds(5);
digitalWrite(pingPin, LOW);
pinMode(inPin, INPUT);
duration = pulseIn(inPin, HIGH);

cm = microsecondsToCentimeters(duration);
outputstate = " ";

buttonState = digitalRead(buttonPin);

  
  if (buttonState == HIGH && prestate == 0) {
    count_value++;
    delay(100);
    prestate = 1;
  } else if(buttonState == LOW) {
    prestate = 0;
  }


if(cm < 10)
    outputstate += " flood! ";
    

if (LDRReading >300){                  
  outputstate += " Night! ";
  }
  else{
  outputstate += " Day! ";
}
if(count_value % 2 == 1){
  outputstate += " Emergency! ";
  
  
}
myservo1.write(90,speed); 
delay(1000);
myservo1.write(0,speed); 
delay(1000);
myservo1.write(90,speed);
delay(3000);
lcd.clear(); // ล้างหน้าจอ
Serial.println(outputstate);
Serial.println(count_value);
Serial.println(cm);
lcd.setCursor(5, 0);
lcd.print("state:     ");
lcd.setCursor(0, 2);
lcd.print(outputstate);
delay(800);

}

long microsecondsToCentimeters(long microseconds)
{
return microseconds / 29 / 2;
}
