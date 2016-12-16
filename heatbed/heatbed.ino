int heartbeat = 0;
int blinkStatus = 0;

int sensorPin = A4;
int heartbeatPin = 13;
int ledPin = 5;
int relaisPin = 7;
int buttonPin = 3;

int wantHeat = 0;
int heating = 0;
int blinking = 0;
int sensorValue = 1023;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(heartbeatPin, OUTPUT);
  pinMode(relaisPin, OUTPUT);
}

void showStatus() {
  if (heartbeat) {
    heartbeat = 0;
    digitalWrite(heartbeatPin, LOW);
  } else {
    heartbeat = 1;
    digitalWrite(heartbeatPin, HIGH);
  }
  if (blinking) {
    if (blinkStatus) {
      blinkStatus = 0;
      digitalWrite(ledPin, LOW);
    } else {
      blinkStatus = 1;
      digitalWrite(ledPin, HIGH);
    }
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void heatBed() {
  // ADC 145 - 0.709V - ~16.5kOhm - ~60 deg C
  if (sensorValue > 145) {
    // start heating if bed temperature is below ~60 deg C
    digitalWrite(relaisPin, 1);
  }
  // ADC 105 - .538V - ~12kOhm - ~65 deg C
  if (sensorValue < 110) {
    // stop heating if bed temperature is above ~65 deg C
    digitalWrite(relaisPin, 0);
  }
  // ADC 170 - .831V - ~20kOhm - ~55 deg C
  if (sensorValue > 170) {
    blinking = 1;
  } else {
    blinking = 0;
  }
}

void coolBed() {
  digitalWrite(relaisPin, 0);
  // ADC 170 - 1.25V  - ~33kOhm - ~45 deg C
  if (sensorValue < 255) {
    blinking = 1;
  } else {
    blinking = 0;
  }
}

void loop() {
  showStatus();
  wantHeat = digitalRead(buttonPin);
  sensorValue = analogRead(sensorPin);
  
  if (wantHeat) {
    heatBed();
  } else {
    coolBed();
  }
  
  delay(1000);
}
