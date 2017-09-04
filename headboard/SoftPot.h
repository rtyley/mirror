#ifndef SoftPot_h
#define SoftPot_h

#include "Arduino.h"

#define NUM_SOFT_POT_SAMPLES 256


class SoftPot
{
  public:
    SoftPot(int pin);
    float readPos();
  private:
    int _pin;
    int total = 0;
    unsigned long now;
    int index = 0;
    
    unsigned long x[NUM_SOFT_POT_SAMPLES];
    float y[NUM_SOFT_POT_SAMPLES];
    
    float lrCoef[2]={0,0};
    void simpLinReg( int n);
};

#endif
