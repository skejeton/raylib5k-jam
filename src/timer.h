#include "raylib.h"
#ifndef TIMER_H
#define TIMER_H

typedef struct {
    double counter;
    double treshold;
} Timer;

// Returns true if timer ticked
bool UpdateTimer(Timer *timer)
{
    timer->counter += GetFrameTime();
    if (timer->counter > timer->treshold) {
        timer->counter = 0;
        return true;
    }
    return false;
}   

#endif // TIMER_H
