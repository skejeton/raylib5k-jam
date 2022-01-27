#ifndef TIMER_H
#define TIMER_H

#include "raylib.h"

typedef struct {
    double counter;
    double treshold;
} Timer;

// Returns true if timer ticked
bool UpdateTimer(Timer *timer);

#endif // TIMER_H
