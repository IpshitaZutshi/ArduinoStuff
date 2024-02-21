 

int phase = 2;    //phase 1 will mean the water will come out of solenoid as long as IR bream is broken. phase 2 will mean the water will only come out for a specific amount of time and will eventually stop. Then there will be a cooldown
int phase2Delay = 1000; // change to increase to 5000 during mouse training

/* ==================== PINS ============================================*/
//Solenoid Pins
const int solPin1 = 40; // Ouputpin for close circuit on transistor - #1
const int solPin2 = 42; // Ouputpin for close circuit on transistor - #2
const int solPin3 = 44; // Ouputpin for close circuit on transistor - #3
const int solPin4 = 46; // Ouputpin for close circuit on transistor - #4
const int solPin5 = 48; // Ouputpin for close circuit on transistor - #5
const int solPin6 = 50; // Ouputpin for close circuit on transistor - #6
const int solPin7 = 52; // Ouputpin for close circuit on transistor - #7

//Capacitance pins
const int lickPin1 = 41;  // Input pin for IR #1                                                 
const int lickPin2 = 43;  // Input pin for IR #2
const int lickPin3 = 45;  // Input pin for IR #3
const int lickPin4 = 47;  // Input pin for IR #4
const int lickPin5 = 49;  // Input pin for IR #5
const int lickPin6 = 21;  // Input pin for IR #6
const int lickPin7 = 53;  // Input pin for IR #7

/* ==============INTAN PINS ============================================*/

//Solenoid Pins
const int solTTLPin1 = 27; // Ouputpin for close circuit on transistor - #1
const int solTTLPin2 = 29; // Ouputpin for close circuit on transistor - #2
const int solTTLPin3 = 31; // Ouputpin for close circuit on transistor - #3
const int solTTLPin4 = 33; // Ouputpin for close circuit on transistor - #4
const int solTTLPin5 = 35; // Ouputpin for close circuit on transistor - #5
const int solTTLPin6 = 37; // Ouputpin for close circuit on transistor - #6
const int solTTLPin7 = 39; // Ouputpin for close circuit on transistor - #7

//Capacitance pins
const int lickTTLPin1 = 26;  // Input pin for touch state - #1
const int lickTTLPin2 = 28;  // Input pin for touch state - #2
const int lickTTLPin3 = 30;  // Input pin for touch state - #3
const int lickTTLPin4 = 32;  // Input pin for touch state - #4
const int lickTTLPin5 = 34;  // Input pin for touch state - #5
const int lickTTLPin6 = 36;  // Input pin for touch state - #6
const int lickTTLPin7 = 38;  // Input pin for touch state - #7

/*==================== SETTINGS ========================================================*/
int lickmeterState1 = 0;
int lickmeterState2 = 0;
int lickmeterState3 = 0;
int lickmeterState4 = 0;
int lickmeterState5 = 0;
int lickmeterState6 = 0;
int lickmeterState7 = 0;


void setup() {

  Serial.begin(57600);
  randomSeed(analogRead(A1));
  
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

}

void loop() {
  
  lickmeterState1 = digitalRead(lickPin1);
  lickmeterState2 = digitalRead(lickPin2);
  lickmeterState3 = digitalRead(lickPin3);
  lickmeterState4 = digitalRead(lickPin4);
  lickmeterState5 = digitalRead(lickPin5);
  lickmeterState6 = digitalRead(lickPin6);
  lickmeterState7 = digitalRead(lickPin7);
    
  if(phase == 1) {
   if (lickmeterState1==HIGH){
      digitalWrite(solPin1, HIGH); // ON
      digitalWrite(solTTLPin1, HIGH); // ON  
   }else{
      digitalWrite(solPin1, LOW); // ON
      digitalWrite(solTTLPin1, LOW); // ON  
   }                                                                                                      
   if (lickmeterState2==HIGH){
      digitalWrite(solPin2, HIGH); // ON
      digitalWrite(solTTLPin2, HIGH); // ON  
   }else{
      digitalWrite(solPin2, LOW); // ON
      digitalWrite(solTTLPin2, LOW); // ON   
   }
   if (lickmeterState3==HIGH){
      digitalWrite(solPin3, HIGH); // ON
      digitalWrite(solTTLPin3, HIGH); // ON  
   }else{
      digitalWrite(solPin3, LOW); // ON
      digitalWrite(solTTLPin3, LOW); // ON  
   }
   if (lickmeterState4==HIGH){
      digitalWrite(solPin4, HIGH); // ON
      digitalWrite(solTTLPin4, HIGH); // ON  
   }else{
      digitalWrite(solPin4, LOW); // ON
      digitalWrite(solTTLPin4, LOW); // ON  
   }
   if (lickmeterState5==HIGH){
      digitalWrite(solPin5, HIGH); // ON
      digitalWrite(solTTLPin5, HIGH); // ON  
   }else{
      digitalWrite(solPin5, LOW); // ON
      digitalWrite(solTTLPin5, LOW); // ON  
   }
   if (lickmeterState6==HIGH){
      digitalWrite(solPin6, HIGH); // ON
      digitalWrite(solTTLPin6, HIGH); // ON  
   }else{
      digitalWrite(solPin6, LOW); // ON
      digitalWrite(solTTLPin6, LOW); // ON  
   }
   if (lickmeterState7==HIGH){
      digitalWrite(solPin7, HIGH); // ON
      digitalWrite(solTTLPin7, HIGH); // ON  
   }else{
      digitalWrite(solPin7, LOW); // ON
      digitalWrite(solTTLPin7, LOW); // ON  
   }                  
  }
  if(phase == 2) {
    if(lickmeterState1==HIGH){
      digitalWrite(solPin1, HIGH); digitalWrite(solTTLPin1, HIGH); delay(30); digitalWrite(solPin1, LOW); digitalWrite(solTTLPin1, LOW); //water will be given for 3 ms and then stop. Cooldown is 6 seconds
      delay(phase2Delay);
    }
    if(lickmeterState2==HIGH){
      digitalWrite(solPin2, HIGH); digitalWrite(solTTLPin2, HIGH); delay(30); digitalWrite(solPin2, LOW); digitalWrite(solTTLPin2, LOW);        
      delay(phase2Delay);
    }
    if(lickmeterState3==HIGH){
      digitalWrite(solPin3, HIGH); digitalWrite(solTTLPin3, HIGH); delay(30); digitalWrite(solPin3, LOW); digitalWrite(solTTLPin3, LOW);        
      delay(phase2Delay);
    }
    if(lickmeterState4==HIGH){
      digitalWrite(solPin4, HIGH); digitalWrite(solTTLPin4, HIGH); delay(30); digitalWrite(solPin4, LOW); digitalWrite(solTTLPin4, LOW);         
      delay(phase2Delay);
    }    
    if(lickmeterState5==HIGH){
      digitalWrite(solPin5, HIGH); digitalWrite(solTTLPin5, HIGH); delay(30); digitalWrite(solPin5, LOW); digitalWrite(solTTLPin5, LOW);        
      delay(phase2Delay);
    }
    if(lickmeterState6==HIGH){
      digitalWrite(solPin6, HIGH); digitalWrite(solTTLPin6, HIGH); delay(30); digitalWrite(solPin6, LOW); digitalWrite(solTTLPin6, LOW);           
      delay(phase2Delay);
    }
    if(lickmeterState7==HIGH){
      digitalWrite(solPin7, HIGH); digitalWrite(solTTLPin7, HIGH); delay(30); digitalWrite(solPin7, LOW); digitalWrite(solTTLPin7, LOW);           
      delay(phase2Delay);
    }            
  }
  

  digitalWrite(lickTTLPin1, lickmeterState1);
  digitalWrite(lickTTLPin2, lickmeterState2);
  digitalWrite(lickTTLPin3, lickmeterState3);
  digitalWrite(lickTTLPin4, lickmeterState4);
  digitalWrite(lickTTLPin5, lickmeterState5);
  digitalWrite(lickTTLPin6, lickmeterState6);
  digitalWrite(lickTTLPin7, lickmeterState7);
  
}
