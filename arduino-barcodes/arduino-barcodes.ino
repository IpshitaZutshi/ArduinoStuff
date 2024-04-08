/*
  32-bit digital barcodes for synchronizing data streams
*/

int OUTPUT_PIN = 51;
int BARCODE_BITS = 32;
int INTER_BARCODE_INTERVAL = 5; // s

long barcode;
int goSig = 0;
unsigned long currentMillis;

void setup() {

  Serial.begin(57600);
  pinMode(OUTPUT_PIN, OUTPUT); // initialize digital pin
  pinMode(LED_BUILTIN, OUTPUT);
  randomSeed(analogRead(0));

  barcode = random(0, pow(2,BARCODE_BITS));
}

void loop() {

  if (Serial.available() > 0) {          
    goSig = 1;  
    currentMillis = millis(); 
  } else {
    goSig = 0;
    return;
  }

  if (goSig == 1){

    delay(1000);
    barcode += 1; // increment barcode on each cycle
  
    digitalWrite(OUTPUT_PIN, HIGH);   // initialize with 20 ms pulse
    delay(20);
    digitalWrite(OUTPUT_PIN, LOW);    // set to low value for 20 ms
    delay(20);
  
    for (int i = 0; i < BARCODE_BITS; i++)
    {
      if ((barcode >> i) & 1){
        digitalWrite(OUTPUT_PIN,HIGH);
        digitalWrite(LED_BUILTIN, HIGH); 
      }else{
        digitalWrite(OUTPUT_PIN,LOW);
        digitalWrite(LED_BUILTIN, LOW);  
      }
       delay((INTER_BARCODE_INTERVAL - 1) * 32 / BARCODE_BITS);
    }
    
    digitalWrite(OUTPUT_PIN, LOW);         // write final low value
    digitalWrite(LED_BUILTIN, LOW);  
    delay(INTER_BARCODE_INTERVAL * 1000);  // wait for interval
  }
}
