
/* ==================== PINS ============================================*/
//Solenoid Pins
const int solPins[7] = { 40, 42, 44, 46, 48, 50, 52 };  // Ouput pins for close circuit on transistors

//IR pins
const int lickPins[7] = { 41, 43, 45, 47, 49, 21, 53 };  // Input pins for IRs

//LED pin
const int ledPin = 17;  // LED for camera tracking

const int optoPin1 = 23;
const int positionAudioPin = 6;

/* ==============INTAN PINS ============================================*/

//Solenoid Pins
const int solTTLPins[7] = { 26, 28, 30, 32, 34, 36, 38 };  // Ouput pins for close circuit on transistors

//Capacitance pins
const int lickTTLPins[7] = { 27, 29, 31, 33, 35, 37, 39 };  // Input pin for touch state

const int trialTTLPin = 25;  //Sends a TTL at the end of each trial

const int optoTTLPin1 = 2;

/*==================== SETTINGS ========================================================*/

const int randSeedNumber = 1789;  //input any random digits
const int solenoidDur = 30;       // time solenoid should be kept on in ms


int lickmeterStates[7] = { 0, 0, 0, 0, 0, 0, 0 };  // stores the state of the lickmeters

float lickmeterProbs[7] = { 0, 0, 0, 0, 0, 0, 0 };  // stores the probability of each lickmeter giving a reward

// float availableProbs[3] = {0.2, 0.5, 0.8}; // probabilities to choose from. not currently used
float highProbs[3] = { 0.8, 0.7, 0.5 };
float lowProbs[3] = { 0.2, 0.1, 0.05 };

int patchProb = 0;
int mostRecent = 0;
int mostRecentPatch = 0;

float curPos = 0;  // position from Bonsai (unused)

unsigned long trialCountDown;
unsigned long currentMillis;
unsigned long LEDMillis;
boolean ledState = false;

unsigned long solenoidStartTime;  // time when solonoid was turned on
boolean detectionActive = false;  // boolean indicating whether a solenoid is currently active

int countTrials = 0;    // total number of trials which have taken place
int correctTrials = 0;  // total number of trials which have resulted in a reward
int trialTracker = 0;   // keeps track of how many trials have occurred in the patch



// Called once at the start of the program
void setup() {
  // initialize serial communications (for debugging only):
  Serial.begin(57600);
  randomSeed(randSeedNumber);

  for (int i = 0; i < 7; i++) {
    pinMode(solPins[i], OUTPUT);      // set pin as OUTPUT
    pinMode(lickPins[i], INPUT);      // set pin as INPUT
    pinMode(solTTLPins[i], OUTPUT);   // set pin as OUTPUT
    pinMode(lickTTLPins[i], OUTPUT);  // set pin as OUTPUT
  }

  pinMode(ledPin, OUTPUT);  // set pin as OUTPUT

  pinMode(trialTTLPin, OUTPUT);  // set pin as OUTPUT
  reassign();
}



// Called repeatedly in an infinite loop
void loop() {

//Serial.print(mostRecent);

  //Flash LED on/off per 500 ms
  if (millis() - LEDMillis >= 500) {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    LEDMillis = millis();
  }
/*
  // read position from Bonsai
  if (Serial.available() > 0) {
    curPos = Serial.parseFloat();  //not used
  } else {
    // If no position is received, set to 0.
    readLickStates();
    setSolsLow();
    return;
  }
*/



  for (int i = 0; i < 8; i++) {
    if (!detectionActive && lickmeterStates[i] == 1) {  // Ignores lickmeters if solenoid is already active
      if (i == mostRecent) {  // prevents mouse from getting rewarded twice in a row at same port
        continue;
      }
      else {
        tracker();
        float prob = lickmeterProbs[i];
        if (random(101) <= prob * 100) {  // Randomly determines if lickmeter should give a reward based on lickmeter probability
          // If lick is detected, set solenoid to high
          digitalWrite(solPins[i], HIGH);
          digitalWrite(solTTLPins[i], HIGH);
          correctTrials++;  // If lick is detected, increase correct trials by 1
        };
        detectionActive = true;
        solenoidStartTime = millis();
        digitalWrite(trialTTLPin, HIGH);  // Send TTL at the end of each trial
        countTrials++;
        mostRecent = i;
        Serial.print(millis());
        Serial.print('_');
        Serial.print(countTrials);
        Serial.print('_');
        Serial.print(correctTrials);
        Serial.print('_');
        Serial.print(i);
        Serial.print('_');
        Serial.print(lickmeterProbs[0]);
        Serial.print(',');
        Serial.print(lickmeterProbs[1]);
        Serial.print(',');
        Serial.print(lickmeterProbs[2]);
        Serial.print(',');
        Serial.print(lickmeterProbs[4]);
        Serial.print(',');
        Serial.print(lickmeterProbs[5]);
        Serial.print(',');
        Serial.print(lickmeterProbs[6]);
        Serial.print('\n');
        Serial.print(trialTracker);
        Serial.print('_');
        Serial.print(patchProb);
        Serial.print('\n');
        break;  //assumes only one lickmeter can be active at a time
      }
    }
  }

  if (detectionActive && (millis() - solenoidStartTime >= solenoidDur)) {
    // If solenoid has been active for the required duration, set to low
    setSolsLow();
  }

  if (!detectionActive) {
    setSolsLow();
  }

  readLickStates();

}

void readLickStates() {
  for (int i = 0; i < 7; i++) {
    // Read lickmeter states and the end of each frame during th trial
    lickmeterStates[i] = digitalRead(lickPins[i]);
    digitalWrite(lickTTLPins[i], lickmeterStates[i]);
  }
}

void setSolsLow() {
  for (int i = 0; i < 7; i++) {
    digitalWrite(solPins[i], LOW);
    digitalWrite(solTTLPins[i], LOW);
  }
  digitalWrite(trialTTLPin, LOW);
  detectionActive = false;
}


void tracker() {
 //keep track of when to switch patch 
  if (trialTracker == 39) {  // 40 is number if trials in a patch
    int choose = random(2);
    if (mostRecentPatch == 1) {
      patchProb = 0;  // patch 1 has a higher probability
    } 
    else {
      patchProb = 1;  // patch 2 has a higher probability
    }
    reassign();
    trialTracker = 0;
  } 
  else {
    trialTracker = trialTracker + 1;
  }
}

/* reassign probabilities to ports in patches */
void reassign() {
  float sumPatch1 = 0;
  float sumPatch2 = 0;
  if (patchProb == 0) {  // patch 1 has a higher probability
    mostRecentPatch = 0;
    while (sumPatch1 <= sumPatch2) {
      for (int i = 0; i < 3; i++) {
        lickmeterProbs[i] = highProbs[random(3)];
        sumPatch1 += lickmeterProbs[i];
      }

      for (int i = 4; i < 7; i++) {
        lickmeterProbs[i] = lowProbs[random(3)];
        sumPatch2 += lickmeterProbs[i];
      }
    }
  }

  else {  // patch 2 has a higher probability
    mostRecentPatch = 1;
    while (sumPatch2 <= sumPatch1) {
      for (int i = 0; i < 3; i++) {
        lickmeterProbs[i] = lowProbs[random(3)];
        sumPatch1 += lickmeterProbs[i];
      }

      for (int i = 4; i < 7; i++) {
        lickmeterProbs[i] = highProbs[random(3)];
        sumPatch2 += lickmeterProbs[i];
      }
    }
  }
}





/* if there can be complete overlap of probabilities. ie one probability array 

void assignProbs() {
  for (int i = 0; i < 3; i++) {
      lickmeterProbs[i] = availableProbs[random(3)]
      sumPatch1 += lickmeterProbs[i]
    }

    for (int i = 4; i < 7; i++) {
      lickmeterProbs[i] = availableProbs[random(3)]
      sumPatch2 += lickmeterProbs[i]
    }
}


/* reassign probabilities to ports in patches 
void reassign(){
  if patchProb1 == 0 {    // patch 1 has a higher probability
    while (sumPatch1 <= sumPatch2) {
      assignProbs()
    }
  }
   
  else {     // patch 2 has a higher probability
    while (sumPatch2 <= sumPatch1) {
      assignProbs()
    }
  }

  */
