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
      digitalWrite(ledR, 255);
      digitalWrite(ledG, 0);
      return;
    case AMBER:
      digitalWrite(ledR, 255);
      digitalWrite(ledG, 255);
      return;
    case GREEN:
      digitalWrite(ledR, 0);
      digitalWrite(ledG, 255);
      return;
    case NONE:
      digitalWrite(ledR, 0);
      digitalWrite(ledG, 0);
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

void setColorStatus(int status, bool isFlash) {
  static bool lastWrite = false;
  if (!isFlash) {
    writeLeds(status);
    return;
  }
  if (isTimerTimeout(500)) lastWrite = !lastWrite;
  writeLeds((lastWrite ? NONE : status));
}