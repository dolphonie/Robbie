#ifndef SERVO_CALIBRATION_H
#define SERVO_CALIBRATION_H

class ServoCalibration {
 public:
  // if calProfileNb==1 then use standard calibration.
  static int degreesToPulseWidth(float degrees, int calProfileNb);
  
};

#endif
