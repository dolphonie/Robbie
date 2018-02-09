#include <Arduino.h>
#include "ServoCalibration.h"
#include "Utils.h"

static const int NB_POSITIONS = 5;
static const int NB_SERVOS = 6;

// pulse widths (microseconds) measured from 9303
static const int MINUS60_TIME = 900;
static const int ZERO_TIME = 1500;
static const int PLUS60_TIME = 2100;
static const int PLUS30_TIME = (PLUS60_TIME + ZERO_TIME) / 2;
static const int MINUS30_TIME = (MINUS60_TIME + ZERO_TIME) / 2;

static const int calibrations[][NB_POSITIONS] = {
    {785, 1050, 1400, 1790, 2130  },
    {850, 1150, 1500, 1850, 2180  },
    {740, 1040, 1375, 1770, 2100  },
    {810, 1120, 1475, 1850, 2190  },
    {850, 1150, 1520, 1890, 2240  },
    {900, 1200, 1510, 1860, 2200  }
};

static const int standardCalibration[] = {
  MINUS60_TIME, MINUS30_TIME, ZERO_TIME, PLUS30_TIME, PLUS60_TIME
};

int ServoCalibration::degreesToPulseWidth(float degrees, int calProfileNb) {
  // range check calProfileNb
  if (calProfileNb>5) {
     Utils::printf("Invalid servo profile: %d\n",calProfileNb);
     calProfileNb = -1;
  }
  if (calProfileNb<-1) {
    Utils::printf("Invalid servo profile: %d\n",calProfileNb);
    calProfileNb = -1;
  }
  const int* profile;
  if (calProfileNb == -1) {
    profile = standardCalibration;
  }else{
    profile = calibrations[calProfileNb];
  }
  // linear interpolation for profile using map function
  if (degrees>=30) {
    return map(degrees,30,60,profile[3],profile[4]);
  } else if (degrees>=0) {
    return map(degrees,0,30,profile[2],profile[3]);
  } else if (degrees>=-30) {
    return map(degrees,-30,0,profile[1],profile[2]);
  } else { 
    return map(degrees,-60,-30,profile[0],profile[1]);
  }
}


