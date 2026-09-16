#include <Arduino.h>
#include <BluetoothSerial.h>

// ts is functionslop

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

int CLK_PIN = 27;
int DT_PIN = 33;
int ENC_SW = 32;

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

String computeRotary() {
  clk = digitalRead(CLK_PIN);
  if (clk != prevClk && clk == 1) {
    if (digitalRead(DT_PIN) != clk) {
      encCounter--;
    } else {
      encCounter++;
    }
  }
  prevClk = clk;
  return String(encCounter);
}

String lastConcatString = "";

String concatenateStrings() {
  String out = "";
  out = getActiveButtons() + computeRotary();
  return out;
}

void setup() {
  // set led status to uninitialized

  Serial.begin(115200);
  for (int i = 0; i < 4; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
  SerialBT.begin("Jon Foenem");
  prevClk = digitalRead(CLK_PIN);
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);
}

void loop() {
  String data = concatenateStrings();
  if (data != lastConcatString)
    SerialBT.write((const uint8_t*)data.c_str(), data.length());
  lastConcatString = data;
  Serial.println(data);

  setColorStatus();
  delay(1);
}
