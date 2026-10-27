#include <Servo.h>

// ================= SETTINGS =================
// Change this setting and re-upload to switch modes.
const bool HABITUATION = false;  // true = habituation; false = trials
const int MOUSE_ID = 1;
const int TRAINING_DAY = 2;
const int CORRECT_ARM = 2;
const int MAX_TRIALS = 15;
const bool ENABLE_PROBE = false;
const int MIN_TRIALS_BEFORE_PROBE = 5;

// Habituation arms: edit this list to select which arms give water.
const int armToTest[] = {1, 2, 3, 4, 5, 6, 7, 8};
const int numArms = sizeof(armToTest) / sizeof(armToTest[0]);
const int ALL_SENSOR_PINS[8] = {38, 39, 40, 41, 42, 43, 44, 45};
const int ALL_SERVO_PINS[8] = {2, 3, 4, 5, 6, 7, 8, 9};
const int HABITUATION_SERVO_STOP = 120;
Servo habituationDoors[8];

// Trial arms. Arm 4 is the probe starting arm only.
const int NUM_ARMS = 3;
const int arms[NUM_ARMS]       = {2, 6, 8};
const int sensorPins[NUM_ARMS] = {39, 43, 45}; // Arm 2 moved from faulty pin 39
const int pumpPins[NUM_ARMS]   = {31, 35, 37};
const int ALL_PUMP_PINS[8] = {30, 31, 32, 33, 34, 35, 36, 37};

// Door commands preserved from your corrected trial code.
// IMPORTANT: these assume positional servos. On continuous rotation servos,
// write() controls speed/direction, not door position; confirm hardware first.
// Set CONTROL_DOORS false to test sensors/pumps with manual doors.
const bool CONTROL_DOORS = true;
const int HOME_SERVO_PIN = 9;   // Arm 8
const int PROBE_SERVO_PIN = 5;  // Arm 4
const int DOOR_OPEN = 130;
const int DOOR_CLOSED = 170;
const unsigned long DOOR_DELAY = 200;
const unsigned long WATER_DELAY = 300;
const unsigned long DEBOUNCE_MS = 20;
const int SENSOR_ACTIVE = HIGH; // Matches working habituation code
const int PUMP_ON = LOW;
const int PUMP_OFF = HIGH;

// Open Ephys: physical arm number as a 5-bit code, least significant bit on 22.
const int ttlPins[5] = {22, 23, 24, 25, 26};
const unsigned long TTL_TIME_MS = 50;

Servo homeDoor, probeDoor;
bool sensorReady[NUM_ARMS] = {false, false, false};
int trial = 0;
int probeTrial = 0;

void giveWater(int i);
int checkPoke();
void sendTTL(int code);

void setup() {
  Serial.begin(57600);
  Serial.setTimeout(50);

  // Explicitly turn ALL eight pumps off, including arm 4 on pin 33.
  for (int i = 0; i < 8; i++) {
    digitalWrite(ALL_PUMP_PINS[i], PUMP_OFF);
    pinMode(ALL_PUMP_PINS[i], OUTPUT);
  }
  for (int i = 0; i < NUM_ARMS; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(ttlPins[i], LOW);
    pinMode(ttlPins[i], OUTPUT);
  }

  if (HABITUATION) {
    for (int i = 0; i < numArms; i++) {
      int arm = armToTest[i] - 1;
      if (arm < 0 || arm >= 8) {
        Serial.println("ERROR: habituation arms must be 1 through 8");
        while (true) delay(100);
      }
      pinMode(ALL_SENSOR_PINS[arm], INPUT);
      habituationDoors[arm].write(HABITUATION_SERVO_STOP);
      habituationDoors[arm].attach(ALL_SERVO_PINS[arm]);
    }
    Serial.println("HABITUATION: Ready");
  } else {
    if (CONTROL_DOORS) {
      homeDoor.write(DOOR_CLOSED);
      probeDoor.write(DOOR_CLOSED);
      homeDoor.attach(HOME_SERVO_PIN);
      probeDoor.attach(PROBE_SERVO_PIN);
      delay(DOOR_DELAY);
    }
    if (ENABLE_PROBE && MIN_TRIALS_BEFORE_PROBE >= 0 &&
        MAX_TRIALS > MIN_TRIALS_BEFORE_PROBE) {
      // A15 must be unused/unconnected for this seed source.
      randomSeed(analogRead(A15) ^ micros());
      probeTrial = random(MIN_TRIALS_BEFORE_PROBE + 1, MAX_TRIALS + 1);
    }
    Serial.println("READY: send INFO or START with Newline");
  }
}

void loop() {
  if (HABITUATION) {
    // Working habituation behavior: reward, then wait for that beam to clear.
    // Sensor scanning pauses during TTL, water delivery and this wait.
    for (int i = 0; i < numArms; i++) {
      int arm = armToTest[i] - 1;
      if (digitalRead(ALL_SENSOR_PINS[arm]) == SENSOR_ACTIVE) {
        Serial.print("Beam broken: Arm "); Serial.println(arm + 1);
        sendTTL(arm + 1);
        unsigned long t = millis();
        digitalWrite(ALL_PUMP_PINS[arm], PUMP_ON);
        delay(WATER_DELAY);
        digitalWrite(ALL_PUMP_PINS[arm], PUMP_OFF);
        Serial.print("WATER,ARM,"); Serial.print(arm + 1);
        Serial.print(",TIME_MS,"); Serial.println(t);
        while (digitalRead(ALL_SENSOR_PINS[arm]) == SENSOR_ACTIVE) delay(10);
        Serial.println("Ready");
      }
    }
    return;
  }

  if (!Serial.available()) return;
  String command = Serial.readStringUntil('\n');
  command.trim();
  if (command == "INFO") {
    Serial.print("PROBE_TRIAL,"); Serial.println(probeTrial);
    return;
  }
  if (trial >= MAX_TRIALS || command != "START") return;

  if (trial == 0) {
    Serial.print("PROBE_TRIAL,"); Serial.println(probeTrial);
  }

  trial++;
  bool probe = (probeTrial > 0 && trial == probeTrial);
  Servo &door = probe ? probeDoor : homeDoor;
  unsigned long startTime = millis();

  if (CONTROL_DOORS) {
    door.write(DOOR_OPEN);
    delay(DOOR_DELAY);
  }
  for (int i = 0; i < NUM_ARMS; i++) sensorReady[i] = false;

  Serial.print("TRIAL_START,MOUSE,"); Serial.print(MOUSE_ID);
  Serial.print(",DAY,"); Serial.print(TRAINING_DAY);
  Serial.print(",TRIAL,"); Serial.print(trial);
  Serial.print(",START_ARM,"); Serial.print(probe ? 4 : 8);
  Serial.print(",TIME_MS,"); Serial.println(startTime);

  // This preserves your blocking trial logic: waits until a choice occurs.
  // START/INFO are not processed during this wait. Send only one START per trial.
  int chosen = -1;
  while (chosen == -1) {
    int i = checkPoke();
    if (i != -1 && (arms[i] == 2 || arms[i] == 6)) chosen = i;
  }

  unsigned long choiceTime = millis();
  if (CONTROL_DOORS) door.write(DOOR_CLOSED);
  // Same arm-code convention as habituation, also on incorrect/probe choices.
  sendTTL(arms[chosen]);
  if (CONTROL_DOORS && DOOR_DELAY > TTL_TIME_MS) {
    delay(DOOR_DELAY - TTL_TIME_MS);
  }

  bool reward = !probe && arms[chosen] == CORRECT_ARM;
  if (reward) giveWater(chosen);

  Serial.print("TRIAL_END,MOUSE,"); Serial.print(MOUSE_ID);
  Serial.print(",DAY,"); Serial.print(TRAINING_DAY);
  Serial.print(",TRIAL,"); Serial.print(trial);
  Serial.print(",PROBE,"); Serial.print(probe ? 1 : 0);
  Serial.print(",START_MS,"); Serial.print(startTime);
  Serial.print(",CHOICE_MS,"); Serial.print(choiceTime);
  Serial.print(",CHOICE,"); Serial.print(arms[chosen]);
  Serial.print(",CORRECT_ARM,"); Serial.print(CORRECT_ARM);
  Serial.print(",REWARD,"); Serial.println(reward ? 1 : 0);
  if (trial == MAX_TRIALS) Serial.println("SESSION_COMPLETE");
}

void giveWater(int i) {
  unsigned long t = millis();
  digitalWrite(pumpPins[i], PUMP_ON);
  delay(WATER_DELAY);
  digitalWrite(pumpPins[i], PUMP_OFF);
  Serial.print("WATER,ARM,"); Serial.print(arms[i]);
  Serial.print(",TIME_MS,"); Serial.println(t);
}

// A sensor must clear before another reward. A held beam gives one reward.
int checkPoke() {
  for (int i = 0; i < NUM_ARMS; i++) {
    if (digitalRead(sensorPins[i]) != SENSOR_ACTIVE) {
      sensorReady[i] = true;
    } else if (sensorReady[i]) {
      delay(DEBOUNCE_MS);
      if (digitalRead(sensorPins[i]) == SENSOR_ACTIVE) {
        sensorReady[i] = false;
        return i;
      }
    }
  }
  return -1;
}

void sendTTL(int code) {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ttlPins[i], (code & (1 << i)) ? HIGH : LOW);
  }
  delay(TTL_TIME_MS);
  for (int i = 0; i < 5; i++) digitalWrite(ttlPins[i], LOW);
}
