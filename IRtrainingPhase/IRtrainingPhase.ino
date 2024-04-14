 //For this, do we want the mice to just roam around and touch the left, right, and home IR or do we also want to train them so that
//they know that poking the door IR will open the door?
#include <Servo.h>
Servo myServo;

const int openDoor = -270;

const int doorLeft = 15;
const int doorRight = 16;
const int doorBase = 17;

Servo servo1;
Servo servo2;
Servo servo3;

int phase = 1; //1 mean the water will come out of solenoid as long as IR bream is broken. phase 2 will mean the water will only come out for a specific amount of time and will eventually stop. Then there will be a cooldown

const int IRbase = 30;
const int IRleft = 32;
const int IRright = 33;

const int solLeft = 48;
const int solBase = 50;
const int solRight = 52;


void setup() {
  pinMode(IRbase, INPUT);
  pinMode(IRleft, INPUT);
  pinMode(IRright, INPUT);

  pinMode(solLeft, OUTPUT);
  pinMode(solRight, OUTPUT);
  pinMode(solBase, OUTPUT);

  servo1.attach(doorLeft);
  servo2.attach(doorRight);
  servo3.attach(doorBase);

  Serial.begin(9600);
  Serial.flush();
}

void loop() {
  servo1.write(openDoor);
  servo2.write(openDoor);
  servo3.write(openDoor);

  if (phase == 1) {
    if (digitalRead(IRbase) == HIGH) {
      digitalWrite(solBase, HIGH);
    } else {
      digitalWrite(solBase, LOW);
    }
    if (digitalRead(IRleft) == HIGH) {
      digitalWrite(solLeft, HIGH);
    } else {
      digitalWrite(solLeft, LOW);
    }
    if (digitalRead(IRright) == HIGH) {
      digitalWrite(solRight, HIGH);
    } else {
      digitalWrite(solRight, LOW);
    }
  }
  if (phase == 2) {
    if (digitalRead(IRbase) == HIGH) {
      digitalWrite(solBase, HIGH); delay(80); digitalWrite(solBase, LOW);              //water will be given for 0.5 seconds and then stop. Cooldown is 6 seconds
      delay(2000);
    }
        if(digitalRead(IRleft) == HIGH) {
        digitalWrite(solLeft, HIGH); delay(50); digitalWrite(solLeft, LOW);
        delay(2000);
        }
    if (digitalRead(IRright) == HIGH) {
      digitalWrite(solRight, HIGH); delay(80); digitalWrite(solRight, LOW);
      delay(2000);
    }
  }

}
