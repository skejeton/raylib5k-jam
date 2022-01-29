#ifndef INTERP_H
#define INTERP_H

#include <math.h>

static inline double InterpolateSineBare(double x) 
{
    return -(cos(M_PI * x) - 1) / 2;
}

static inline double InterpolateSine(double low, double x, double hi) 
{
    return InterpolateSineBare(x / (low - hi)) * (low - hi) + low;
}

#endif
