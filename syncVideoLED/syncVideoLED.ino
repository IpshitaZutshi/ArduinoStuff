// IO Pins definitions
int intanPin1 = 2;
int intanPin2 = 3;
int Npx = 4;
int ledPin1 = 8;
int ledPin2 = 9;
int pulseDur = 100; //duration of pulse on, in ms
int latencyDur[4] = {500,1500,700,2000};
int goSig = 0;


void setup() {
  Serial.begin(57600);
  pinMode(intanPin1, OUTPUT); //define both IO pins as outputs
  pinMode(intanPin2, OUTPUT);
  pinMode(Npx, OUTPUT);
  pinMode(ledPin1, OUTPUT); //define both IO pins as outputs
  pinMode(ledPin2, OUTPUT);  
  randomSeed(analogRead(0));
}

void loop() {

  if (Serial.available() > 0) {       
    delay(3000);   
    goSig = 1;  
  } else {
    goSig = 0;
    return;
  }

  if (goSig == 1){
    digitalWrite(intanPin1, HIGH);  
    digitalWrite(intanPin2, HIGH); 
    digitalWrite(Npx, HIGH); 
    digitalWrite(ledPin1, HIGH); 
    digitalWrite(ledPin2, HIGH);  
    delay(pulseDur);
    digitalWrite(intanPin1, LOW);  
    digitalWrite(intanPin2, LOW); 
    digitalWrite(Npx, LOW); 
    digitalWrite(ledPin1, LOW); 
    digitalWrite(ledPin2, LOW); 

    //Random delay between blinks
    int curDelay = random(0,4);
    delay(latencyDur[curDelay]);
  }
}
