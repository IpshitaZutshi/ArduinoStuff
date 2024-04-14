
// Last modified on 7/14/23 to include changes to sync LED with camera (speakerLeft pin is now used to trigger an LED with the base solenoid TTL)

#include <Servo.h>
Servo myServo;

int state = 0;

int phase = 3;  //if phase = 2 (training), both doors open + extended cue
               //if phase = 3 (training), both doors open + variable length cue
               //if phase = 4 HAS DELAY SO WE WILL NOW STICK WITH PHASE 3 FOR EXPERIMENT AS WELL
               
int optPhase = 0;   //0 = no stim
                    //1 = during cue only
                    //2 = during delay only
                    //3 = cue + delay (the 2 seconds)
                    //4 = delay + going to right/left nosepoke 
                    //5 = start immediately after delay, continues for a certain duration after delay
                    //6 = starts at a certain duration (delaystimDur) after delay, continues for a specific time (stimDur)
                    //7  = longer delay, stim starts 1s into delay
                    
boolean stimTrial = false;                    

int blocksSize = 6;//n+1                    
                    
int modality = 4; // make it "int modality = PUT IN A NUMBER (1,2,3,4) MANUALLY DEPENDING ON WHAT YOU WANT TO DO". For example, if u want to train all mice to do just LED manually enter "modality = 1"

int biasCorrect = 1; //make it 1 or 0, if 1, the probability of a left trial is the fraction of the last 20 trials gone right
const int numBiasTrials = 10;

const int numRandTrials = 4;// variable to make sure that the left and right trials are equally presented

int probStim = 65; // Chance that current trial is not an opto trial

int pos = 0;    

int prevTrialCount = 0;  
int countTrials = 0;
int correctTrials = 0;
int numLeft = 0;
int numRight = 0;

//------------ALL DELAYS---------------------------//                                
int cueDelay = 3000; // duration of cue           
int doorBaseDelay = 2000; //4000     //DEFAULT IS 2000       
int varyCue = 0;// if 0, cueDelay is constant, if 1 cueDelay changes;
int cueDelayTimes[4] = {50,200,500,1000};
int stimDur = 2000;
int delaystimDur = 1600;
                              
int speakerDelay = 1000; // time for wrong tone
int solenoidDelayR = 40;//
int solenoidDelayL = 40;//
int solenoidDelay = 40;//

//-------------------------------------------------//

const int openDoor = -270;                                                   
const int closeDoor = (-openDoor/3);


bool doorLeftState = false;
bool doorRightState = false;                                                  
bool doorBaseState = false;                                                   
                                                                              
const int ledLeft = 4;
const int ledRight = 5;

const int speakerLeft = 20; // Now the LED to sync with cam                                        
const int speakerRight = 21;                                        
                                                                                                                
const int solLeft = 48;                                             
const int solBase = 50;
const int solRight = 52;

const int doorLeft = 15;
const int doorRight = 16;
const int doorBase = 17;

const int IRbase = 30;
const int IRdoor = 31;
const int IRleft = 32;
const int IRright = 33;

int blocksCount = 1;


//INTAN PINS ========================================= 
 
const int leftCue = 37;      //intan pin 6
const int rightCue = 39;     //intan pin 7
const int solPin = 41;       //intan pin 8   
const int baseSolPin = 43;    //intan pin 9  //baseSolPin
const int IRdoorpin = 45;    //intan pin 10
const int IRleftpin = 47;    //intan pin 11 //
const int IRrightpin = 49;   //intan pin 12 //
const int delayIntan = 51;   //intan pin 13           //it will turn high when mouse pokes nose at doorIR. It will turn off when door opens. We can just subtract the duration of light from this to figure out how long the delay (period where there is no cue) was for the trials. IMPLEMENT THIS LATER
const int optStim = 53;      //intan pin 14

int startMillis;
long cueMillis = 0;
long delayMillis = 0;
long stimMillis = 0;
long delaystimMillis = 0;
int delaystimON = 0;
int stimON = 0;
int cueON = 0;
int delayON = 0;
int wrongTrial = 0;
int pastchoices[numBiasTrials];
int newchoices[numBiasTrials];
float bias = -1;
float sumchoices = -1;

int pastTrials[numRandTrials] = {0,0,0,0};
int newTrials[numRandTrials]= {0,0,0,0};
float trialbias = -1;
float trialsumchoices = -1;

Servo servo1;
Servo servo2;
Servo servo3;

long randNumber = 0;
long randNumberModality;     

void setup() {

  //OUTPUTS 
  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);
  pinMode(speakerLeft, OUTPUT);
  pinMode(speakerRight, OUTPUT);
  pinMode(solLeft, OUTPUT);
  pinMode(solRight, OUTPUT);
  pinMode(solBase, OUTPUT);

  pinMode(leftCue, OUTPUT); digitalWrite(leftCue, LOW);
  pinMode(rightCue, OUTPUT); digitalWrite(rightCue, LOW);
  pinMode(solPin, OUTPUT); digitalWrite(solPin, LOW);
  pinMode(baseSolPin, OUTPUT); digitalWrite(baseSolPin, LOW);
  pinMode(IRdoorpin, OUTPUT); digitalWrite(IRdoorpin, LOW);
  pinMode(IRleftpin, OUTPUT); digitalWrite(IRleftpin, LOW);
  pinMode(IRrightpin, OUTPUT); digitalWrite(IRrightpin, LOW);
  pinMode(delayIntan, OUTPUT); digitalWrite(delayIntan, LOW);      

  pinMode(optStim, OUTPUT); digitalWrite(optStim, LOW);
  

  servo1.attach(doorLeft);
  servo2.attach(doorRight);
  servo3.attach(doorBase);

  //INPUTS
  pinMode(IRbase, INPUT);
  pinMode(IRdoor, INPUT);
  pinMode(IRleft, INPUT);
  pinMode(IRright, INPUT);
  
  Serial.begin(9600);
  Serial.flush();
  startMillis  = millis();

  randomSeed(analogRead(1));
}

void loop(){
  
  if(state == 0) {                                             //state = 0 means home base
    if(digitalRead(IRdoor) == HIGH) {
//      if(blocksCount == blocksSize) {
//        stimTrial = !stimTrial;
//        blocksCount = 1;
//      }      

      stimTrial = randNumberProb(probStim);
      
      delayMillis = millis();
      delayON = 1;      
      digitalWrite(delayIntan, HIGH);    
       
      if (varyCue==1){        
        cueDelay = cueDelayTimes[random(0,4)];
        doorBaseDelay = 1000+cueDelay;
      }
      
      if (biasCorrect==0){ // Don't correct for mouse's bias, but make sure that the arduino doesn't generate way more trials towards a certain side
         if (countTrials<numRandTrials){  
            randNumber = random(0,2);     
          //  randNumber = 1;      
          } else{
              trialsumchoices = 0;
              for (int ii = 0; ii<numRandTrials; ii++){
                trialsumchoices += newTrials[ii]; 
              }
              trialbias = round((trialsumchoices/numRandTrials)*100);
              randNumber = randNumberProb(trialbias); //make it so that randNumber is 0 or 1 with the probability of past trials;
              
         //  randNumber = 1;
         }
         for (int ii = 0; ii<(numRandTrials-1); ii++){
           newTrials[ii+1] = pastTrials[ii]; 
         }
         memcpy(pastTrials,newTrials, sizeof(pastTrials));
              
      } else if (biasCorrect==1){
          if (countTrials<numBiasTrials){  
            randNumber = random(0,2);
            
          } else{
              sumchoices = 0;
              for (int ii = 0; ii<numBiasTrials; ii++){
                sumchoices += newchoices[ii]; 
              }
              bias = round((sumchoices/numBiasTrials)*100);
              randNumber = randNumberProb(bias); //make it so that randNumber is 0 or 1 with the probability of pastchoices;  
         
          }
          for (int ii = 0; ii<(numBiasTrials-1); ii++){
            newchoices[ii+1] = pastchoices[ii]; 
          }
          memcpy(pastchoices,newchoices, sizeof(pastchoices));

          //randNumber = 0;
      }
      digitalWrite(IRdoorpin, HIGH); delay(1); digitalWrite(IRdoorpin, LOW);
      
      newTrials[0] = randNumber;
      
      if(randNumber == 0) {
        numLeft = numLeft+1;
        if(modality == 1) {
          digitalWrite(ledLeft, HIGH); 
          digitalWrite(leftCue, HIGH);  
          if (phase == 3){
            cueMillis = millis();
            cueON=1;
          }
          else if (phase == 4) {
            delay(cueDelay); 
            digitalWrite(ledLeft, LOW);
            digitalWrite(leftCue, LOW);
          }
        }
        else if(modality == 2) {
          tone(speakerRight, 10000);
          digitalWrite(leftCue, HIGH);
          if (phase == 3){
            cueMillis = millis();
            cueON=1;
          }
          else if (phase == 4) {
            delay(cueDelay);
            noTone(speakerRight);
            digitalWrite(leftCue, LOW);
          }          
        }
        else if(modality == 3) {
          randNumberModality = random(0,2);
          if(randNumberModality == 0) {                                                   //if randNumberModality is 0 --> LED will be the presented cue. If randNumberModality is 1 --> speaker will be the presented cue
            digitalWrite(ledLeft, HIGH); 
            digitalWrite(leftCue, HIGH);  
            if (phase == 3){
              cueMillis = millis();
              cueON=1;
            }
            else if (phase == 4) {
              delay(cueDelay); 
              digitalWrite(ledLeft, LOW);
              digitalWrite(leftCue, LOW);
            }
          }
          else if(randNumberModality == 1) {
            tone(speakerRight, 10000);
            digitalWrite(leftCue, HIGH);
            if (phase == 3){
              cueMillis = millis();
              cueON=1;
            }
            else if (phase == 4) {
              delay(cueDelay);
            //  noTone(speakerLeft);
              digitalWrite(leftCue, LOW);
            } 
          }
        }
        else if(modality == 4) {
          digitalWrite(ledLeft, HIGH); 
          digitalWrite(leftCue, HIGH);  
         tone(speakerRight, 10000);
          if (phase == 3){
            cueMillis = millis();
            cueON=1;
            if(optPhase == 1 || optPhase == 3) {                   //opto turns on for cue phase or cue + delay phase
              digitalWrite(optStim, stimTrial);
            } else {digitalWrite(optStim, LOW); 
          }
          }
          else if (phase == 4) {
            delay(cueDelay);
            noTone(speakerRight);
            digitalWrite(leftCue, LOW);
            digitalWrite(ledLeft, LOW);        
          }          
                
        if(phase == 1) {            //in training, only left door (correct door) opens
          servo1.write(openDoor);
          doorLeftState = true;
        } 
        else {       //in experiment, both doors open
          servo1.write(openDoor);
          doorLeftState = true;
          servo2.write(openDoor);
          doorRightState = true;
        }
       }
      } else if(randNumber == 1) {
        numRight = numRight+1;
        if(modality == 1) {
          digitalWrite(ledRight, HIGH); 
          digitalWrite(rightCue, HIGH); 
          if (phase == 3){
            cueMillis = millis();
            cueON=1;
          }
          else if (phase == 4) {
            delay(cueDelay); 
            digitalWrite(ledRight, LOW);
            digitalWrite(rightCue, LOW);
          }          
        }
        else if(modality == 2) {
          tone(speakerRight, 10000);
          digitalWrite(rightCue, HIGH);
          if (phase == 3){
            cueMillis = millis();
            cueON=1;
          }
          else if (phase == 4) {
            delay(cueDelay);
            noTone(speakerRight);
            digitalWrite(rightCue, LOW);
          }  
        }
        else if(modality == 3) {
          randNumberModality = random(0,2);
          if(randNumberModality == 0) {
            digitalWrite(ledRight, HIGH); 
            digitalWrite(rightCue, HIGH); 
            if (phase == 3){
              cueMillis = millis();
              cueON=1;
            }
            else if (phase == 4) {
              delay(cueDelay); 
              digitalWrite(ledRight, LOW);
              digitalWrite(rightCue, LOW);
            }  
          }
          else if(randNumberModality == 1) {
            tone(speakerRight, 10000);
            digitalWrite(rightCue, HIGH);
            if (phase == 3){
              cueMillis = millis();
              cueON=1;
            }
            else if (phase == 4) {
              delay(cueDelay);
              noTone(speakerRight);
              digitalWrite(rightCue, LOW);
            }  
          }
        }
        else if(modality == 4) {
          digitalWrite(ledRight, HIGH); 
          digitalWrite(rightCue, HIGH);        
          tone(speakerRight, 10000);
          if (phase == 3){
            cueMillis = millis();
            cueON=1;
            if(optPhase == 1 || optPhase == 3) {                   //opto turns on for cue phase or cue + delay phase
              digitalWrite(optStim, stimTrial);
            } else {digitalWrite(optStim, LOW); 
          }
          }
          else if (phase == 4) {
            delay(cueDelay);
            noTone(speakerRight);
            digitalWrite(rightCue, LOW);
            digitalWrite(ledRight, LOW);         
          }          
        }  
        if(phase == 1) {
          servo2.write(openDoor);
          doorRightState = true;
        }
        else {
          servo2.write(openDoor);
          doorRightState = true;
          servo1.write(openDoor);
          doorLeftState = true;
        }     
      }
      state = 1;
    }
  }
  else if(state == 1) {            //mouse is on its way to correct side (right or left)
    if(randNumber == 0) {
      if(digitalRead(IRleft) == HIGH) {
        if(optPhase == 4 || optPhase == 6){                   //opto turns off for delay + right/left nosepoke phase
          digitalWrite(optStim, LOW);
        }
        digitalWrite(IRleftpin, HIGH); delay(10); digitalWrite(IRleftpin, LOW);
        digitalWrite(solLeft, HIGH); digitalWrite(solPin, HIGH); delay(solenoidDelayL); digitalWrite(solLeft, LOW); digitalWrite(solPin, LOW);
        digitalWrite(ledLeft, LOW);
        digitalWrite(leftCue, LOW);        
        noTone(speakerRight);      
        
        if(doorRightState) {
        servo2.write(closeDoor);
        }
        doorRightState = false;
        state = 2;
         //INCREMENT CORRECT TRIAL
        correctTrials= correctTrials+1;
        newchoices[0] = 0;
        wrongTrial = 0;
      }
      else if(digitalRead(IRright) == HIGH) {
         if(optPhase == 4 || optPhase == 6){                   //opto turns off for delay + right/left nosepoke phase
          digitalWrite(optStim, LOW);
        }
        digitalWrite(IRrightpin, HIGH); delay(10); digitalWrite(IRrightpin, LOW);
        digitalWrite(ledLeft, LOW);
        digitalWrite(leftCue, LOW);
        noTone(speakerRight); 

        if(doorLeftState) {                                             
        servo1.write(closeDoor);
        }
        doorLeftState = false;          
        tone(speakerRight, 3000);     //  tone(speakerLeft, 3000);                      //if they get it wrong, both speakers play a sound
        delay(speakerDelay);
        noTone(speakerRight);// noTone(speakerLeft); 
        state = 2;
        newchoices[0] = 1;
        //DON'T INCREMENT CORRECT TIRAL BECAUSE ITS WRONG
        wrongTrial = 1;
      }
    }
    else if(randNumber == 1) {
      if(digitalRead(IRright) == HIGH) {
        if(optPhase == 4 || optPhase == 6) {                   //opto turns off for delay + right/left nosepoke phase
          digitalWrite(optStim, LOW);
        }
        digitalWrite(IRrightpin, HIGH); delay(10); digitalWrite(IRrightpin, LOW);
        digitalWrite(solRight, HIGH); digitalWrite(solPin, HIGH); delay(solenoidDelayR); digitalWrite(solRight, LOW); digitalWrite(solPin, LOW);
        digitalWrite(ledRight, LOW); 
        digitalWrite(rightCue, LOW);       
        noTone(speakerRight);        

        if(doorLeftState) {                                             
          servo1.write(closeDoor);
        }
        doorLeftState = false;
        state = 2;
        wrongTrial = 0;
        newchoices[0] = 1;
        //INCREMENT CORRECT TRIAL
        correctTrials= correctTrials+1;
      }
      else if(digitalRead(IRleft) == HIGH) {
        if(optPhase == 4 || optPhase == 6) {                   //opto turns off for delay + right/left nosepoke phase
          digitalWrite(optStim, LOW);
        }
        digitalWrite(IRleftpin, HIGH); delay(10); digitalWrite(IRleftpin, LOW); 
        digitalWrite(ledRight, LOW); 
        digitalWrite(rightCue, LOW); 
        noTone(speakerRight); 

        if(doorRightState) {
          servo2.write(closeDoor);
        }
        doorRightState = false;         
        tone(speakerRight, 3000);       // tone(speakerLeft, 3000); //if they get it wrong, both speakers play a sound
        delay(speakerDelay);
        noTone(speakerRight);//noTone(speakerLeft);
        state = 2;
        wrongTrial = 1;
        newchoices[0] = 0;
        //DON'T INCREMENT CORRECT TIRAL BECAUSE ITS WRONG
      }
    }
  }
  else if(state == 2) {    //mouse on its way back to home base but trial hasn't resetted yet
    
    if(digitalRead(IRbase) == HIGH) {
      //if (wrongTrial == 0){
        digitalWrite(solBase, HIGH); digitalWrite(speakerLeft, HIGH); digitalWrite(baseSolPin, HIGH); 
        delay(solenoidDelay); 
        digitalWrite(solBase, LOW); digitalWrite(baseSolPin, LOW);
      //}
      if(doorLeftState) {                                             
        servo1.write(closeDoor);
      }
      if(doorRightState) {
        servo2.write(closeDoor);
      }
      
      if(doorBaseState) {          
        servo3.write(closeDoor);
      }
      
      doorLeftState = false;
      doorRightState = false;
      doorBaseState = false;
      
      state = 0; //state goes back to 0 and trial resets
      countTrials = countTrials+1;
      blocksCount = blocksCount+1;
      delay(500);
      digitalWrite(speakerLeft, LOW);       
    }
         
  }

  if (phase==3){
    if (cueON==1){
      if (millis()-cueMillis>cueDelay){      
        digitalWrite(ledLeft, LOW);
        digitalWrite(leftCue, LOW);
        //noTone(speakerLeft);       
        digitalWrite(ledRight, LOW); 
        digitalWrite(rightCue, LOW); 
        noTone(speakerRight);       
        cueON=0;
        if(optPhase == 1) {
          digitalWrite(optStim, LOW);             //opto turns off for cue phase
        }
        if(optPhase == 2 || optPhase == 4) {
          digitalWrite(optStim, stimTrial);       //opto turns on for delay phase or delay + right/left nosepoke phase
        }      
    }
   }
  }

  if (delayON==1){
   if (millis()-delayMillis>doorBaseDelay){   
     servo3.write(openDoor);
     doorBaseState = true;
     digitalWrite(delayIntan, LOW);
     delayON=0;
     if(optPhase == 2 || optPhase == 3) {
        digitalWrite(optStim, LOW);          //opto turns off for delay phase or cue + delay phase
     }    
    if(optPhase == 5) {
      digitalWrite(optStim, stimTrial);       //opto turns on for stim after delay
      stimON = 1;
      stimMillis = millis();
    
    }
    if (optPhase == 6){
      delaystimON=1;
      delaystimMillis = millis();
    }                  
   }else if ((millis()-delayMillis>2000) && (stimON ==0)){
     if(optPhase == 7) {
      digitalWrite(optStim, stimTrial);       //opto turns on for stim after delay
      stimON = 1;
      stimMillis = millis();    
    }    
   }
  }
  
  if (delaystimON==1){  // if stim only turns on for a certain amount of time
    if (millis()-delaystimMillis>delaystimDur){  
      delaystimON=0;
      digitalWrite(optStim, stimTrial);       //opto turns on for stim 2s after delay
      stimON = 1;
      stimMillis = millis();    
    }    
  }

  if (stimON==1){ // if stim only turns on for a certain amount of time
    if (millis()-stimMillis>stimDur){  
      stimON = 0;
      digitalWrite(optStim, LOW);    // turn stim off after that time has elapsed
    }    
  }
 
  digitalWrite(IRdoorpin,digitalRead(IRdoor));

  Serial.print(millis()-startMillis);
  Serial.print("ms, modality: ");
  Serial.print(modality);
  Serial.print(", arm: ");
  Serial.print(randNumber);  
  Serial.print(", total trials: ");
  Serial.print(countTrials);    
  Serial.print(", correct trials: ");
  Serial.print(correctTrials);
  Serial.print(", left: ");
  Serial.print(numLeft);
  Serial.print(", right: ");
  Serial.print(numRight); 
  Serial.print(", bias: ");
  Serial.print(bias);   
  Serial.print(", delay: ");
  Serial.print(cueDelay);  
  Serial.print('\n');
 
}

int randNumberProb(int x){
  int result;
  result = random(0,100);
  if (result<(x-1)){
    return 0;
  }else{
    return 1;
  }
}
