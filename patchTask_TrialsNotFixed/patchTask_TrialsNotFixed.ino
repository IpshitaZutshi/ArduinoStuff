/* ==================== PINS ============================================*/
// Solenoid Pins
const int solPins[7] = { 40, 42, 44, 46, 48, 50, 52 };  // Output pins for close circuit on transistors

// IR pins
const int lickPins[7] = { 41, 43, 45, 47, 49, 21, 53 };  // Input pins for IRs

// LED pin
const int ledPin = 17;  // LED for camera tracking

const int optoPin1 = 23;
const int positionAudioPin = 6;

/* ==============INTAN PINS ============================================*/

// Solenoid Pins
const int solTTLPins[7] = { 26, 28, 30, 32, 34, 36, 38 };  // Output pins for close circuit on transistors

// Capacitance pins
const int lickTTLPins[7] = { 27, 29, 31, 33, 35, 37, 39 };  // Input pin for touch state

const int trialTTLPin = 25;  // Sends a TTL at the end of each trial

const int optoTTLPin1 = 2;

/*==================== SETTINGS ========================================================*/

const int randSeedNumber = 1789;  
const int solenoidDur = 30;       // time solenoid should be kept on in ms

int lickmeterStates[7] = { 0, 0, 0, 0, 0, 0, 0 };  

float lickmeterProbs[7] = { 0, 0, 0, 0, 0, 0, 0 };  

float highProbs[3] = { 0.8, 0.7, 0.9 };   // { 0.8, 0.7, 0.5 }; 0.8, 0.7, 0.9
float lowProbs[3] = { 0.2, 0.1, 0.05 }; //  0.2, 0.1, 0.05 

int patchProb = random(2);
int mostRecent = 0;
int mostRecentPatch = 0;

float curPos = 0;  

unsigned long trialCountDown;
unsigned long currentMillis;
unsigned long LEDMillis;
boolean ledState = false;

unsigned long solenoidStartTime;  
boolean detectionActive = false;  

int countTrials = 0;    // total number of trials which have taken place
int correctTrials = 0;  // total number of trials which have resulted in a reward
int trialTracker = 0;   // counts the trial to make sure they reach at least 30
int totalTrials = 0;  // keeps track of how many trials have occurred in the patch

// Track the last 30 trials and their patch status
int trialIndex = 0;  // Index to track where to store the next trial result
int minTrials = 30;         // Minimum number of trials before making a decision
int lastTrials[30];  // Array to store if the trial was in high (1) or low (0) patch MUST BE SAME AS minTrials
float percentageThreshold = 0.75;  // Percentage threshold for high-probability trials

void setup() {
  Serial.begin(57600);
  //randomSeed(randSeedNumber);
  randomSeed(analogRead(0));
  patchProb = random(2);  
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

void loop() {
  // Flash LED on/off per 500 ms
  if (millis() - LEDMillis >= 500) {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    LEDMillis = millis();
  }

  for (int i = 0; i < 8; i++) {
    if (!detectionActive && lickmeterStates[i] == 1) { 
      if (i == mostRecent) {  // prevents mouse from getting rewarded twice in a row at same port
        continue;
      }
      else {
        mostRecent = i;
        tracker();
        float prob = lickmeterProbs[i];
        if (random(101) <= prob * 100) {  
          digitalWrite(solPins[i], HIGH);
          digitalWrite(solTTLPins[i], HIGH);
          correctTrials++;  
        };
        detectionActive = true;
        solenoidStartTime = millis();
        digitalWrite(trialTTLPin, HIGH);  
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
        Serial.print(totalTrials);
        Serial.print('_');
        Serial.print(patchProb);
        Serial.print('\n');
        break;  
      }
    }
  }

  if (detectionActive && (millis() - solenoidStartTime >= solenoidDur)) {
    setSolsLow();
  }

  if (!detectionActive) {
    setSolsLow();
  }

  readLickStates();
}

void readLickStates() {
  for (int i = 0; i < 7; i++) {
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
  // Always update the lastTrials array with the most recent trial
  for (int i = 0; i < minTrials - 1; i++) {
    lastTrials[i] = lastTrials[i + 1];
  }
  
  lastTrials[minTrials - 1] = round(lickmeterProbs[mostRecent]); 
  
  Serial.print("Updated lastTrials: ");
  for (int i = 0; i < minTrials; i++) {
    Serial.print(lastTrials[i]);
    Serial.print(" ");
  }
  Serial.println(); 
  
  totalTrials++; 

  if (totalTrials >= minTrials) {  
    int highProbCount = 0;

    for (int i = 0; i < minTrials; i++) {
      if (lastTrials[i] == 1) {  // 1 means high-probability patch
        highProbCount++;
      }
    }

    if ((float)highProbCount / minTrials >= percentageThreshold) {
      // Switch patch based on the most recent patch
      if (patchProb == 1) {
        patchProb = 0;  
      } else {
        patchProb = 1;  
      }

      reassign();  
      trialTracker = 0;
      totalTrials = 0;
      memset(lastTrials, 0, sizeof(lastTrials));  // Reset the trial counter for the new patch
    }
  } 
  else {
    trialTracker++;  
  }
}


void reassign() {
  float sumPatch1 = 0;
  float sumPatch2 = 0;

  if (patchProb == 0) {  
    mostRecentPatch = 0;

    float tempProbs[3] = {highProbs[0], highProbs[1], highProbs[2]};  
    shuffleArray(tempProbs, 3);

    for (int i = 0; i < 3; i++) {
      lickmeterProbs[i] = tempProbs[i];
      sumPatch1 += lickmeterProbs[i];
    }

    float tempLowProbs[3] = {lowProbs[0], lowProbs[1], lowProbs[2]};  
    shuffleArray(tempLowProbs, 3);  // Shuffle the array

    for (int i = 4; i < 7; i++) {
      lickmeterProbs[i] = tempLowProbs[i - 4];  
      sumPatch2 += lickmeterProbs[i];
    }

  } else {  
    mostRecentPatch = 1;
    float tempLowProbs[3] = {lowProbs[0], lowProbs[1], lowProbs[2]}; 
    shuffleArray(tempLowProbs, 3);  // Shuffle the array

    for (int i = 0; i < 3; i++) {
      lickmeterProbs[i] = tempLowProbs[i];
      sumPatch1 += lickmeterProbs[i];
    }
    float tempProbs[3] = {highProbs[0], highProbs[1], highProbs[2]};  
    shuffleArray(tempProbs, 3);  

    for (int i = 4; i < 7; i++) {
      lickmeterProbs[i] = tempProbs[i - 4];  
      sumPatch2 += lickmeterProbs[i];
    }
  }
}

// Function to shuffle an array
void shuffleArray(float arr[], int size) {
  for (int i = size - 1; i > 0; i--) {
    int j = random(i + 1);  
    float temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

