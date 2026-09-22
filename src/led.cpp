#include "led.h"

#include "Arduino.h"

int ledR = 4;
int ledG = 5;

int lastLedR = 0;
int lastLedG = 0;

unsigned long timerLastMillis = 0;
int timerInterval = 500;

void writeLeds(int status) {
  switch (status) {
    case RED:
      lastLedR = 255;
      lastLedG = 0;
      digitalWrite(ledR, lastLedR);
      digitalWrite(ledG, lastLedG);
      return;
    case AMBER:
      lastLedG = 255;
      lastLedR = 255;
      digitalWrite(ledR, lastLedR);
      digitalWrite(ledG, lastLedG);
      return;
    case GREEN:
      lastLedR = 0;
      lastLedG = 255;
      digitalWrite(ledR, lastLedR);
      digitalWrite(ledG, lastLedG);
      return;
    case NONE:
      lastLedR = 0;
      lastLedG = 0;
      digitalWrite(ledR, lastLedR);
      digitalWrite(ledG, lastLedG);
      return;
    default:
      return;
  }
}

bool isTimerTimeout(int interval) {
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
  if (isTimerTimeout(interval)) {
    lastLedR = !lastLedR;
    lastLedG = !lastLedG;
    analogWrite(ledG, boolToAnalogInt(lastLedG));
    analogWrite(ledR, boolToAnalogInt(lastLedR));
  }
}

void setColorStatus(int status, bool isFlash) {
  static bool lastWrite = false;
  if (!isFlash) {
    writeLeds(status);
    return;
  }
  if (isTimerTimeout(500)) lastWrite = !lastWrite;
  writeLeds((lastWrite ? NONE : status));
}