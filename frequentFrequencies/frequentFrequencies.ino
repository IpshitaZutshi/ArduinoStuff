#include <toneAC.h>

int delayBetweenPitches[5] = {10, 20, 30, 40, 50};
int delayBetweenSweeps[5] = {2, 4, 6, 8, 10};
int freqRange[2] = {2000, 22000};
float freqExp = (log10(freqRange[1]/freqRange[0]));

// IO Pins definitions
int intanPin1 = 8;
int intanPin2 = 9;
int intanPin3 = 10;
float thisPitch;

void setup() {
  pinMode(intanPin1, OUTPUT); //define both IO pins as outputs
  pinMode(intanPin2, OUTPUT);
  pinMode(intanPin3, OUTPUT);
 
  Serial.begin(9600);
}

void loop() {
  //Step 1  - exterior loop for the sweeps
  for (int ii = 0; ii<100; ii++){ //0:99 or 100 sweeps
   
    int curPitchDelay = random(0,5);
    int curSweepDelay = random(0,5);

    digitalWrite(intanPin1,HIGH); // initiate
    digitalWrite(intanPin2,HIGH);
    digitalWrite(intanPin3,HIGH);    
   
    //Step 2 - interior loop for within a sweep
    for (float jj = 0; jj <= 1; jj+=.02){
      thisPitch = freqRange[0]*(pow(10,(freqExp*jj)));
      toneAC(thisPitch,9); //check syntax
      delay(delayBetweenPitches[curPitchDelay]);
    }
   
    toneAC();
    digitalWrite(intanPin1,LOW); //turn off
    digitalWrite(intanPin2,LOW);
    digitalWrite(intanPin3,LOW);

    Serial.print(ii);
    Serial.print('\n');

    delay(delayBetweenSweeps[curSweepDelay]*1000);
  }
  
}
