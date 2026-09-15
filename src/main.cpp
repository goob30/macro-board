#include <Arduino.h>
#include <BluetoothSerial.h>

// ts is functionslop

BluetoothSerial SerialBT;

constexpr int BUTTON_COUNT = 4;
int BUTTON_PINS[BUTTON_COUNT] = {16, 17, 18, 19};
String buttonStatus[BUTTON_COUNT] = {"", "", "", ""};

int ledR = 21;
int ledG = 22;

int lastLedR = 0;
int lastLedG = 0;

unsigned long timerLastMillis = 0;
int timerInterval = 500;

#define CW 0
#define CCW 1

int CLK_PIN = 21;
int DT_PIN = 22;
int ENC_SW = 23;

int encCounter = 0;
int direction = CW;
int clk;
int prevClk;

bool isTimerTick(int interval) {
  if (millis() - timerLastMillis > interval) {
    timerLastMillis = millis();
    return true;
  }
  return false;
}

String getActiveButtons() {
  String serialString;
  for (int i = 0; i < 4; i++) {
    buttonStatus[i] = String(!digitalRead(BUTTON_PINS[i]));
    serialString += buttonStatus[i];
  }
  return serialString + "\n";
}

int boolToAnalogInt(bool val) {
  if (val == true) return 255;
  return 0;
}

bool analogIntToBool(int val) {
  if (val > 127) return true;
  return false;
}

void blinkLeds(int interval) {
  if (isTimerTick(interval)) {
    lastLedR = !boolToAnalogInt(analogIntToBool(lastLedR));
    lastLedG = !boolToAnalogInt(analogIntToBool(lastLedG));
    analogWrite(lastLedG, ledG);
    analogWrite(lastLedR, ledR);
  }
}

void setColorStatus() {
  if (!SerialBT.isReady()) {
    analogWrite(ledG, 0);
    analogWrite(ledR, 255);
    return;  // if bt isnt ready its cooked
  }
  if (!SerialBT.connected()) {
    blinkLeds(500);
    return;
  }
  if (SerialBT.connected()) {
    analogWrite(ledR, boolToAnalogInt(true));
  }
}

String computeRotary() {
  clk = digitalRead(CLK_PIN);
  if (clk != prevClk && clk == 1) {
    encCounter--;
    direction = CCW;
  } else {
    encCounter++;
    direction = CW;
  }
  // TODO: modify to return structured string and probably only say how many
  // turns it went every serial print and then reset it
}

void setup() {
  // set led status to uninitialized

  Serial.begin(115200);
  for (int i = 0; i < 4; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
  SerialBT.begin("Jon Foenem");
  prevClk = digitalRead(CLK_PIN);
}

void loop() {
  SerialBT.write((const uint8_t*)getActiveButtons().c_str(), 5);
  setColorStatus();
  delay(10);
}
