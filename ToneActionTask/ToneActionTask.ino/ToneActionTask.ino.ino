#include <toneAC.h>

/* ==================== PINS ============================================*/
//Solenoid Pins
const int solPin1 = 40;  // Ouputpin for close circuit on transistor - #1
const int solPin3 = 44;  // Ouputpin for close circuit on transistor - #3
const int solPin5 = 48;  // Ouputpin for close circuit on transistor - #5
const int solPin7 = 52;  // Ouputpin for close circuit on transistor - #7

//IR pins
const int lickPin1 = 41;  // Input pin for IR #1
const int lickPin3 = 45;  // Input pin for IR #3
const int lickPin5 = 49;  // Input pin for IR #5
const int lickPin7 = 53;  // Input pin for IR #7

const int positionAudioPin = 6;

/* ==============INTAN PINS ============================================*/

//Solenoid Pins
const int solTTLPin1 = 26;  // Ouputpin for close circuit on transistor - #1
const int solTTLPin3 = 30; // Ouputpin for close circuit on transistor - #3
const int solTTLPin5 = 34;  // Ouputpin for close circuit on transistor - #5
const int solTTLPin7 = 38;  // Ouputpin for close circuit on transistor - #7

//Capacitance pins
const int lickTTLPin1 = 27;  // Input pin for touch state - #1
const int lickTTLPin3 = 31;  // Input pin for touch state - #3
const int lickTTLPin5 = 35;  // Input pin for touch state - #5
const int lickTTLPin7 = 37;  // Input pin for touch state - #7

// Speaker pins
const int toneTTLPin1 = 32; // High if low tone trial
const int toneTTLPin2 = 36; // High if med tone trial
const int toneTTLPin3 = 38; // High if high tone trial
const int toneTTLPin4 = 28; // High if high tone trial

const int trialTTLPin = 25;  //Sends a TTL at the end of each trial

/*==================== SETTINGS ========================================================*/

//Variables to change
int randSeedNumber = 1789;  //input any random digits
int training = 1;           //Flag for training. If 0, water delivery happens ONLY after a lick is detected on the correct port
int probeTrials = 0;        // Flag for whether to include probe trials - here no tone plays, allowing for "uncertainty"
int probeTrialProb = 20;    // Probability of probe trials if probeTrials = 1
int cueDur = 500;

// Dont change anything below
int freq[3] = {4000, 8000, 14000};
int lickmeterState1 = 0;
int lickmeterState3 = 0;
int lickmeterState5 = 0;
int lickmeterState7 = 0;

float cur_pos = 0;  // position from Bonsai
int state = 3;           // 0 - about to play cue, 1 - after cue, 2 - waiting for response, 3 - waiting for home lick 
int randNumber = 0;

int solenoidDur = 30;        // in ms
int timeOutDur = 60 * 1000;  // 60 seconds
unsigned long trialCountDown;
unsigned long currentMillis;
unsigned long cueMillis;
unsigned long startMillis;
int cueON = 0;

int countTrials = 0;
int correctTrials = 0;
int numLow = 0;  
int numMed = 0;
int numHigh = 0;

int wrongTrial = 0;
int rewardStatus = 0;
int probeTrialCur = 0;
int probeTrialPrev = 0;

void setup() {
  // initialize serial communications (for debugging only):
  Serial.begin(57600);
  randomSeed(randSeedNumber);

  pinMode(lickPin1, INPUT);  // set pin as INPUT
  pinMode(lickPin5, INPUT);  // set pin as INPUT
  pinMode(lickPin7, INPUT);  // set pin as INPUT

  pinMode(solPin1, OUTPUT);  // set pin as OUTPUT
  pinMode(solPin3, OUTPUT);  // set pin as OUTPUT
  pinMode(solPin5, OUTPUT);  // set pin as OUTPUT
  pinMode(solPin7, OUTPUT);  // set pin as OUTPUT

  pinMode(toneTTLPin1, OUTPUT);  // set pin as OUTPUT
  pinMode(toneTTLPin2, OUTPUT);  // set pin as OUTPUT
  pinMode(toneTTLPin3, OUTPUT);  // set pin as OUTPUT
  pinMode(toneTTLPin4, OUTPUT);  // set pin as OUTPUT

  pinMode(solTTLPin1, OUTPUT);  // set pin as OUTPUT
  pinMode(solTTLPin5, OUTPUT);  // set pin as OUTPUT
  pinMode(solTTLPin7, OUTPUT);  // set pin as OUTPUT

  pinMode(lickTTLPin1, OUTPUT);  // set pin as OUTPUT
  pinMode(lickTTLPin5, OUTPUT);  // set pin as OUTPUT
  pinMode(lickTTLPin7, OUTPUT);  // set pin as OUTPUT

  pinMode(trialTTLPin, OUTPUT);  // set pin as OUTPUT

  pinMode(positionAudioPin, OUTPUT);  //
  startMillis  = millis();
}

void loop() {

  // read position from Bonsai
  if (Serial.available() > 0) {
    cur_pos = Serial.parseFloat();
  } else {

    lickmeterState1 = digitalRead(lickPin1);
    lickmeterState5 = digitalRead(lickPin5);
    lickmeterState7 = digitalRead(lickPin7);

    digitalWrite(lickTTLPin1, lickmeterState1);
    digitalWrite(lickTTLPin5, lickmeterState5);
    digitalWrite(lickTTLPin7, lickmeterState7);

    digitalWrite(solPin1, LOW);     // OFF
    digitalWrite(solTTLPin1, LOW);  // OFF
    digitalWrite(solPin5, LOW);     // OFF
    digitalWrite(solTTLPin5, LOW);  // OFF
    digitalWrite(solPin7, LOW);     // OFF
    digitalWrite(solTTLPin7, LOW);  // OFF

    toneAC();
    return;
  }

  if (rewardStatus==0){
    if (state == 0) {  // If mouse has poked the return, initiate next trial and play cue
      randNumber = random(0,3); 
      state = 1; // Trial initiated, now play cue
      if (probeTrials==1){
        probeTrialPrev = probeTrialCur;
        probeTrialCur = randNumberProb(probeTrialProb);
        if ((probeTrialPrev==1) && (probeTrialCur==1)){probeTrialCur=0;}// Don't have two consecutive probe trials
      }
    } else if(state == 1){
    // Trial initiated, now play cue if mouse crosses location threshold
      if ((cueON==0) && (cur_pos> 0.08) && (probeTrialCur==0)){
        if (randNumber == 0) {
          toneAC(freq[0],10);
          digitalWrite(toneTTLPin1, HIGH); 
          numLow = numLow+1;  
        } else if (randNumber == 1) {
          toneAC(freq[1],10);
          digitalWrite(toneTTLPin2, HIGH);  
          numMed = numMed+1;  
        } else if (randNumber == 2) {
          toneAC(freq[2],10);
          digitalWrite(toneTTLPin3, HIGH);  
          numHigh = numHigh+1;  
        }
        cueMillis = millis();
        cueON=1;         
        state = 2;
      } else if (probeTrialCur==1){ // if its a probe trial, don't play anything but send a similar signal to intan
        digitalWrite(toneTTLPin4, HIGH); 
        cueMillis = millis();
        cueON=1;
        state = 2;
      }     
    } else if(state == 2){
    // Cue played, waiting for mouse to make a choice
      if(training==1){
        if ((randNumber == 0) && (cur_pos>0.24)){
          digitalWrite(solPin3, HIGH);      // ON
          digitalWrite(solTTLPin3, HIGH);   // ON
          digitalWrite(trialTTLPin, HIGH);  // ON
          rewardStatus = 1;
          state = 3;
          correctTrials = correctTrials + 1;
          currentMillis = millis();
          cueON = 0; 
          toneAC();
        } else if ((randNumber == 1) && (cur_pos>0.64)){
          digitalWrite(solPin5, HIGH);      // ON
          digitalWrite(solTTLPin5, HIGH);   // ON
          digitalWrite(trialTTLPin, HIGH);  // ON
          rewardStatus = 1;
          state = 3;
          correctTrials = correctTrials + 1;
          currentMillis = millis();
          cueON = 0; 
          toneAC();          
        } else if((randNumber == 2) && (cur_pos>0.95)){
          digitalWrite(solPin7, HIGH);      // ON
          digitalWrite(solTTLPin7, HIGH);   // ON
          digitalWrite(trialTTLPin, HIGH);  // ON
          rewardStatus = 1;
          state = 3;
          correctTrials = correctTrials + 1;
          currentMillis = millis();
          cueON = 0; 
          toneAC();
        }
      } else if (training==0){
        if(randNumber == 0) { // if low tone, go to port 3
          if(digitalRead(lickPin3) == HIGH) { // correct
            digitalWrite(solPin3, HIGH);      // ON
            digitalWrite(solTTLPin3, HIGH);   // ON
            digitalWrite(trialTTLPin, HIGH);  // ON
            rewardStatus = 1;
            state = 3;
            correctTrials = correctTrials + 1;
            currentMillis = millis();
            cueON = 0; 
            toneAC();
          }
          else if ((digitalRead(lickPin5) == HIGH) || (digitalRead(lickPin7) == HIGH)){
            rewardStatus = 0;
            state = 3;
            digitalWrite(trialTTLPin, HIGH);  // ON
            cueON = 0; 
            toneAC();
            toneAC(3000, 10);
            wrongTrial = 1;
            currentMillis = millis();
          }
          else if (millis() - trialCountDown >= timeOutDur) {
            rewardStatus = 0;
            state = 3;
            digitalWrite(trialTTLPin, HIGH);  // ON
            cueON = 0; 
            toneAC();
            toneAC(3000, 10);
            wrongTrial = 1;
            currentMillis = millis();
          }
        }
        else if(randNumber == 1) { // if med tone, go to port 5
          if(digitalRead(lickPin5) == HIGH) { // correct
            digitalWrite(solPin5, HIGH);      // ON
            digitalWrite(solTTLPin5, HIGH);   // ON
            digitalWrite(trialTTLPin, HIGH);  // ON
            rewardStatus = 1;
            state = 3;
            correctTrials = correctTrials + 1;
            currentMillis = millis();
            cueON = 0; 
            toneAC();
          }
          else if ((digitalRead(lickPin3) == HIGH) || (digitalRead(lickPin7) == HIGH)){
            rewardStatus = 0;
            state = 3;
            digitalWrite(trialTTLPin, HIGH);  // ON
            cueON = 0; 
            toneAC();
            toneAC(3000, 10);
            wrongTrial = 1;
            currentMillis = millis();
          }
          else if (millis() - trialCountDown >= timeOutDur) {
            rewardStatus = 0;
            state = 3;
            digitalWrite(trialTTLPin, HIGH);  // ON
            cueON = 0; 
            toneAC();
            toneAC(3000, 10);
            wrongTrial = 1;
            currentMillis = millis();
          }
        }
        else if(randNumber == 2) { // if high tone, go to port 7
          if(digitalRead(lickPin7) == HIGH) { // correct
            digitalWrite(solPin7, HIGH);      // ON
            digitalWrite(solTTLPin7, HIGH);   // ON
            digitalWrite(trialTTLPin, HIGH);  // ON
            rewardStatus = 1;
            state = 3;
            correctTrials = correctTrials + 1;
            currentMillis = millis();
            cueON = 0; 
            toneAC();
          }
          else if ((digitalRead(lickPin3) == HIGH) || (digitalRead(lickPin5) == HIGH)){
            rewardStatus = 0;
            state = 3;
            digitalWrite(trialTTLPin, HIGH);  // ON
            cueON = 0; 
            toneAC();
            toneAC(3000, 10);
            wrongTrial = 1;
            currentMillis = millis();
          }
          else if (millis() - trialCountDown >= timeOutDur) {
            rewardStatus = 0;
            state = 3;
            digitalWrite(trialTTLPin, HIGH);  // ON
            cueON = 0; 
            toneAC();
            toneAC(3000, 10);
            wrongTrial = 1;
            currentMillis = millis();
          }
        }
      }
    } 
    else if(state == 3){
    // Mouse has made a choice, and now has to run back and lick the home port
      if (wrongTrial == 1) {
        if (millis() - currentMillis >= solenoidDur) {
          digitalWrite(trialTTLPin, LOW);  // ON
        }
        if (millis() - currentMillis >= 2000) {
          toneAC();
        }
      }
      if (lickmeterState1==HIGH){
        toneAC();
        rewardStatus = 1;
        // Mark the end of the trial but do not deliver reward
        digitalWrite(solTTLPin1, HIGH); // ON 
        digitalWrite(solPin1, HIGH); // ON 
        currentMillis = millis();
        trialCountDown = millis();  
        wrongTrial = 0;
        countTrials = countTrials+1;
        state = 0;
      }
    }
  }
  else if (rewardStatus == 1){
    if (millis() - currentMillis >= solenoidDur){
        rewardStatus = 0; 
        digitalWrite(solPin1, LOW); // OFF
        digitalWrite(solTTLPin1, LOW); // OFF  
        digitalWrite(solPin3, LOW); // OFF
        digitalWrite(solTTLPin3, LOW); // OFF           
        digitalWrite(solPin5, LOW); // OFF
        digitalWrite(solTTLPin5, LOW); // OFF   
        digitalWrite(solPin7, LOW); // OFF
        digitalWrite(solTTLPin7, LOW); // OFF     
        digitalWrite(trialTTLPin, LOW); // ON     
    } 
  }

  if (cueON==1){
      if (millis()-cueMillis>cueDur){      
        digitalWrite(toneTTLPin1, LOW); 
        digitalWrite(toneTTLPin2, LOW); 
        digitalWrite(toneTTLPin3, LOW); 
        digitalWrite(toneTTLPin4, LOW); 
        toneAC();
        cueON = 0;        
      }
  }

  lickmeterState1 = digitalRead(lickPin1);
  lickmeterState3 = digitalRead(lickPin3);
  lickmeterState5 = digitalRead(lickPin5);
  lickmeterState7 = digitalRead(lickPin7);

  digitalWrite(lickTTLPin1, lickmeterState1);
  digitalWrite(lickTTLPin3, lickmeterState3);
  digitalWrite(lickTTLPin5, lickmeterState5);
  digitalWrite(lickTTLPin7, lickmeterState7);

  Serial.print(millis()-startMillis);
  Serial.print(", port: ");
  Serial.print(randNumber);  
  Serial.print(", total trials: ");
  Serial.print(countTrials);    
  Serial.print(", correct trials: ");
  Serial.print(correctTrials);
  Serial.print(", Low: ");
  Serial.print(numLow);
  Serial.print(", Med: ");
  Serial.print(numMed); 
  Serial.print(", High: ");
  Serial.print(numHigh); 
  Serial.print(", Dur: ");
  Serial.print(cueDur);  
  Serial.print('\n');
  //
}

int randNumberProb(int x){
  int result;
  result = random(0,100);
  if (result>x){
    return 0;
  }else{
    return 1;
  }
}