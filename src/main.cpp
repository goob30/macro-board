#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ESP32Encoder.h>

// ts is functionslop

// TODO
// add screen support
// add potentiometer lowk

ESP32Encoder enc;
BluetoothSerial SerialBT;

constexpr int BUTTON_COUNT = 4;
int BUTTON_PINS[BUTTON_COUNT] = {16, 17, 18, 19};
String buttonStatus[BUTTON_COUNT] = {"", "", "", ""};

int ledR = 4;
int ledG = 5;

int lastLedR = 0;
int lastLedG = 0;

unsigned long timerLastMillis = 0;
int timerInterval = 500;

int CLK_PIN = 25;
int DT_PIN = 26;
int ENC_SW = 27;

int POT_PIN = 33;

int encCounter = 0;
int clk;
int prevClk;

enum LedStat {
  RED,
  AMBER,
  AMBER_FLASH,
  GREEN
};

void writeLeds(int status) {
  switch (status) {
    case RED:
      digitalWrite(ledR, lastLedR);
      return;
    case AMBER:
      return;
    case AMBER_FLASH:
      return;
    case GREEN:
      return;
    default:
      return;
  }
}

bool isTimerTick(int interval) {
  if (millis() - timerLastMillis > interval) {
    timerLastMillis = millis();
    return true;
  }
  return false;
}

int boolToAnalogInt(bool val) {
  if (val == true) return 255;
  return 0;
}

bool analogIntToBool(int val) {
  if (val > 127) return true;
  return false;
}

void blinkLedMultiple(int interval) {
  if (isTimerTick(interval)) {
    lastLedR = !lastLedR;
    lastLedG = !lastLedG;
    analogWrite(ledG, boolToAnalogInt(lastLedG));
    analogWrite(ledR, boolToAnalogInt(lastLedR));
  }
}
void setColorStatus() {
  if (!SerialBT.isReady()) {
    analogWrite(ledG, 0);
    analogWrite(ledR, 255);
    return;  // if bt isnt ready its cooked
  }
  if (!SerialBT.connected()) {
    blinkLedMultiple(500);
    return;
  }
  if (SerialBT.connected()) {
    analogWrite(ledR, boolToAnalogInt(true));
  }
}

int getPotVal() {
  int val;
  val = analogRead(POT_PIN);
  return val;
}

void setup() {
  // set led status to uninitialized

  Serial.begin(115200);
  for (int i = 0; i < 4; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
  SerialBT.begin("Jon Foenem");

  ESP32Encoder::useInternalWeakPullResistors = puType::up;
  enc.attachHalfQuad(CLK_PIN, DT_PIN);
  enc.setCount(0);

  pinMode(POT_PIN, INPUT);
}

void loop() {
  // if (data != lastConcatString && SerialBT.connected())
  //   SerialBT.write((const uint8_t*)data.c_str(), data.length());
  // lastConcatString = data;
  Serial.println(data);

  setColorStatus();
  delay(10);
}
