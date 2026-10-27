#include <Servo.h>

/* ==================== CHOOSE ARM ======================================*/

// Change this from 1 to 8 for now
int armToTest[] = {1,2,3,4,5,6,7,8};

int numArms = sizeof(armToTest)/sizeof(armToTest[0]);


/* ==================== PINS ============================================*/

int servoPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
int pumpPins[8]  = {30, 31, 32, 33, 34, 35, 36, 37};
int lickPins[8]  = {38, 39, 40, 41, 42, 43, 44, 45}; //38 and 39 are not working properly

// Open Ephys digital outputs
// Arduino Mega pins 22-26 = PORTA
int ttlPins[5] = {22, 23, 24, 25, 26};


/* ==================== SETTINGS ========================================*/

int sensorTriggeredState = HIGH;

// Continuous rotation servo
int servoStop =120;
int servoMove = 175;

int servoMoveTime = 3000;
int pumpTime = 300;

// How long to hold TTL code
int ttlTime = 50;   // ms


/* ==================== SERVO ===========================================*/

Servo doors[8];


/* ==================== SETUP ===========================================*/

void setup() {

  Serial.begin(57600);

  for (int i = 0; i < numArms; i++) {
    int arm = armToTest[i] - 1;

    pinMode(lickPins[arm], INPUT);
    pinMode(pumpPins[arm], OUTPUT);
    digitalWrite(pumpPins[arm], HIGH);

    doors[arm].attach(servoPins[arm]);
    doors[arm].write(servoStop);
  }

  // Open Ephys TTL outputs
  for (int i = 0; i < 5; i++) {
    pinMode(ttlPins[i], OUTPUT);
    digitalWrite(ttlPins[i], LOW);
  }

  Serial.println("Ready");
}


/* ==================== LOOP ============================================*/

void loop() {

  for (int i = 0; i < numArms; i++) {

    int arm = armToTest[i] - 1;

    if (digitalRead(lickPins[arm]) == sensorTriggeredState) {

      Serial.print("Beam broken: Arm ");
      Serial.println(arm+1);

      // Send arm number as 5-bit code to Open Ephys
      sendTTL(arm+1);

      //Run pump
      digitalWrite(pumpPins[arm], LOW);
      delay(pumpTime);
      digitalWrite(pumpPins[arm], HIGH);

      // Move servo
      doors[arm].write(servoMove);
      delay(servoMoveTime);

      // Stop servo
      doors[arm].write(servoStop);

      // Wait until beam clears
      while (digitalRead(lickPins[arm]) == sensorTriggeredState) {
        delay(10);
      }

      Serial.println("Ready");
    }
  }
}


/* ==================== TTL FUNCTION ====================================*/

void sendTTL(int code) {

  Serial.print("Sending TTL code: ");
  Serial.println(code);

  // Write 5-bit binary code
  for (int i = 0; i < 5; i++) {

    if (code & (1 << i)) {
      digitalWrite(ttlPins[i], HIGH);
    }
    else {
      digitalWrite(ttlPins[i], LOW);
    }
  }

  delay(ttlTime);

  // Clear all TTL lines
  for (int i = 0; i < 5; i++) {
    digitalWrite(ttlPins[i], LOW);
  }
}