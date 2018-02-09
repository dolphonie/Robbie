#include "ServoR.h"
#include "ppmSum.h"

char* noName = "";

ServoR::ServoR(int channel) {
  this->channel=channel;
  center = 0;
  name = noName;
  reverse = false;
  profileNb = -1;
}

void ServoR::setName(char* name) {
  this->name = name;
}

char* ServoR::getName() {
  return name;
}

void ServoR::setCenter(int centerDegrees) {
  this->center = centerDegrees;
}

void ServoR::setReverse(boolean reverse) {
  this->reverse = reverse;
}

void ServoR::setCalibration(int profileNb) {
  this->profileNb = profileNb;
}

void ServoR::setAngle(float angle) {
  if (reverse){
    angle = -angle;
  }
  angle+=center;
  int pulseWidth = ServoCalibration::degreesToPulseWidth (angle,profileNb);
  ppmSum.writeMicroseconds(channel,pulseWidth);
}

