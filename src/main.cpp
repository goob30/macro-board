#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ESP32Encoder.h>
#include <stdio.h>
#include <string.h>
#include "led.h"
// ts is functionslop

// TODO
// add screen support
// add potentiometer lowk

ESP32Encoder enc;
BluetoothSerial SerialBT;

constexpr int BUTTON_COUNT = 4;
int BUTTON_PINS[BUTTON_COUNT] = {16, 17, 18, 19};

int CLK_PIN = 25;
int DT_PIN = 26;
int ENC_SW = 27;

int POT_PIN = 33;

int encCounter = 0;
int clk;
int prevClk;


std::string getButtons() {
  std::string out = "";
  for (int i = 0; i < 4; i++) {
    out += !digitalRead(BUTTON_PINS[i]) ? '1' : '0';
  }
  return "B" + out;
}


std::string getPotVal() {
  char out[5];
  snprintf(out, sizeof(out), "%04d", analogRead(POT_PIN));
  return "P" + std::string(out);
}

int lastCount = 0;

// TODO: decide between constant polling + deltas or update-based with abs values
// int getEncoderDelta() {
//   int delta = 0;
//   if (enc.getCount() > lastCount) {
//     delta = enc.getCount() - lastCount;
//   }
// }

std::string getEncoder() {
  return "E" + std::to_string(enc.getCount());
}

std::string lastConcatString = "";


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

  analogReadResolution(8);
}

std::string data = "";

void loop() {
  data = "";
  data += getButtons() + getPotVal() + getEncoder() + "\n";
  if (data != lastConcatString && SerialBT.connected())
    SerialBT.write((const uint8_t*)data.c_str(), data.length());
  lastConcatString = data;
  // Serial.println(data);

  setColorStatus();
  delay(10);
}
