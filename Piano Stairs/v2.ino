int sensitivity = 50;
int dly = 20;

const int analogPinLength = 6;
const int analogPins[] = {A0, A1, A2, A3, A4, A5};
const int digitalPin = 5;

bool pinsDetected[analogPinLength+1];

int initialAnalogPinResists[analogPinLength];

void setup() {
  Serial.begin(9600);

  for (int i=0; i<analogPinLength; i++) {
    pinMode(analogPins[i], INPUT);
    initialAnalogPinResists[i] = analogRead(analogPins[i]);
    pinsDetected[i] = false;
  }

  pinMode(digitalPin, INPUT);
  pinsDetected[analogPinLength+1] = false;

}

void loop() {
  for (int i = 0; i<analogPinLength; i++) {
      if (analogRead(analogPins[i]) < initialAnalogPinResists[i] - sensitivity && !pinsDetected[i]) {
        Serial.println(i+1);
        pinsDetected[i] = true;
      }
      if (analogRead(analogPins[i]) > initialAnalogPinResists[i]) {
        pinsDetected[i] = false;
      }
  }

  if (digitalRead(digitalPin) == 0 && !pinsDetected[analogPinLength+1]) {
    Serial.println(analogPinLength+1);
    pinsDetected[analogPinLength+1] = true;
  }
  if (digitalRead(digitalPin) == 1) {
    pinsDetected[analogPinLength+1] = false;
  }

}
