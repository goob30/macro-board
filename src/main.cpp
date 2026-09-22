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

constexpr int BUTTON_COUNT = 12;
int BUTTON_PINS[BUTTON_COUNT] = {16, 17, 18, 19};
int MX_X[3] = {25, 26, 27};      // TODO
int MX_Y[4] = {16, 17, 18, 19};  // TODO

int posX = 0;
int posY = 0;
int btnIdx = 0;

int scanX = 0;

int CLK_PIN = 33;
int DT_PIN = 32;
int ENC_SW = 35;

int POT_PIN = 34;

int encCounter = 0;
int clk;
int prevClk;

// TODO: update for matrix where idx = y * 3 + x (y and x would be a set of matrix tx/rx active pins)
void getButtons(char* out) {
  for (int i = 0; i < 3; i++) {  // write high to
    digitalWrite(MX_X[i], i == scanX ? LOW : HIGH);
  }
  for (int y = 0; y < 4; y++) {
    int idx = y * 3 + scanX;
    out[idx] = !digitalRead(MX_Y[y]) ? '1' : '0';
  }
  scanX = (scanX + 1) % 3;
}

void getPotVal(char* out) {
  snprintf(out, 6, "P%04d", analogRead(POT_PIN));
}

void getEncoder(char* out) {
  snprintf(out, 16, "E%d", (int)(enc.getCount() / 4));
}
std::string lastConcatString = "";

void getColorStatus() {
  // 
}

void setup() {
  // set led status to uninitialized
  setColorStatus(RED, false);

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