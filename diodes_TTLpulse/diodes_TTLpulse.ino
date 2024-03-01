/* Send a TTL pulse from the arduino to LED controller. When input is high, all LEDs are on, else all are off */

// Pins
const int diodesPin = 2;
const int activationPin = 12; // From the control behaviour arduino

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(diodesPin, OUTPUT);
  pinMode(activationPin, INPUT);
  
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off   
  digitalWrite(diodesPin, LOW);   // turn the diodes off  
}

void loop() {

   if (digitalRead(activationPin) == HIGH){
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on   
      digitalWrite(diodesPin, HIGH);   // turn the diodes on
   } else if (digitalRead(activationPin) == LOW){
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off   
      digitalWrite(diodesPin, LOW);   // turn the diodes off
   }

}
