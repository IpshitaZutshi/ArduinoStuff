// IO Pins definitions
int OUTPUT_PIN = 51;
int pulseDur = 100; //duration of pulse on, in ms
int latencyDur[6] = {500,1500,700,2000,3000,4000};


void setup() {
  Serial.begin(57600);
  pinMode(OUTPUT_PIN, OUTPUT); // initialize digital pin
  pinMode(LED_BUILTIN, OUTPUT);
  randomSeed(analogRead(0));
}

void loop() {

    digitalWrite(OUTPUT_PIN, HIGH);   // initialize with 20 ms pulse
    digitalWrite(LED_BUILTIN, HIGH); 
    delay(pulseDur);
    digitalWrite(OUTPUT_PIN, LOW);   // initialize with 20 ms pulse
    digitalWrite(LED_BUILTIN, LOW); 


    //Random delay between blinks
    int curDelay = random(0,6);
    delay(latencyDur[curDelay]);
}
