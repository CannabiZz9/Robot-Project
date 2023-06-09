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
int buzzer = 9;
int buttonState = 0;
int buttonState1 = 0;
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
const int servoPin1 = 8;
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

  if (cm <= 6) {
    outputstate += " flood! ";
    flooddor = true;
  }
  else if (cm > 6) {
    flooddor = false;
  }
  if (LDRReading < 300) {
    outputstate += " Night! ";
    nightdoor = true;
    daydoor = false;
    digitalWrite(led1, HIGH);
  }
  else {
    outputstate += " Day! ";
    daydoor = true;
    nightdoor = false;
    digitalWrite(led1, LOW);
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
    Serial.println("Button press");
    checkenvi();
    delay(100);
    prestate = 1;
  } else if (buttonState == LOW) {
    prestate = 0;
  }
  if (flooddor && !isOpen) {
    digitalWrite(led2, HIGH);
    opendoor50();
    digitalWrite(led2, LOW);
    delay(50);
    digitalWrite(led2, HIGH);
    buzz();
    digitalWrite(led2, LOW);
    delay(50);
    digitalWrite(led2, HIGH);
    isOpen = true;
  }
  else if (flooddor && isOpen) {
    digitalWrite(led2, HIGH);
    digitalWrite(led2, LOW);
    delay(50);
    digitalWrite(led2, HIGH);
    buzz();
    digitalWrite(led2, LOW);
    delay(50);
    digitalWrite(led2, HIGH);
  }
  else if (!flooddor && isOpen) {
    digitalWrite(buzzer, LOW);
    digitalWrite(led2, LOW);
  }
  else if (!flooddor && !isOpen) {
    digitalWrite(buzzer, LOW);
    digitalWrite(led2, LOW);
  }
}


void checkenvi() {
  if (isOpen) {
    closedoor();
    isOpen = false;
    Serial.println("closedoor");
  }
  else if (!isOpen && daydoor && !nightdoor) {
    opendoor100();
    isOpen = true;
    Serial.println("opendoor100");
  }
  else if (!isOpen && !daydoor && nightdoor) {
    opendoor50();
    isOpen = true;
    Serial.println("opendoor50");
  }
}

void opendoor50() {
  int i =0;
  while (buttonState1 != 1){
    buttonState1 = digitalRead(buttonPin2);
    if(buttonState1 == 1){
      Serial.println("Emergency! Button press");
      outputstate = "reset to continue           Emergency!";
      updatelcd();
      myservo1.write(90, 100);
      do{
        digitalWrite(led1, LOW);
        digitalWrite(led2, HIGH);
        delay(500);
        digitalWrite(led2, LOW);
        delay(500);
      }while(i < 99999999);
    }
    else{
      if(i < 7){
        myservo1.write(180, 7);
        delay(500);
        myservo1.write(90, 10);
        delay(500);
        i++;
      }
      else{
        break; } 
    }
  }
}

void opendoor100() {
  int i =0;
  while (buttonState1 != 1){
    buttonState1 = digitalRead(buttonPin2);
    if(buttonState1 == 1){
      Serial.println("Emergency! Button press");
      outputstate = "reset to continue           Emergency!";
      updatelcd();
      myservo1.write(90, 100);
      do{
        digitalWrite(led1, LOW);
        digitalWrite(led2, HIGH);
        delay(500);
        digitalWrite(led2, LOW);
        delay(500);
      }while(i < 99999999);
    }
    else{
      if(i < 9){
        myservo1.write(180, 8);
        delay(500);
        myservo1.write(90, 10);
        delay(500);
        i++;
      }
      else{ 
        break; } 
    }
  }
}

void closedoor() {
  int i =0;
  while (buttonState1 != 1){
    buttonState1 = digitalRead(buttonPin2);
    if(buttonState1 == 1){
      Serial.println("Emergency! Button press");
      outputstate = "reset to continue           Emergency!";
      updatelcd();
      myservo1.write(90, 100);
      do{
        digitalWrite(led1, LOW);
        digitalWrite(led2, HIGH);
        delay(500);
        digitalWrite(led2, LOW);
        delay(500);
      }while(i < 99999999);
    }
    else{
      if(i < 8){
        myservo1.write(0, 7);
        delay(500);
        myservo1.write(90, 10);
        delay(500);
        i++;
      }
      else{
        break; } 
    }
  }
}



#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0


// change this to make the song slower or faster
int tempo = 140;


// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Happy Birthday
  // Score available at https://musescore.com/user/8221/scores/26906

  NOTE_C4, 4, NOTE_C4, 8,
  NOTE_D4, -4, NOTE_C4, -4, NOTE_F4, -4,
  NOTE_E4, -2, NOTE_C4, 4, NOTE_C4, 8,
  NOTE_D4, -4, NOTE_C4, -4, NOTE_G4, -4,
  NOTE_F4, -2, NOTE_C4, 4, NOTE_C4, 8,

  NOTE_C5, -4, NOTE_A4, -4, NOTE_F4, -4,
  NOTE_E4, -4, NOTE_D4, -4, NOTE_AS4, 4, NOTE_AS4, 8,
  NOTE_A4, -4, NOTE_F4, -4, NOTE_G4, -4,
  NOTE_F4, -2,

};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

void buzz() {
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 0.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}
