 

int phase = 2;    //phase 1 will mean the water will come out of solenoid as long as IR bream is broken. phase 2 will mean the water will only come out for a specific amount of time and will eventually stop. Then there will be a cooldown
int phase2Delay = 1000; // change to increase to 5000 during mouse training

/* ==================== PINS ============================================*/
//Solenoid Pins
const int valve1 = 40; // Ouputpin for close circuit on transistor - #1
const int valve7 = 52; // Ouputpin for close circuit on transistor - #2

//Capacitance pins
const int IRsensorL = 24;  // Input pin for IR #1 (left/short)                                          
const int IRsensorR = 22;  // Input pin for IR #2 (right/long)

/* ==============INTAN PINS ============================================*/

//Solenoid Pins
const int valve1Intan = 26; // Ouputpin for close circuit on transistor - #1
const int valve7Intan = 38; // Ouputpin for close circuit on transistor - #7

/*==================== SETTINGS ========================================================*/
int lickmeterState1 = 0;
int lickmeterState7 = 0;


void setup() {

  Serial.begin(57600);
  randomSeed(analogRead(A1));
  
  pinMode(IRsensorL, INPUT); // set pin as INPUT
  pinMode(IRsensorR, INPUT); // set pin as INPUT
  
  pinMode(valve1, OUTPUT); // set pin as OUTPUT
  pinMode(valve7, OUTPUT); // set pin as OUTPUT
    
  pinMode(valve1Intan, OUTPUT); // set pin as OUTPUT
  pinMode(valve7Intan, OUTPUT); // set pin as OUTPUT
  
  digitalWrite(valve1, LOW); // OFF
  digitalWrite(valve1Intan, LOW); // OFF  
  digitalWrite(valve7, LOW); // OFF
  digitalWrite(valve7Intan, LOW); // OFF        
}

void loop() {
  
  lickmeterState1 = digitalRead(IRsensorR);
  lickmeterState7 = digitalRead(IRsensorL);
 
    
  if(phase == 1) {
   if (lickmeterState1==LOW){
      digitalWrite(valve1, HIGH); // ON
      digitalWrite(valve1Intan, HIGH); // ON  
   }else{
      digitalWrite(valve1, LOW); // ON
      digitalWrite(valve1Intan, LOW); // ON  
   }                                                                                                      
   if (lickmeterState7==LOW){
      digitalWrite(valve7, HIGH); // ON
      digitalWrite(valve7Intan, HIGH); // ON  
   }else{
      digitalWrite(valve7, LOW); // ON
      digitalWrite(valve7Intan, LOW); // ON   
   }
  }
  if(phase == 2) {
    if(lickmeterState1==LOW){
      digitalWrite(valve1, HIGH); digitalWrite(valve1Intan, HIGH); delay(30); digitalWrite(valve1, LOW); digitalWrite(valve1Intan, LOW); //water will be given for 3 ms and then stop. Cooldown is 6 seconds
      delay(phase2Delay);
    }
    if(lickmeterState7==LOW){
      digitalWrite(valve7, HIGH); digitalWrite(valve7Intan, HIGH); delay(30); digitalWrite(valve7, LOW); digitalWrite(valve7Intan, LOW);        
      delay(phase2Delay);
    }
  }
  

//  digitalWrite(lickTTLPin1, lickmeterState1);
//  digitalWrite(lickTTLPin2, lickmeterState2);
//  digitalWrite(lickTTLPin3, lickmeterState3);
//  digitalWrite(lickTTLPin4, lickmeterState4);
//  digitalWrite(lickTTLPin5, lickmeterState5);
//  digitalWrite(lickTTLPin6, lickmeterState6);
//  digitalWrite(lickTTLPin7, lickmeterState7);
  
}
