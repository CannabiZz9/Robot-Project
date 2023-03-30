# Robot-Project

const int pingPin = 13;    //trig pin ultrasonic -> digital 13
int inPin = 12;            //echo pin ultrasonic -> digital 12
int buttonPin1 = 2;        //button open door pin -> digital 2
int buttonPin2 = 3;        //button EMERGENCY! pin -> digital 3
int LDR = A0;              //Light sensor pin -> analog 0
int LDRReading = 0;           //value
int led = 6;               //led night pin -> digital 6
int led = 7;               //led EMERGENCY! pin -> digital 7
int buzzer = 8;            //buzzer pin -> digital 8
int buttonState = 0;          //
int count_value =0;           //value
int prestate =0;              //value
int lastState = HIGH;         //value
int currentState;             //value
String outputstate = "";      //value
bool daydoor = false;         //value
bool nightdoor = false;       //value
bool stopdoor = false;        //value
bool flooddor = false;        //value
const int servoPin1 = 9;   //servo pin -> digital 9
