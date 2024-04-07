#include <toneAC.h>

/* ==================== PINS ============================================*/
//Solenoid Pins
const int solPin1 = 40;  // Ouputpin for close circuit on transistor - #1
const int solPin5 = 48;  // Ouputpin for close circuit on transistor - #5
const int solPin7 = 52;  // Ouputpin for close circuit on transistor - #7

//IR pins
const int lickPin1 = 41;  // Input pin for IR #1
const int lickPin5 = 49;  // Input pin for IR #5
const int lickPin7 = 53;  // Input pin for IR #7

const int positionAudioPin = 6;

/* ==============INTAN PINS ============================================*/

//Solenoid Pins
const int solTTLPin1 = 26;  // Ouputpin for close circuit on transistor - #1
const int solTTLPin5 = 34;  // Ouputpin for close circuit on transistor - #5
const int solTTLPin7 = 38;  // Ouputpin for close circuit on transistor - #7

//Capacitance pins
const int lickTTLPin1 = 27;  // Input pin for touch state - #1
const int lickTTLPin5 = 35;  // Input pin for touch state - #5
const int lickTTLPin7 = 39;  // Input pin for touch state - #7

const int trialTTLPin = 25;  //Sends a TTL at the end of each trial

/*==================== SETTINGS ========================================================*/

//Variables to change
int randSeedNumber = 1789;  //input any random digits
int training = 0;           //Flag for training. If 0, water delivery happens ONLY after a lick is detected on the correct port
int probeTrials = 0;        // Flag for whether to include probe trials - here no tone plays, allowing for "uncertainty"
int probeTrialProb = 30;    // Probability of probe trials if probeTrials = 1
int biasCorrect = 1;        // Flag for whether to do bias correction

// Dont change anything below
int freqs[2] = { 9000, 16000 };
int lickmeterState1 = 0;
int lickmeterState5 = 0;
int lickmeterState7 = 0;

float cur_pos = 0;  // position from Bonsai
int rewardStatus = 0;
int returnRun = 1;           // Flag for a return trial (no audio)
int solenoidDur = 30;        // in ms
int timeOutDur = 60 * 1000;  // 10 seconds
unsigned long trialCountDown;
unsigned long currentMillis;

const int numRandTrials = 4;// variable to make sure that the left and right trials are equally presented
const int numBiasTrials = 10;
int pastchoices[numBiasTrials];
int newchoices[numBiasTrials];
float bias = -1;
float sumchoices = -1;

int pastTrials[numRandTrials] = {0,0,0,0};
int newTrials[numRandTrials]= {0,0,0,0};
float trialbias = -1;
float trialsumchoices = -1;


int prevTrialCount = 0;  
int countTrials = 0;
int correctTrials = 0;
int randNumber = 0;

int wrongTrial = 0;
int cur_gain_num;
int cur_gain;   //current gain
int past_gain;  //current gain
int cur_reward = -1;


void setup() {
  // initialize serial communications (for debugging only):
  Serial.begin(57600);
  randomSeed(randSeedNumber);

  pinMode(lickPin1, INPUT);  // set pin as INPUT
  pinMode(lickPin5, INPUT);  // set pin as INPUT
  pinMode(lickPin7, INPUT);  // set pin as INPUT

  pinMode(solPin1, OUTPUT);  // set pin as OUTPUT
  pinMode(solPin5, OUTPUT);  // set pin as OUTPUT
  pinMode(solPin7, OUTPUT);  // set pin as OUTPUT

  pinMode(solTTLPin1, OUTPUT);  // set pin as OUTPUT
  pinMode(solTTLPin5, OUTPUT);  // set pin as OUTPUT
  pinMode(solTTLPin7, OUTPUT);  // set pin as OUTPUT

  pinMode(lickTTLPin1, OUTPUT);  // set pin as OUTPUT
  pinMode(lickTTLPin5, OUTPUT);  // set pin as OUTPUT
  pinMode(lickTTLPin7, OUTPUT);  // set pin as OUTPUT

  pinMode(trialTTLPin, OUTPUT);  // set pin as OUTPUT

  pinMode(positionAudioPin, OUTPUT);  //
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


  if (state == 0) {  // If mouse has poked the return, initiate next trial and play cue
      if (biasCorrect == 0) {  // Don't correct for mouse's bias
        if (countTrials < numRandTrials) {
          randNumber = random(0, 2);
        } else {
          trialsumchoices = 0;
          for (int ii = 0; ii < numRandTrials; ii++) {
            trialsumchoices += newTrials[ii];
          }
          trialbias = round((trialsumchoices / numRandTrials) * 100);
          randNumber = randNumberProb(trialbias);  //make it so that randNumber is 0 or 1 with the probability of past trials;
        }
        for (int ii = 0; ii < (numRandTrials - 1); ii++) {
          newTrials[ii + 1] = pastTrials[ii];
        }
        memcpy(pastTrials, newTrials, sizeof(pastTrials));

      } else if (biasCorrect == 1) {
        if (countTrials < numBiasTrials) {
          randNumber = random(0, 2);

        } else {
          sumchoices = 0;
          for (int ii = 0; ii < numBiasTrials; ii++) {
            sumchoices += newchoices[ii];
          }
          bias = round((sumchoices / numBiasTrials) * 100);
          randNumber = randNumberProb(bias);  //make it so that randNumber is 0 or 1 with the probability of pastchoices;
        }
        for (int ii = 0; ii < (numBiasTrials - 1); ii++) {
          newchoices[ii + 1] = pastchoices[ii];
        }
        memcpy(pastchoices, newchoices, sizeof(pastchoices));
      }
      state = 1; // Trial initiated, now play cue
  } 
  
  elseif(state == 1){
  // Trial initiated, now play cue if mouse crosses location threshold
    if (cueON==0) && (cur_pos> 0.2){
      if (randNumber == 0) {
        toneAC(freq[0],10)
        digitalWrite(tonePin1, HIGH); 
        numLow = numLow+1;  
      } else{
        toneAC(freq[1],10)
        digitalWrite(tonePin2, HIGH);  
        numHigh = numHigh+1;  
      }
      cueMillis = millis();
      cueON=1;         
    }
    state = 2;
  }
  elseif(state == 2){
  // Cue played, waiting for mouse to make a choice
    if(randNumber == 0) { // if low tone, go to port 5
      if(digitalRead(lickPin5) == HIGH) { // correct
        digitalWrite(solPin5, HIGH);      // ON
        digitalWrite(solTTLPin5, HIGH);   // ON
        digitalWrite(trialTTLPin, HIGH);  // ON
        rewardStatus = 1;
        state = 3;
        correctTrials = correctTrials + 1;
        currentMillis = millis();
        toneAC();
      }
      else if (digitalRead(lickPin7) == HIGH){
        rewardStatus = 0;
        state = 3;
        digitalWrite(trialTTLPin, HIGH);  // ON
        toneAC();
        toneAC(3000, 10);
        wrongTrial = 1;
        currentMillis = millis();
      }
      else if (millis() - trialCountDown >= timeOutDur) {
        rewardStatus = 0;
        state = 3;
        digitalWrite(trialTTLPin, HIGH);  // ON
        toneAC();
        toneAC(3000, 10);
        wrongTrial = 1;
        currentMillis = millis();
      }
    }
    elseif(randNumber == 1) { // if high tone, go to port 7
      if(digitalRead(lickPin7) == HIGH) { // correct
        digitalWrite(solPin7, HIGH);      // ON
        digitalWrite(solTTLPin7, HIGH);   // ON
        digitalWrite(trialTTLPin, HIGH);  // ON
        rewardStatus = 1;
        state = 3;
        correctTrials = correctTrials + 1;
        currentMillis = millis();
        toneAC();
      }
      else if (digitalRead(lickPin5) == HIGH){
        rewardStatus = 0;
        state = 3;
        digitalWrite(trialTTLPin, HIGH);  // ON
        toneAC();
        toneAC(3000, 10);
        wrongTrial = 1;
        currentMillis = millis();
      }
      else if (millis() - trialCountDown >= timeOutDur) {
        rewardStatus = 0;
        state = 3;
        digitalWrite(trialTTLPin, HIGH);  // ON
        toneAC();
        toneAC(3000, 10);
        wrongTrial = 1;
        currentMillis = millis();
      }
    }
  } 
  elseif(state == 3){
  // Mouse has made a choice, and now has to run back and lick the home port
    if (wrongTrial == 1) {
      if (millis() - currentMillis >= solenoidDur) {
        digitalWrite(trialTTLPin, LOW);  // ON
      }
      if (millis() - currentMillis >= 2000) {
        toneAC();
      }
    }else{

    }

  }

      } else if (returnRun == 1) {
        
          if (lickmeterState1 == HIGH) {
            toneAC();
            rewardStatus = 1;
            // Mark the end of the trial but do not deliver reward
            digitalWrite(solTTLPin1, HIGH);  // ON
            currentMillis = millis();
            trialCountDown = millis();
            wrongTrial = 0;
            prevTrialCount = countTrials;
            countTrials = countTrials + 1;
            if (opto > 0) {
              optoCount = optoCount + 1;
            }
          }
        } else if (wrongTrial == 0) {
          if (lickmeterState1 == HIGH) {
            rewardStatus = 1;
            digitalWrite(solPin1, HIGH);     // ON
            digitalWrite(solTTLPin1, HIGH);  // ON
            currentMillis = millis();
            trialCountDown = millis();
            prevTrialCount = countTrials;
            countTrials = countTrials + 1;
            if (opto > 0) {
              optoCount = optoCount + 1;
            }
          }
        }
      }
    } else if (rewardStatus == 1) {
      if (millis() - currentMillis >= solenoidDur) {
        rewardStatus = 0;
        digitalWrite(solPin1, LOW);      // OFF
        digitalWrite(solTTLPin1, LOW);   // OFF
        digitalWrite(solPin2, LOW);      // OFF
        digitalWrite(solTTLPin2, LOW);   // OFF
        digitalWrite(solPin3, LOW);      // OFF
        digitalWrite(solTTLPin3, LOW);   // OFF
        digitalWrite(solPin4, LOW);      // OFF
        digitalWrite(solTTLPin4, LOW);   // OFF
        digitalWrite(solPin5, LOW);      // OFF
        digitalWrite(solTTLPin5, LOW);   // OFF
        digitalWrite(solPin6, LOW);      // OFF
        digitalWrite(solTTLPin6, LOW);   // OFF
        digitalWrite(solPin7, LOW);      // OFF
        digitalWrite(solTTLPin7, LOW);   // OFF
        digitalWrite(trialTTLPin, LOW);  // ON
        if (returnRun == 0) {
          returnRun = 1;
          toneAC();
        } else if (returnRun == 1) {
          returnRun = 0;
        }
      }
    }
  }

  lickmeterState1 = digitalRead(lickPin1);
  lickmeterState5 = digitalRead(lickPin5);
  lickmeterState7 = digitalRead(lickPin7);

  digitalWrite(lickTTLPin1, lickmeterState1);
  digitalWrite(lickTTLPin2, lickmeterState2);
  digitalWrite(lickTTLPin3, lickmeterState3);
  digitalWrite(lickTTLPin4, lickmeterState4);
  digitalWrite(lickTTLPin5, lickmeterState5);
  digitalWrite(lickTTLPin6, lickmeterState6);
  digitalWrite(lickTTLPin7, lickmeterState7);

  Serial.print(millis());
  Serial.print('_');
  Serial.print(countTrials);
  Serial.print('_');
  Serial.print(cur_gain);
  Serial.print('_');
  Serial.print(correctTrials);
  Serial.print('_');
  Serial.print(linearTrackTrials);
  Serial.print('_');
  Serial.print(training);
  Serial.print('_');
  Serial.print(controlTrials);
  Serial.print('_');
  Serial.print(returnRun);
  Serial.print('_');
  Serial.print(optoFlag);
  Serial.print('_');
  Serial.print(ledState);
  Serial.print('\n');
  //
}
