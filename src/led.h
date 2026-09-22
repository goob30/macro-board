#pragma once

extern int ledR;
extern int ledG;

extern int lastLedR;
extern int lastLedG;

extern unsigned long timerLastMillis;
extern int timerInterval;

enum LedStat {
  RED,
  AMBER,
  GREEN,
  NONE
};

void setColorStatus(int color, bool isFlash);