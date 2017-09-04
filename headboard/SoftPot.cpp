#include "Arduino.h"
#include "SoftPot.h"

#include <math.h>

SoftPot::SoftPot(int pin)
{
  // pinMode(pin, OUTPUT);
  _pin = pin;
}

float SoftPot::readPos()
{
  index = (index + 1) % NUM_SOFT_POT_SAMPLES;
  float pos = (((float)analogRead(_pin))- 306.0)/1.18;

  x[index] = millis();
  y[index] = pos;
  
  Serial.print("pos=");Serial.println(pos);
  
  simpLinReg(NUM_SOFT_POT_SAMPLES);
  return y[index];
}


/**
 * Don't calculate velocity - instead output a position that varies relative to the initial touch point.
 * 
 */


void SoftPot::simpLinReg( int n) {
  // pass x and y arrays (pointers), lrCoef pointer, and n.  The lrCoef array is comprised of the slope=lrCoef[0] and intercept=lrCoef[1].  n is length of the x and y arrays.
  // http://en.wikipedia.org/wiki/Simple_linear_regression

  // initialize variables
  float x_sum=0;
  float y_sum=0;
  float xy_sum=0;
  float xsq_sum=0;
  float ysq_sum=0;
  int samples = 0;
  boolean contactLost = false;
  
  // calculations required for linear regression
  for (int i=0; i<n; i++){
    float timeDiff = (float)(x[index] - x[i]);
    float pos = y[i];
    //Serial.print("timeDiff=");Serial.println(timeDiff);

    boolean inContact = pos < 450;

    if (!inContact && !contactLost) {
      Serial.print("Contact lost at ");Serial.println(x[i]);
    }
    
    contactLost |= !inContact;
    
    if (timeDiff < 100 && inContact) {
      ++samples;
      
      x_sum+=timeDiff;
      y_sum+=pos;
      xy_sum+=timeDiff*pos;
      xsq_sum+=timeDiff*timeDiff;
      ysq_sum+=pos*pos;
    }
  }
  //Serial.print("samples=");Serial.println(samples);
  
  if (samples > 5 && !contactLost) {
    float xbar=x_sum/samples;
    float ybar=y_sum/samples;
    float xybar=xy_sum/samples;
    float xsqbar=xsq_sum/samples;
    float r = xy_sum / sqrt(xsq_sum*ysq_sum);
    
    // simple linear regression algorithm
    lrCoef[0]=(xybar-xbar*ybar)/(xsqbar-xbar*xbar);
    lrCoef[1]=ybar-lrCoef[0]*xbar;
  
    int speed = round(lrCoef[0] * 100);
  
    if (abs(speed) > 2) {
      //Serial.print("r=");Serial.println(r);
      total+=speed;
      // Serial.print("speed=");Serial.println(speed);
      Serial.print("total=");Serial.println(total);
    }
    //Serial.print(" l1=");Serial.println(lrCoef[1]);
  }

  if (samples ==1) {
    Serial.println("...");
  }
}
