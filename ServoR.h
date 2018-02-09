/* Servo for robot code */

#ifndef SERVOR_H
#define SERVOR_H

#include <Arduino.h>
#include "ServoCalibration.h"

class ServoR {
 public:
  ServoR(int channel);
  void setName(char* name);
  char* getName();
  void setCenter(int centerDegrees);
  void setReverse(boolean reverse);
  void setCalibration(int profileNb);
  
  // angle varies from -60 to +60
  void setAngle(float angleDegrees);
  
 private:
  int channel;     // start numbering from 0
  char* name;
  int center;      // degrees
  boolean reverse;
  int profileNb;
};


#endif // SERVOR_H
