#include <toneAC.h>

/* ==================== PINS ============================================*/
//Solenoid Pins
const int solPin1 = 40; // Ouputpin for close circuit on transistor - #1
const int solPin2 = 42; // Ouputpin for close circuit on transistor - #2
const int solPin3 = 44; // Ouputpin for close circuit on transistor - #3
const int solPin4 = 46; // Ouputpin for close circuit on transistor - #4
const int solPin5 = 48; // Ouputpin for close circuit on transistor - #5
const int solPin6 = 50; // Ouputpin for close circuit on transistor - #6
const int solPin7 = 52; // Ouputpin for close circuit on transistor - #7

//IR pins
const int lickPin1 = 41;  // Input pin for IR #1
const int lickPin2 = 43;  // Input pin for IR #2
const int lickPin3 = 45;  // Input pin for IR #3
const int lickPin4 = 47;  // Input pin for IR #4
const int lickPin5 = 49;  // Input pin for IR #5
const int lickPin6 = 21;  // Input pin for IR #6
const int lickPin7 = 53;  // Input pin for IR #7

//LED pin
const int ledPin = 17;// LED for camera tracking

const int optoPin1 = 23;
const int positionAudioPin = 6;

/* ==============INTAN PINS ============================================*/

//Solenoid Pins
const int solTTLPin1 = 26; // Ouputpin for close circuit on transistor - #1
const int solTTLPin2 = 28; // Ouputpin for close circuit on transistor - #2
const int solTTLPin3 = 30; // Ouputpin for close circuit on transistor - #3
const int solTTLPin4 = 32; // Ouputpin for close circuit on transistor - #4
const int solTTLPin5 = 34; // Ouputpin for close circuit on transistor - #5
const int solTTLPin6 = 36; // Ouputpin for close circuit on transistor - #6
const int solTTLPin7 = 38; // Ouputpin for close circuit on transistor - #7

//Capacitance pins
const int lickTTLPin1 = 27;  // Input pin for touch state - #1
const int lickTTLPin2 = 29;  // Input pin for touch state - #2
const int lickTTLPin3 = 31;  // Input pin for touch state - #3
const int lickTTLPin4 = 33;  // Input pin for touch state - #4
const int lickTTLPin5 = 35;  // Input pin for touch state - #5
const int lickTTLPin6 = 37;  // Input pin for touch state - #6
const int lickTTLPin7 = 39;  // Input pin for touch state - #7

const int trialTTLPin = 25; //Sends a TTL at the end of each trial

const int optoTTLPin1 = 2;
/*==================== SETTINGS ========================================================*/

//Variables to change
int numLinTrials = 0; //input any random digits
int randSeedNumber = 1789; //input any random digits
int trialBlockNum = 201;//If trialBlocks is  1, after how many trials do you want linear trials to start again?
int training = 0; //Flag for training. If 0, water delivery happens ONLY after a lick is detected on the correct port
int controlTrials = 0; // Flag for whether these are control mice. The tone occcurs but they must ignore and lick the end. 
int equalDist = 1;//6 0 - probability of ports is skewed towards later ports
                  // 1 - equal probability of ports
                  // -1 - only first 3 ports rewarded
                  // 2,3,4,5,6,7 - port number x+2 is the only one rewarded.
int numPorts = 6;// 3 or 6                  
int trialBlocks = 0; // Flag for whether to run linear track and tone trials in blocks
int playToneLinear = 0; // play tone in the linear track portion

int opto = 0; //0 no stim
             //1 stim in blocks during forward run for trigger 1
             //2 stim in blocks during forward run for trigger 2
int optoBlocks = 10;             

// Dont change anything below
int freqRange[2] = {1000, 22000};
float freqExp = (log10(freqRange[1]/freqRange[0]));
int lickmeterState1 = 0;
int lickmeterState2 = 0;
int lickmeterState3 = 0;
int lickmeterState4 = 0;
int lickmeterState5 = 0;
int lickmeterState6 = 0;
int lickmeterState7 = 0;

float cur_pos = 0; // position from Bonsai
float map_pos;// modified position according to trial type
int map_pos2;
int rewardStatus = 0;
int returnRun = 0; // Flag for a return trial (no audio)
int solenoidDur = 30; // in ms
int timeOutDur = 60*1000; // 10 seconds
unsigned long trialCountDown;
unsigned long currentMillis;
unsigned long LEDMillis;

int linearTrackTrials = 1; // Flag to start with linear track trials
int countTrials = 0;
int correctTrials = 0;
int prevTrialCount = 0;

int toneGainOptions[10] = {1,2,3,4,4,5,5,6,6,6};
int toneGainOptions2[4] = {1,2,3,4};
int toneGainOptions3[7] = {6,5,6,5,4,6,6}; //1,2,3,4 change according to which ports should be rewarded
const int arrLen = sizeof(toneGainOptions3[4]) / sizeof(toneGainOptions3[0]);

int gainFactor[6] = {55,130,218,306,390,450};
int maxDim = 450;
int wrongTrial = 0;
int cur_gain_num;
int cur_gain; //current gain
int past_gain; //current gain
int cur_reward = -1;
int optoFlag = 0;
int optoCount = 0;
boolean ledState = false;

void setup() {
  // initialize serial communications (for debugging only):
  Serial.begin(57600);
  randomSeed(randSeedNumber);
      
  pinMode(lickPin1, INPUT); // set pin as INPUT
  pinMode(lickPin2, INPUT); // set pin as INPUT
  pinMode(lickPin3, INPUT); // set pin as INPUT
  pinMode(lickPin4, INPUT); // set pin as INPUT
  pinMode(lickPin5, INPUT); // set pin as INPUT
  pinMode(lickPin6, INPUT); // set pin as INPUT
  pinMode(lickPin7, INPUT); // set pin as INPUT
  
  pinMode(solPin1, OUTPUT); // set pin as OUTPUT
  pinMode(solPin2, OUTPUT); // set pin as OUTPUT
  pinMode(solPin3, OUTPUT); // set pin as OUTPUT
  pinMode(solPin4, OUTPUT); // set pin as OUTPUT
  pinMode(solPin5, OUTPUT); // set pin as OUTPUT
  pinMode(solPin6, OUTPUT); // set pin as OUTPUT
  pinMode(solPin7, OUTPUT); // set pin as OUTPUT

  pinMode(ledPin, OUTPUT); // set pin as OUTPUT
  pinMode(optoPin1, OUTPUT); // set pin as OUTPUT
  pinMode(optoTTLPin1, OUTPUT); // set pin as OUTPUT
      
  pinMode(solTTLPin1, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin2, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin3, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin4, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin5, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin6, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin7, OUTPUT); // set pin as OUTPUT
      
  pinMode(lickTTLPin1, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin2, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin3, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin4, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin5, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin6, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin7, OUTPUT); // set pin as OUTPUT
  
  pinMode(trialTTLPin, OUTPUT); // set pin as OUTPUT

  pinMode(positionAudioPin, OUTPUT);//
}

void loop() {

  //Flash LED on/off per 500 ms
  if (millis()-LEDMillis >= 500){
    ledState = !ledState;
    digitalWrite(ledPin, ledState); // OFF  
    LEDMillis = millis();   
  }
  
  // read position from Bonsai
  if (Serial.available() > 0) {          
    cur_pos = Serial.parseFloat();   
  } 
  else{
    
    lickmeterState1 = digitalRead(lickPin1);
    lickmeterState2 = digitalRead(lickPin2);
    lickmeterState3 = digitalRead(lickPin3);
    lickmeterState4 = digitalRead(lickPin4);
    lickmeterState5 = digitalRead(lickPin5);
    lickmeterState6 = digitalRead(lickPin6);
    lickmeterState7 = digitalRead(lickPin7);
  
    digitalWrite(lickTTLPin1, lickmeterState1);
    digitalWrite(lickTTLPin2, lickmeterState2);
    digitalWrite(lickTTLPin3, lickmeterState3);
    digitalWrite(lickTTLPin4, lickmeterState4);
    digitalWrite(lickTTLPin5, lickmeterState5);
    digitalWrite(lickTTLPin6, lickmeterState6);
    digitalWrite(lickTTLPin7, lickmeterState7);
        
    digitalWrite(solPin1, LOW); // OFF
    digitalWrite(solTTLPin1, LOW); // OFF  
    digitalWrite(solPin2, LOW); // OFF
    digitalWrite(solTTLPin2, LOW); // OFF         
    digitalWrite(solPin3, LOW); // OFF
    digitalWrite(solTTLPin3, LOW); // OFF  
    digitalWrite(solPin4, LOW); // OFF
    digitalWrite(solTTLPin4, LOW); // OFF   
    digitalWrite(solPin5, LOW); // OFF
    digitalWrite(solTTLPin5, LOW); // OFF         
    digitalWrite(solPin6, LOW); // OFF
    digitalWrite(solTTLPin6, LOW); // OFF  
    digitalWrite(solPin7, LOW); // OFF
    digitalWrite(solTTLPin7, LOW); // OFF   
    
    toneAC();
    return;    
    
  }

  // If initial trials, just trigger end solenoids, play tones in reverse order in one running direction
  if (linearTrackTrials ==1){  
    
    //map_pos = map(cur_pos, 0, 1, 1, 0); // reverse position to reverse tone sequence

    // map current position onto tone landscape      
    //int thisPitch = ((cur_pos-0)*((freqRange[1]-freqRange[0])/(1-0)))+freqRange[0];
    int thisPitch = freqRange[0]*(pow(10,(freqExp*cur_pos)));

    // Turn off opto if on
    optoFlag = 0;
    digitalWrite(optoPin1, LOW); // OFF
    digitalWrite(optoTTLPin1, LOW); // OFF 
    optoCount = 0;

    
    // Only play tones in one running direction
    if (returnRun==0){
      if (playToneLinear==1){
        analogWrite(positionAudioPin, cur_pos * 200);
        toneAC(thisPitch,10);
      } else{
        analogWrite(positionAudioPin,0); // No tone being played
        toneAC();
      }
    }
    else{
      analogWrite(positionAudioPin,0); // No tone being played
      toneAC();
    }
    
    //trigger solenoid at the two ends of the track
    if (rewardStatus==0){
      if (returnRun==0){
        if (lickmeterState7==HIGH){//(cur_pos>0.9){//
          rewardStatus = 1;
          returnRun = 1;
          digitalWrite(solPin7, HIGH); // ON
          digitalWrite(solTTLPin7, HIGH); // ON  
          digitalWrite(trialTTLPin, HIGH); // ON            
          currentMillis = millis();   
        } 
       }
       else if (returnRun==1){
          if (lickmeterState1==HIGH){//(cur_pos<0.1){//
            rewardStatus = 1;
            returnRun = 0;            
            digitalWrite(solPin1, HIGH); // ON
            digitalWrite(solTTLPin1, HIGH); // ON          
            currentMillis = millis();  
            trialCountDown = millis(); 
            prevTrialCount = countTrials;
            countTrials = countTrials+1;
          } 
       }
    }
    else if (rewardStatus = 1){
      if (millis() - currentMillis >= solenoidDur){
        rewardStatus = 0; 
        digitalWrite(solPin7, LOW); // OFF
        digitalWrite(solTTLPin7, LOW); // OFF  
        digitalWrite(solPin1, LOW); // OFF
        digitalWrite(solTTLPin1, LOW); // OFF
        digitalWrite(trialTTLPin, LOW); // ON
        if (countTrials== numLinTrials){
           linearTrackTrials = 0;
        }
      }
    }

  }else if (linearTrackTrials == 0){

    // Select a tone gain ONCE per trail
    if (prevTrialCount<countTrials && returnRun == 0){
      if (trialBlocks==1){
        if (countTrials==trialBlockNum){
          if (linearTrackTrials==0){
            linearTrackTrials = 1;
          }
        }
     }
     if (opto>0){ // if we want opto, target it to blocks
      if (optoCount==optoBlocks){
        optoCount = 0;
        if (optoFlag == 0){
          optoFlag = 1;
        } else if (optoFlag==1){
          optoFlag = 0;
        }            
      }
     }

     if (numPorts==6){
      cur_gain_num = random(0,10);
     } else if (numPorts==3){
      cur_gain_num = random(0,4);  
      }
      
     if (equalDist==1){ 
      if (numPorts==6){
          cur_gain = random(0,6);
      } else if (numPorts==3){
          cur_gain = toneGainOptions2[cur_gain_num]-1;
      }
       
     } else if (equalDist==0){
        cur_gain = toneGainOptions[cur_gain_num]-1;
     } else if (equalDist==-1){
        cur_gain_num = random(0,7);
        cur_gain = toneGainOptions3[cur_gain_num]-1;
     } else {      
        cur_gain = equalDist-2;
     }     
     prevTrialCount = countTrials;    
   }
           
    map_pos = (cur_pos/gainFactor[cur_gain])*maxDim; // Gain position for tone sequence    
       
    // map current position onto tone landscape
    //int thisPitch = ((map_pos-0)*((freqRange[1]-freqRange[0])/(1-0)))+freqRange[0];
    int thisPitch = freqRange[0]*(pow(10,(freqExp*map_pos)));
  
    // Only play tones in one running direction
    if (returnRun==0){
      if ((map_pos<1.2) && (cur_pos<1)){
        int map_pos2 = map(map_pos, 0, 1.2, 0, 250);         
        
        if (thisPitch<(freqRange[1]+2000)){
          analogWrite(positionAudioPin, map_pos2);//Send analog tone being played to intan
          // play the pitch:
          toneAC(thisPitch,10);           
        } else{
          analogWrite(positionAudioPin,0); // No tone being played         
          toneAC();   
        }
      } else{       
        toneAC();
        analogWrite(positionAudioPin,0); // No tone being played
      }
      if (optoFlag==1){
        if (opto==1){ 
            digitalWrite(optoPin1, HIGH); // ON
            digitalWrite(optoTTLPin1, HIGH); // ON
          } 
        else if (opto==2){
            digitalWrite(optoPin1, HIGH); // ON
          }
      } 
    } else{
        digitalWrite(optoPin1, LOW); // OFF
        digitalWrite(optoTTLPin1, LOW); // OFF          
    }

    //trigger solenoid 
    if (rewardStatus==0){
      if (returnRun==0){
        if (controlTrials==1){  // Control mice, the tone is playing at random gains, but mice must lick at the last solenoid
          if (lickmeterState7==HIGH){
            rewardStatus = 1;
            digitalWrite(solPin7, HIGH); // ON
            digitalWrite(solTTLPin7, HIGH); // ON  
            digitalWrite(trialTTLPin, HIGH); // ON            
            currentMillis = millis();   
            } 
          }
        else if (training==1){ //training, reward is delivered automatically
            if (map_pos>0.85){
              switch (cur_gain){
                case 0:
                  digitalWrite(solPin2, HIGH); // ON
                  digitalWrite(solTTLPin2, HIGH); // ON    
                  break;
                case 1:
                  digitalWrite(solPin3, HIGH); // ON
                  digitalWrite(solTTLPin3, HIGH); // ON    
                  break;
                case 2:
                  digitalWrite(solPin4, HIGH); // ON
                  digitalWrite(solTTLPin4, HIGH); // ON    
                  break; 
                case 3:
                  digitalWrite(solPin5, HIGH); // ON
                  digitalWrite(solTTLPin5, HIGH); // ON    
                  break;
                case 4:
                  digitalWrite(solPin6, HIGH); // ON
                  digitalWrite(solTTLPin6, HIGH); // ON    
                  break; 
                case 5:
                  digitalWrite(solPin7, HIGH); // ON
                  digitalWrite(solTTLPin7, HIGH); // ON    
                  break;           
              }
              rewardStatus = 1;
              currentMillis = millis();   
            } 
        }
        else if (training==0){ // solenoid signal is dependent on the lick
          switch (cur_gain){
            case 0:
              if ((lickmeterState2==HIGH)){
                digitalWrite(solPin2, HIGH); // ON
                digitalWrite(solTTLPin2, HIGH); // ON  
                digitalWrite(trialTTLPin, HIGH); // ON  
                rewardStatus = 1;
                correctTrials = correctTrials+1;
                currentMillis = millis();   
                break;                
              } else if (lickmeterState3==HIGH || lickmeterState4==HIGH || lickmeterState5==HIGH || lickmeterState6==HIGH || lickmeterState7==HIGH){
                rewardStatus = 0;
                returnRun = 1;
                digitalWrite(trialTTLPin, HIGH); // ON  
                toneAC(3000,10); 
                wrongTrial = 1;
                currentMillis = millis(); 
                break;
              } else if (millis() - trialCountDown >= timeOutDur){
                rewardStatus = 0;    
                returnRun = 1;
                digitalWrite(trialTTLPin, HIGH); // ON  
                toneAC(3000,10); 
                wrongTrial = 1;
                currentMillis = millis();                 
                break;          
              }        
            case 1:
              if ((lickmeterState3==HIGH)){
                digitalWrite(solPin3, HIGH); // ON
                digitalWrite(solTTLPin3, HIGH); // ON  
                digitalWrite(trialTTLPin, HIGH); // ON  
                rewardStatus = 1;
                correctTrials = correctTrials+1;
                currentMillis = millis();   
                break;                
              } else if (lickmeterState2==HIGH || lickmeterState4==HIGH || lickmeterState5==HIGH || lickmeterState6==HIGH || lickmeterState7==HIGH){
                rewardStatus = 0;
                returnRun = 1;
                digitalWrite(trialTTLPin, HIGH); // ON  
                toneAC(3000,10); 
                wrongTrial = 1;
                currentMillis = millis();                 
                break;
              } else if (millis() - trialCountDown >= timeOutDur){
                rewardStatus = 0;   
                returnRun = 1;
                digitalWrite(trialTTLPin, HIGH); // ON
                toneAC(3000,10); 
                wrongTrial = 1;
                currentMillis = millis();                 
                break;           
              } 
            case 2:
              if ((lickmeterState4==HIGH)){
                digitalWrite(solPin4, HIGH); // ON
                digitalWrite(solTTLPin4, HIGH); // ON  
                digitalWrite(trialTTLPin, HIGH); // ON  
                rewardStatus = 1;
                correctTrials = correctTrials+1;
                currentMillis = millis();                   
                break;                
              } else if (lickmeterState2==HIGH || lickmeterState3==HIGH || lickmeterState5==HIGH || lickmeterState6==HIGH || lickmeterState7==HIGH){
                rewardStatus = 0;
                returnRun = 1;
                digitalWrite(trialTTLPin, HIGH); // ON 
                toneAC(3000,10); 
                wrongTrial = 1;
                currentMillis = millis();                 
                break;
              } else if (millis() - trialCountDown >= timeOutDur){
                rewardStatus = 0;  
                returnRun = 1;
                digitalWrite(trialTTLPin, HIGH); // ON  
                toneAC(3000,10); 
                wrongTrial = 1;
                currentMillis = millis();                 
                break;            
              }         
            
            case 3:
              if (lickmeterState5==HIGH){
                digitalWrite(solPin5, HIGH); // ON
                digitalWrite(solTTLPin5, HIGH); // ON 
                digitalWrite(trialTTLPin, HIGH); // ON  
                rewardStatus = 1;
                correctTrials = correctTrials+1;
                currentMillis = millis();                   
                break;                
              } else if (lickmeterState2==HIGH || lickmeterState3==HIGH || lickmeterState4==HIGH || lickmeterState6==HIGH || lickmeterState7==HIGH){
                rewardStatus = 0;
                returnRun = 1;
                digitalWrite(trialTTLPin, HIGH); // ON 
                toneAC(3000,10); 
                wrongTrial = 1;
                currentMillis = millis();                 
                break;
              } else if (millis() - trialCountDown >= timeOutDur){
                rewardStatus = 0;  
                returnRun = 1;
                digitalWrite(trialTTLPin, HIGH); // ON  
                toneAC(3000,10); 
                wrongTrial = 1;
                currentMillis = millis();                 
                break;            
              }  
               
            case 4:
              if (lickmeterState6==HIGH){
                digitalWrite(solPin6, HIGH); // ON
                digitalWrite(solTTLPin6, HIGH); // ON  
                digitalWrite(trialTTLPin, HIGH); // ON  
                rewardStatus = 1;
                correctTrials = correctTrials+1;
                currentMillis = millis();                   
                break;                
              } else if (lickmeterState2==HIGH || lickmeterState3==HIGH || lickmeterState4==HIGH || lickmeterState5==HIGH || lickmeterState7==HIGH){
                rewardStatus = 0;
                returnRun = 1;
                digitalWrite(trialTTLPin, HIGH); // ON 
                toneAC(3000,10); 
                wrongTrial = 1;
                currentMillis = millis();                 
                break;
              } else if (millis() - trialCountDown >= timeOutDur){
                rewardStatus = 0;  
                returnRun = 1;
                digitalWrite(trialTTLPin, HIGH); // ON  
                toneAC(3000,10); 
                wrongTrial = 1;
                currentMillis = millis();                 
                break;            
              } 
               
            case 5:
              if (lickmeterState7==HIGH){
                digitalWrite(solPin7, HIGH); // ON
                digitalWrite(solTTLPin7, HIGH); // ON  
                digitalWrite(trialTTLPin, HIGH); // ON  
                rewardStatus = 1;
                correctTrials = correctTrials+1;
                currentMillis = millis();                   
                break;                
              } else if (lickmeterState2==HIGH || lickmeterState3==HIGH || lickmeterState4==HIGH || lickmeterState5==HIGH || lickmeterState6==HIGH){
                rewardStatus = 0;
                returnRun = 1;
                digitalWrite(trialTTLPin, HIGH); // ON 
                toneAC(3000,10); 
                wrongTrial = 1;
                currentMillis = millis();                 
                break;
              } else if (millis() - trialCountDown >= timeOutDur){
                rewardStatus = 0;  
                returnRun = 1;
                digitalWrite(trialTTLPin, HIGH); // ON  
                toneAC(3000,10); 
                wrongTrial = 1;
                currentMillis = millis();                 
                break;            
              }                                                                                          
          }
        }                  
       }
       else if (returnRun==1){
          if (wrongTrial==1){
            if (millis() - currentMillis >= solenoidDur){
              digitalWrite(trialTTLPin, LOW); // ON  
            }
            if (millis() - currentMillis >= 2000){
              toneAC();
            }           
            if (lickmeterState1==HIGH){
              toneAC();
              rewardStatus = 1;
              // Mark the end of the trial but do not deliver reward
              digitalWrite(solTTLPin1, HIGH); // ON 
              currentMillis = millis();
              trialCountDown = millis();  
              wrongTrial = 0;
              prevTrialCount = countTrials;
              countTrials = countTrials+1;
              if (opto>0){
                optoCount = optoCount+1;                  
              }
           }
          } 
          else if (wrongTrial==0){             
            if (lickmeterState1==HIGH){
              rewardStatus = 1;
              digitalWrite(solPin1, HIGH); // ON
              digitalWrite(solTTLPin1, HIGH); // ON 
              currentMillis = millis();
              trialCountDown = millis();  
              prevTrialCount = countTrials;
              countTrials = countTrials+1;
              if (opto>0){
                optoCount = optoCount+1;            
              }              
            }        
          }        
       }                             
    }else if (rewardStatus == 1){
      if (millis() - currentMillis >= solenoidDur){
        rewardStatus = 0; 
        digitalWrite(solPin1, LOW); // OFF
        digitalWrite(solTTLPin1, LOW); // OFF  
        digitalWrite(solPin2, LOW); // OFF
        digitalWrite(solTTLPin2, LOW); // OFF         
        digitalWrite(solPin3, LOW); // OFF
        digitalWrite(solTTLPin3, LOW); // OFF  
        digitalWrite(solPin4, LOW); // OFF
        digitalWrite(solTTLPin4, LOW); // OFF      
        digitalWrite(solPin5, LOW); // OFF
        digitalWrite(solTTLPin5, LOW); // OFF         
        digitalWrite(solPin6, LOW); // OFF
        digitalWrite(solTTLPin6, LOW); // OFF  
        digitalWrite(solPin7, LOW); // OFF
        digitalWrite(solTTLPin7, LOW); // OFF     
        digitalWrite(trialTTLPin, LOW); // ON    
        if (returnRun==0){
          returnRun = 1;
          toneAC();
        }else if (returnRun==1){
          returnRun = 0;               
        }
      }
    }
  }
  
  lickmeterState1 = digitalRead(lickPin1);
  lickmeterState2 = digitalRead(lickPin2);
  lickmeterState3 = digitalRead(lickPin3);
  lickmeterState4 = digitalRead(lickPin4);
  lickmeterState5 = digitalRead(lickPin5);
  lickmeterState6 = digitalRead(lickPin6);
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
