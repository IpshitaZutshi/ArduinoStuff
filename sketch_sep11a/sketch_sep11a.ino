#include <Servo.h>

/* ==================== CHOOSE ARM ======================================*/

// Change this from 1 to 4 for now
int armToTest = 2;


/* ==================== PINS ============================================*/

int servoPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
int pumpPins[8]  = {30, 31, 32, 33, 34, 35, 36, 37};
int lickPins[8]  = {38, 39, 40, 41, 42, 43, 44, 45};

// Open Ephys digital outputs
// Arduino Mega pins 22-26 = PORTA
int ttlPins[5] = {22, 23, 24, 25, 26};


/* ==================== SETTINGS ========================================*/

int sensorTriggeredState = HIGH;

// Continuous rotation servo
int servoStop =120;
int servoMove = 175;

int servoMoveTime = 3000;
int pumpTime = 100;

// How long to hold TTL code
int ttlTime = 50;   // ms


/* ==================== SERVO ===========================================*/

Servo door;


/* ==================== SETUP ===========================================*/

void setup() {

  Serial.begin(57600);

  int arm = armToTest - 1;

  pinMode(lickPins[arm], INPUT);
  pinMode(pumpPins[arm], OUTPUT);

  digitalWrite(pumpPins[arm], HIGH);

  door.attach(servoPins[arm]);
  door.write(servoStop);

  // Open Ephys TTL outputs
  for (int i = 0; i < 5; i++) {
    pinMode(ttlPins[i], OUTPUT);
    digitalWrite(ttlPins[i], LOW);
  }

  Serial.print("Testing arm ");
  Serial.println(armToTest);
}


/* ==================== LOOP ============================================*/

void loop() {

  int arm = armToTest - 1;

  if (digitalRead(lickPins[arm]) == sensorTriggeredState) {

    Serial.println("Beam broken");

    // Send arm number as 5-bit code to Open Ephys
    sendTTL(armToTest);

    //Run pump
    digitalWrite(pumpPins[arm], LOW);
    delay(pumpTime);
    digitalWrite(pumpPins[arm], HIGH);

    // Move servo
    door.write(servoMove);
    delay(servoMoveTime);

    // Stop servo
    door.write(servoStop);

    // Wait until beam clears
    while (digitalRead(lickPins[arm]) == sensorTriggeredState) {
      delay(10);
    }

    Serial.println("Ready");
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