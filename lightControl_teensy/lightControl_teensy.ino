const int dirPin = 8;
const int pwmPin = 9;

String inputString = "";

float currentBrightness = 0;
float targetBrightness = 0;

unsigned long lastCommandTime = 0;
const unsigned long timeoutMs = 250;

// smoothing strength:
// smaller = smoother/slower
// larger = faster/more responsive
const float alpha = 0.08;

void setup() {
  pinMode(dirPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);

  digitalWrite(dirPin, HIGH);

  analogWriteFrequency(pwmPin, 10000);
  analogWrite(pwmPin, 0);

  Serial.begin(115200);
}

void loop() {

  // Read complete lines from Bonsai
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n') {
      targetBrightness = inputString.toInt();
      targetBrightness = constrain(targetBrightness, 0, 128);

      lastCommandTime = millis();
      inputString = "";
    }
    else if (c != '\r') {
      inputString += c;
    }
  }

  // If Bonsai stops sending, turn light off
  if (millis() - lastCommandTime > timeoutMs) {
    targetBrightness = 0;
  }

  // Exponential smoothing
  currentBrightness =
      currentBrightness +
      alpha * (targetBrightness - currentBrightness);

  analogWrite(pwmPin, (int)currentBrightness);
}