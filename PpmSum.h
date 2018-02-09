/**
   Generate a 9 channel composite PPM signal to be sent to the buddy
   box input of a JR 9303 transmitter.

   Signal consists of 10 inverted pulses. Pulse width is 394 us. The
   channel times are encoded in the spacing of pulses. A channel's 
   pulse width is measured from the leading edge of one pulse to the
   leading edge of the next.

   This is a Singleton Class
*/

#ifndef PPMSUM_H
#define PPMSUM_H

#include <Arduino.h>

class PpmSum {
 public:
  void attach(int pinNb);
  void writeMicroseconds(int channel, int us);

  void dumpChannels(); // for debugging
  void handleInterrupt();

  static PpmSum ppmSum;
 private:
  enum Mode {LO, HI, INITIAL};
  
  static const unsigned int 
    NB_CHANNELS = 9,
    CPU_CLOCKS_PER_US = clockCyclesPerMicrosecond(),
    TIMER_TICKS_PER_US = CPU_CLOCKS_PER_US / 8, // assumes timer prescaler of 8
    LO_TIME_TICKS = 394 * TIMER_TICKS_PER_US,
    FRAME_TIME_TICKS = 20000 * TIMER_TICKS_PER_US,
    SERVO_CENTER_TICKS = 1500 * TIMER_TICKS_PER_US,
    MIN_US = 900,
    MAX_US = 2100;
    
  int hiTimeTicks[NB_CHANNELS];
  int pinNb;
  int curChannel;
  Mode mode;

  void clipMsg(int channel, int us);
  
  PpmSum();
  void trace(unsigned int i);
};

extern PpmSum& ppmSum;

#endif // PPMSUM_H
