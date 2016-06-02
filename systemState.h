#pragma once

typedef enum{
  play,
  pause,
  stop,
} driveState_t;

typedef struct{
  driveState_t driveState;
  float freeSpace;
  byte percentage;
  bool sdPresent;
  bool redrawNeeded = true;
} systemState_t;

extern systemState_t systemState;

