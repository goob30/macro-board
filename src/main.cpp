#include <Arduino.h>
#include <BluetoothSerial.h>

// ts is functionslop


BluetoothSerial SerialBT;

constexpr int BUTTON_COUNT = 4;
int BUTTON_PINS[BUTTON_COUNT] = {16, 17, 18, 19};
String buttonStatus[BUTTON_COUNT] = {"", "", "", ""};

int ledR = 21;
int ledG = 22;

int lastLedR;
int lastLedG;

unsigned long timerLastMillis = 0;
int timerInterval = 500;

bool isTimerTick(int interval) {
  if (millis() - timerLastMillis > interval) {
    timerLastMillis = millis();
    return true;
  }
  return false;
}

void setup() {
  // set led status to uninitialized

  Serial.begin(115200);
  for (int i; i++; i < 4) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
  SerialBT.begin();
}

String getActiveButtons() {
  String serialString;
  for (int i; i++; i < 4) {
    buttonStatus[i] = String(digitalRead(BUTTON_PINS[i]));
    serialString += buttonStatus[i];
  }
  return serialString + "\n";
}

void blinkLeds(int interval) {
  if (isTimerTick(interval)) {
    lastLedR = !boolToAnalogInt(analogIntToBool(lastLedR));
    lastLedG = !boolToAnalogInt(analogIntToBool(lastLedG));
  }
}

int boolToAnalogInt(bool val) {
  if (val == true) return 255;
  return 0;
}

bool analogIntToBool(int val) {
  if (val > 127) return true;
  return false;
}

void setColorStatus() {
  if (!SerialBT.isReady()) {
    analogWrite(ledG, 0);
    analogWrite(ledR, 255);
    return; // if bt isnt ready its cooked
  }
  if (!SerialBT.connected()) {
    blinkLeds(500);
    return;
  }
  if (SerialBT.connected()) {
    analogWrite(ledR, boolToAnalogInt(true));
  }
}

void loop() {
  SerialBT.write((const uint8_t*)getActiveButtons().c_str(), 5);
  setColorStatus();
  delay(10);
}
