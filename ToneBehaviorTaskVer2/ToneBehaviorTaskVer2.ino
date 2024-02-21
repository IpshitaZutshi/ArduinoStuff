#include <toneAC.h>

/* ==================== PINS ============================================*/
//Solenoid Pins
const int solPin1 = 44; // Ouputpin for close circuit on transistor - #1
const int solPin2 = 46; // Ouputpin for close circuit on transistor - #2
const int solPin3 = 48; // Ouputpin for close circuit on transistor - #3
const int solPin4 = 50; // Ouputpin for close circuit on transistor - #4

//Capacitance pins
const int lickPin1 = 45;  // Input pin for touch state - #1
const int lickPin2 = 47;  // Input pin for touch state - #2
const int lickPin3 = 49;  // Input pin for touch state - #3
const int lickPin4 = 51;  // Input pin for touch state - #4

const int positionPin = 6;
const int positionAudioPin = 7;

/* ==============INTAN PINS ============================================*/

//Solenoid Pins
const int solTTLPin1 = 36; // Ouputpin for close circuit on transistor - #1
const int solTTLPin2 = 38; // Ouputpin for close circuit on transistor - #2
const int solTTLPin3 = 40; // Ouputpin for close circuit on transistor - #3
const int solTTLPin4 = 42; // Ouputpin for close circuit on transistor - #4

//Capacitance pins
const int lickTTLPin1 = 37;  // Input pin for touch state - #1
const int lickTTLPin2 = 39;  // Input pin for touch state - #2
const int lickTTLPin3 = 41;  // Input pin for touch state - #3
const int lickTTLPin4 = 43;  // Input pin for touch state - #4

const int trialTTLPin = 35; //Sends a TTL at the end of each trial

/*==================== SETTINGS ========================================================*/

//Variables to change
int controlTrials = 0; // First N trials are regular linear track
int numLinTrials = 1; // First 10 trials are linear track
int trialBlocks = 1; // Flag for whether to run control and tone trials in blocks
int trialBlockNum = 30; //If trialBlocks is  1, how many trials per block
int playToneLinear = 0; // play tone in the linear track portion
int training = 1; //Flag for training. If 0, water delivery happens ONLY after a lick is detected on the correct port

// Dont change anything below
int freqRange[2] = {1000, 25000};
float freqExp = (log10(freqRange[1]/freqRange[0]));
int lickmeterState1 = 0;
int lickmeterState2 = 0;
int lickmeterState3 = 0;
int lickmeterState4 = 0;

float cur_pos = 0; // position from Bonsai
float map_pos;// modified position according to trial type
float map_pos2;
int rewardStatus = 0;
int returnRun = 0; // Flag for a return trial (no audio)
int solenoidDur = 3; // in ms
int timeOutDur = 60*1000; // 10 seconds
unsigned long trialCountDown;
unsigned long currentMillis;

int linearTrackTrials = 1; // First N trials are regular linear track
int countTrials = 0;
int correctTrials =0;
int prevTrialCount = 0;
int speakPin = 0;

int gainFactor[3] = {220,320,400};
int maxDim = 400;
int wrongTrial = 0;
int cur_gain; //current gain
int past_gain1; //current gain
int past_gain2; //current gain
int cur_reward = -1;

void setup() {
  // initialize serial communications (for debugging only):
  Serial.begin(57600);
  randomSeed(analogRead(A1));
      
  pinMode(lickPin1, INPUT); // set pin as INPUT
  pinMode(lickPin2, INPUT); // set pin as INPUT
  pinMode(lickPin3, INPUT); // set pin as INPUT
  pinMode(lickPin4, INPUT); // set pin as INPUT

  pinMode(solPin1, OUTPUT); // set pin as OUTPUT
  pinMode(solPin2, OUTPUT); // set pin as OUTPUT
  pinMode(solPin3, OUTPUT); // set pin as OUTPUT
  pinMode(solPin4, OUTPUT); // set pin as OUTPUT

  pinMode(solTTLPin1, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin2, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin3, OUTPUT); // set pin as OUTPUT
  pinMode(solTTLPin4, OUTPUT); // set pin as OUTPUT
    
  pinMode(lickTTLPin1, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin2, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin3, OUTPUT); // set pin as OUTPUT
  pinMode(lickTTLPin4, OUTPUT); // set pin as OUTPUT
  pinMode(trialTTLPin, OUTPUT); // set pin as OUTPUT
  
  pinMode(positionPin, OUTPUT);//
  pinMode(positionAudioPin, OUTPUT);//
}

void loop() {

  // read position from Bonsai
  if (Serial.available() > 0) {      
    cur_pos = Serial.parseFloat();   
  } 
  else{

    lickmeterState1 = digitalRead(lickPin1);
    lickmeterState2 = digitalRead(lickPin2);
    lickmeterState3 = digitalRead(lickPin3);
    lickmeterState4 = digitalRead(lickPin4);
  
    digitalWrite(lickTTLPin1, lickmeterState1);
    digitalWrite(lickTTLPin2, lickmeterState2);
    digitalWrite(lickTTLPin3, lickmeterState3);
    digitalWrite(lickTTLPin4, lickmeterState4);
    
    digitalWrite(solPin1, LOW); // OFF
    digitalWrite(solTTLPin1, LOW); // OFF  
    digitalWrite(solPin2, LOW); // OFF
    digitalWrite(solTTLPin2, LOW); // OFF         
    digitalWrite(solPin3, LOW); // OFF
    digitalWrite(solTTLPin3, LOW); // OFF  
    digitalWrite(solPin4, LOW); // OFF
    digitalWrite(solTTLPin4, LOW); // OFF   
    
    toneAC();
    return;
    
  }

  // If initial trials, just trigger end solenoids, play tones in reverse order in one running direction
  if (linearTrackTrials ==1){  
    analogWrite(positionPin, cur_pos * 200);// Actual position
    //map_pos = map(cur_pos, 0, 1, 1, 0); // reverse position to reverse tone sequence

    // map current position onto tone landscape      
    //int thisPitch = ((cur_pos-0)*((freqRange[1]-freqRange[0])/(1-0)))+freqRange[0];
    int thisPitch = freqRange[0]*(pow(10,(freqExp*cur_pos)));
    
    // Only play tones in one running direction
    if (returnRun==0){
      if (playToneLinear==1){
        analogWrite(positionAudioPin, cur_pos * 200);
        if (speakPin==0){
          toneAC(thisPitch,10);
        }else if (speakPin==1){
          toneAC(thisPitch,10);
        }
      } else{
        analogWrite(positionAudioPin,0); // No tone being played
      }
    }
    else{
      analogWrite(positionAudioPin,0); // No tone being played
    }
    
    //trigger solenoid at the two ends of the track
    if (rewardStatus==0){
      if (returnRun==0){
        if (cur_pos>0.92){
          rewardStatus = 1;
          returnRun = 1;
          digitalWrite(solPin4, HIGH); // ON
          digitalWrite(solTTLPin4, HIGH); // ON  
          digitalWrite(trialTTLPin, HIGH); // ON            
          currentMillis = millis();   
        } 
       }
       else if (returnRun==1){
          if (cur_pos <0.08){
            rewardStatus = 1;
            returnRun = 0;            
            digitalWrite(solPin1, HIGH); // ON
            digitalWrite(solTTLPin1, HIGH); // ON          
            currentMillis = millis();  
            trialCountDown = millis(); 
            prevTrialCount = countTrials;
            countTrials = countTrials+1;
            speakPin = random(0,2);
          } 
       }
    }
    else if (rewardStatus = 1){
      if (millis() - currentMillis >= solenoidDur){
        rewardStatus = 0; 
        digitalWrite(solPin4, LOW); // OFF
        digitalWrite(solTTLPin4, LOW); // OFF  
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
          if (training==0){
            training = 1;
          } else if (training==1){
            training = 0;
         }
        }
     }      
      past_gain2 = past_gain1;
      past_gain1 = cur_gain;     
      cur_gain = random(0,3);
      //don't allow more than 2 0s in a row
      //while((cur_gain==0)&&(past_gain1==0)&&(past_gain1==0)){
      //  cur_gain = random(0,3);
      //}
      prevTrialCount = countTrials;
      if (controlTrials==2){
        cur_reward = random(0,3);
        while (cur_reward==cur_gain){
          cur_reward = random(0,3);
        }
      }      
   }
           
    analogWrite(positionPin, cur_pos * 200);// Actual position
    map_pos = (cur_pos/gainFactor[cur_gain])*maxDim; // Gain position for tone sequence
    
    // map current position onto tone landscape
    //int thisPitch = ((map_pos-0)*((freqRange[1]-freqRange[0])/(1-0)))+freqRange[0];
    int thisPitch = freqRange[0]*(pow(10,(freqExp*map_pos)));
    
    // Only play tones in one running direction
    if (returnRun==0){
      if (map_pos<1.2){
        analogWrite(positionAudioPin, map_pos * 200);//Send analog tone being played to intan
        // play the pitch:
        if (training ==0){
          if (speakPin==0){
            toneAC(thisPitch,10);
          }else if (speakPin==1){
            toneAC(thisPitch,10);
          }
        }
      } else{
        analogWrite(positionAudioPin,0); // No tone being played            
        }
    } else{
      analogWrite(positionAudioPin,0); // No tone being played
    }

    //trigger solenoid 
    if (rewardStatus==0){
      if (returnRun==0){
        if (controlTrials==1){
          if (cur_pos>0.92){
            rewardStatus = 1;
            digitalWrite(solPin4, HIGH); // ON
            digitalWrite(solTTLPin4, HIGH); // ON  
            digitalWrite(trialTTLPin, HIGH); // ON            
            currentMillis = millis();   
          } 
        }
        else if (controlTrials==2){
          map_pos2 = (cur_pos/gainFactor[cur_reward])*maxDim; // Gain position for tone sequence
          if (map_pos2>0.92){
            switch (cur_reward){
              case 0:
                digitalWrite(solPin2, HIGH); // ON
                digitalWrite(solTTLPin2, HIGH); // ON    
                digitalWrite(trialTTLPin, HIGH); // ON  
                break;
              case 1:
                digitalWrite(solPin3, HIGH); // ON
                digitalWrite(solTTLPin3, HIGH); // ON    
                digitalWrite(trialTTLPin, HIGH); // ON  
                break;
              case 2:
                digitalWrite(solPin4, HIGH); // ON
                digitalWrite(solTTLPin4, HIGH); // ON    
                digitalWrite(trialTTLPin, HIGH); // ON  
                break; 
            }
            rewardStatus = 1;
            currentMillis = millis();   
          }           
        }
        else if (training==1){
          if (map_pos>0.92){
            switch (cur_gain){
              case 0:
                digitalWrite(solPin2, HIGH); // ON
                digitalWrite(solTTLPin2, HIGH); // ON    
                digitalWrite(trialTTLPin, HIGH); // ON  
                break;
              case 1:
                digitalWrite(solPin3, HIGH); // ON
                digitalWrite(solTTLPin3, HIGH); // ON    
                digitalWrite(trialTTLPin, HIGH); // ON  
                break;
              case 2:
                digitalWrite(solPin4, HIGH); // ON
                digitalWrite(solTTLPin4, HIGH); // ON    
                digitalWrite(trialTTLPin, HIGH); // ON  
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
              } else if (lickmeterState3==HIGH || lickmeterState4==HIGH){
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
              } else if (lickmeterState2==HIGH || lickmeterState4==HIGH){
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
              } else if (lickmeterState2==HIGH || lickmeterState3==HIGH){
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
            if (cur_pos <0.08){
              rewardStatus = 1;
              // Mark the end of the trial but do not deliver reward
              digitalWrite(solTTLPin1, HIGH); // ON 
              currentMillis = millis();
              trialCountDown = millis();  
              wrongTrial = 0;
              prevTrialCount = countTrials;
              countTrials = countTrials+1;
              speakPin = random(0,2);
           }
          } 
          else if (wrongTrial==0){
            if (cur_pos <0.08){
              rewardStatus = 1;
              digitalWrite(solPin1, HIGH); // ON
              digitalWrite(solTTLPin1, HIGH); // ON 
              currentMillis = millis();
              trialCountDown = millis();  
              prevTrialCount = countTrials;
              countTrials = countTrials+1;
              speakPin = random(0,2);
            }        
          }        
       }                             
    }else if (rewardStatus = 1){
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
  
  digitalWrite(lickTTLPin1, lickmeterState1);
  digitalWrite(lickTTLPin2, lickmeterState2);
  digitalWrite(lickTTLPin3, lickmeterState3);
  digitalWrite(lickTTLPin4, lickmeterState4);
  
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
  Serial.print(cur_reward);    
  Serial.print('_');   
  Serial.print(speakPin);     
  Serial.print('\n');
//  
}
