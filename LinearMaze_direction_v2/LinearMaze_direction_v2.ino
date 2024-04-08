
/* 
 *  Sofware for linear maze with one or two ports rewarded...
 *  
 *  Ver 1.0, 06/2023
 *  BuzsakiLab-AA
 */

#include <MsTimer2.h> // // Extrict control time library

// USER VARIABLES
int caudalR = 20; // time solenoid is open, in ms
int caudalL = 20; // time solenoid is open, in ms


// IO P
const int syncPin = 7; // For Intan/ Camera sync
const int valve1 = 40; // For solenoid 1 (long IR/right)
const int valve7 = 52; // For solenoid 7 (short IR/left)

const int IRsensorR = 22; // IR sensor left (port 1)
const int IRsensorL = 24; // IR sensor right (port 7)

const int valve1Intan = 26; // output for intan solenoid 1
const int valve7Intan = 38; // output for intan solenoid 7

// const int sensRIntan = 27;
// const int sensLIntan = 39;

boolean valveLeftState = 0; // Valve left on/off

// state variables
int previousTrial = 0;
const boolean aMouse = HIGH; 
const boolean noMouse = LOW;
int onePort = 1; // 0 - two ports are open 
                 // 1 - one port is open 
int state = -1; // 0  is starting a new trial

long int timeCounter = 0;
int rTrial = 0;
int lTrial = 0;
int trialCount = 0;
int blocksCount = 2;
int block = 0;
int trialsPerBlock = 10;



 
void setup() {
  // OUTPUTS
  pinMode(valve1, OUTPUT); 
  pinMode(valve7, OUTPUT);
  pinMode(valve1Intan, OUTPUT); digitalWrite(valve1Intan, LOW);
  pinMode(valve7Intan, OUTPUT); digitalWrite(valve7Intan, LOW);
//  pinMode(sensRIntan, OUTPUT); digitalWrite(sensRIntan, LOW);
//  pinMode(sensLIntan, OUTPUT); digitalWrite(sensLIntan, LOW);

  // INPUTS
  pinMode(IRsensorL,INPUT); //IR1 input 
  pinMode(IRsensorR,INPUT); //IR2 input
  
  Serial.begin(9600); //Start serial communication boud rate at 9600
  Serial.flush();

  // MsTimer setup, running every 1000 ms
  MsTimer2::set(2000,flash); // run flash every 1000 ms
  MsTimer2::start(); //enable the interrupt
}

void loop() {
  if (blocksCount == 2){
    trialsPerBlock = 40;
    // One port rewarded (RIGHT - short sensor)
    if (trialCount <= trialsPerBlock){
      block = 1;
      if (digitalRead(IRsensorL)==aMouse){ // mouse left, do not reward left 
        if (previousTrial == 0 || previousTrial == 1){ // if first trial or previously in right
          digitalWrite(valve7Intan, HIGH); delay(caudalR); digitalWrite(valve7Intan, LOW);
          lTrial = lTrial + 1;
          previousTrial = 2;
          trialCount = lTrial;
        }
      }
      if (digitalRead(IRsensorR)==noMouse){// Mouse go right, reward right
        if (previousTrial == 0 || previousTrial == 2){ // if first trial or previously in left
          digitalWrite(valve1Intan, HIGH);
          digitalWrite(valve1, HIGH); delay(caudalR); digitalWrite(valve1, LOW);
          digitalWrite(valve1Intan, LOW);
          rTrial = rTrial + 1;
          previousTrial = 1;
          trialCount = lTrial;
        }
      }
    }
    // Two ports rewarded 
    else if (trialCount > trialsPerBlock-1){
      block = 2;
      if (digitalRead(IRsensorL)==aMouse){// Mouse go left
        if (previousTrial == 0 || previousTrial == 1){ // if first trial or previously in right
          digitalWrite(valve7Intan, HIGH);
          digitalWrite(valve7, HIGH); delay(caudalL); digitalWrite(valve7, LOW);
          digitalWrite(valve7Intan, LOW);
          lTrial = lTrial + 1;
          previousTrial = 2;
          trialCount = lTrial;
        }
      }

      if (digitalRead(IRsensorR)==noMouse){// Mouse go right
        if (previousTrial == 0 || previousTrial == 2){ // if first trial or previously in left
          digitalWrite(valve1Intan, HIGH);
          digitalWrite(valve1, HIGH); delay(caudalR); digitalWrite(valve1, LOW);
          digitalWrite(valve1Intan, LOW);
          rTrial = rTrial + 1;
          previousTrial = 1;
          trialCount = lTrial;
        }
      }
    }
  }
  else if (blocksCount == 3){
    trialsPerBlock = 40;
    // One port rewarded (RIGHT - short sensor)
    if (trialCount <= trialsPerBlock){
      block = 1;
      if (digitalRead(IRsensorL)==aMouse){ // mouse left, do not reward left 
        if (previousTrial == 0 || previousTrial == 1){ // if first trial or previously in right
          digitalWrite(valve7Intan, HIGH); delay(caudalR); digitalWrite(valve7Intan, LOW);
          lTrial = lTrial + 1;
          previousTrial = 2;
          trialCount = lTrial;
        }
      }
      if (digitalRead(IRsensorR)==noMouse){// Mouse go right, reward right
        if (previousTrial == 0 || previousTrial == 2){ // if first trial or previously in left
          digitalWrite(valve1Intan, HIGH);
          digitalWrite(valve1, HIGH); delay(caudalR); digitalWrite(valve1, LOW);
          digitalWrite(valve1Intan, LOW);
          rTrial = rTrial + 1;
          previousTrial = 1;
          trialCount = lTrial;
        }
      }
    }
    // Two ports rewarded 
    else if ((trialCount > trialsPerBlock) & (trialCount <= 2*trialsPerBlock)){
      block = 2;
      if (digitalRead(IRsensorL)==aMouse){// Mouse go left
        if (previousTrial == 0 || previousTrial == 1){ // if first trial or previously in right
          digitalWrite(valve7Intan, HIGH);
          digitalWrite(valve7, HIGH); delay(caudalL); digitalWrite(valve7, LOW);
          digitalWrite(valve7Intan, LOW);
          lTrial = lTrial + 1;
          previousTrial = 2;
          trialCount = lTrial;
        }
      }

      if (digitalRead(IRsensorR)==noMouse){// Mouse go right
        if (previousTrial == 0 || previousTrial == 2){ // if first trial or previously in left
          digitalWrite(valve1Intan, HIGH);
          digitalWrite(valve1, HIGH); delay(caudalR); digitalWrite(valve1, LOW);
          digitalWrite(valve1Intan, LOW);
          rTrial = rTrial + 1;
          previousTrial = 1;
          trialCount = lTrial;
        }
      }
    }
    // One port rewarded 
    else{
      block = 3;
      if (digitalRead(IRsensorL)==aMouse){ // mouse left, do not reward left 
        if (previousTrial == 0 || previousTrial == 1){ // if first trial or previously in right
          digitalWrite(valve7Intan, HIGH); delay(caudalR); digitalWrite(valve7Intan, LOW);
          lTrial = lTrial + 1;
          previousTrial = 2;
          trialCount = lTrial;
        }
      }
      if (digitalRead(IRsensorR)==noMouse){// Mouse go right, reward right
        if (previousTrial == 0 || previousTrial == 2){ // if first trial or previously in left
          digitalWrite(valve1Intan, HIGH);
          digitalWrite(valve1, HIGH); delay(caudalR); digitalWrite(valve1, LOW);
          digitalWrite(valve1Intan, LOW);
          rTrial = rTrial + 1;
          previousTrial = 1;
          trialCount = lTrial;
        }
      }
    }
  }  
}


void flash(){
  Serial.print(timeCounter);
  timeCounter = timeCounter + 2;
  Serial.print("s, right trials: ");
  Serial.print(rTrial);
  Serial.print(", left trials: ");
  Serial.print(lTrial);
  Serial.print(", total trials: ");
  Serial.print(trialCount);
  Serial.print(", block: ");
  Serial.println(block);
}
