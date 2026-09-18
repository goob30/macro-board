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

int CLK_PIN = 33;
int DT_PIN = 32;
int ENC_SW = 35;

int POT_PIN = 34;

int encCounter = 0;
int clk;
int prevClk;

void getButtons(char* out) {
  for (int i = 0; i < BUTTON_COUNT; i++) {
    out[i] = !digitalRead(BUTTON_PINS[i]) ? '1' : '0';
  }
  out[BUTTON_COUNT] = '\0';
}

void getPotVal(char* out) {
  snprintf(out, 6, "P%04d", analogRead(POT_PIN));
}

int lastCount = 0;

// TODO: decide between constant polling + deltas or update-based with abs values
// int getEncoderDelta() {
//   int delta = 0;
//   if (enc.getCount() > lastCount) {
//     delta = enc.getCount() - lastCount;
//   }
// }

void getEncoder(char* out) {
  snprintf(out, 16, "E%d", (int)(enc.getCount() / 4));
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
  // setup()
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);
  enc.attachFullQuad(CLK_PIN, DT_PIN);
  enc.setCount(0);
  pinMode(POT_PIN, INPUT);

  analogReadResolution(8);
}

char data[32];
char lastData[32] = "";

void updateAndSendSerialBT() {
  char butt[5];
  char pot[6];
  char encoder[16];

  getButtons(butt);
  getPotVal(pot);
  getEncoder(encoder);

  snprintf(data, sizeof(data), "B%s%s%s\n", butt, pot, encoder);

  if (strcmp(data, lastData) != 0) {
    Serial.write((const uint8_t*)data, strlen(data));
    strcpy(lastData, data);
  }
}

unsigned long lastSend = 0;

void loop() {
  if (millis() - lastSend >= 30) {
    updateAndSendSerialBT();
    setColorStatus();
    lastSend = millis();
  }
}