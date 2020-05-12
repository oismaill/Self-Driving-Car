//XPERT MIND.
//This program is used to control a robot using a app that communicates with Arduino through a bluetooth module.
//Error Code Chart: Code 01; Turnradius is higher than Speed; Code 02; Speed is higher than 255;
#define in1 5 //L298n Motor Driver pins.
#define in2 6
#define in3 10
#define in4 11
#define LED 13
long duration, cm;
const int pingPin_Trig = 7;
const int pingPin_Echo = 13;
int command; //Int to store app command state.
int Speed = 204; // 0 - 255.
int Speedsec;
int Right=190;
int Left=190;
int buttonState = 0;
int lastButtonState = 0;
int Turnradius = 0; //Set the radius of a turn, 0 - 255 Note:the robot will malfunction if this is higher than int Speed.
int brakeTime = 45;
int brkonoff = 1; //1 for the electronic braking system, 0 for normal.
void setup() {
  pinMode(pingPin_Trig, OUTPUT);
  pinMode(pingPin_Echo, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(LED, OUTPUT); //Set the LED pin.
  Serial.begin(9600);  //Set the baud rate to your Bluetooth module.
}

void loop() {
  delay(2000);
  digitalWrite(pingPin_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin_Trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin_Trig, LOW);
  duration = pulseIn(pingPin_Echo, HIGH);
  cm = microsecondsToCentimeters(duration);
  
  if (Serial.available() > 0) {
    command = Serial.read();
    Stop(); //Initialize with motors stoped.
    switch (command) {
      case 'F':
        Forward();
        break;
      case 'L':
        Left+=10;
        Leftt();
        break;
      case 'R':
        Right+=10;
        Rightt();
        break;
      case 'O':
        Stop();
        break;
    }
    Speedsec = Turnradius;
    if (brkonoff == 1) {
      brakeOn();
    } else {
      brakeOff();
    }
  }
}

void forward() {
  analogWrite(in1, 100);
  analogWrite(in3, 100);
}

void back() {
  analogWrite(in2, 0);
  analogWrite(in4, 0);
}

void leftt() {
  analogWrite(in3, 95);
  analogWrite(in1, 70);
}

void right() {
  analogWrite(in3, 100);
  analogWrite(in1, 200);
}

void Forward(){
  analogWrite(in1, 190);
  analogWrite(in3, 190);
}
void Leftt(){
  analogWrite(in3, Left);
  analogWrite(in1, Right);
}
void Rightt(){
  analogWrite(in3, Left);
  analogWrite(in1, Right);
}
void Stop()  {
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}

void brakeOn() {
  //Here's the future use: an electronic braking system!
  // read the pushbutton input pin:
  buttonState = command;
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (lastButtonState == 'F') {
      if (buttonState == 'S') {
        back();
        delay(brakeTime);
        Stop();
      }
    }
    if (lastButtonState == 'B') {
      if (buttonState == 'S') {
        forward();
        delay(brakeTime);
        Stop();
      }
    }
    if (lastButtonState == 'L') {
      if (buttonState == 'S') {
        right();
        delay(brakeTime);
        Stop();
      }
    }
    if (lastButtonState == 'R') {
      if (buttonState == 'S') {
        leftt();
        delay(brakeTime);
        Stop();
      }
    }
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState = buttonState;
}
void brakeOff() {

}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

