#include "led.h"
#include "Arduino.h"

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
    
}