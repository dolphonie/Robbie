
#include <avr/interrupt.h>
#include "PpmSum.h"
#include "Utils.h"

PpmSum::PpmSum() {}

void PpmSum::attach(int pinNb) {
  this->pinNb = pinNb;
  pinMode(pinNb,OUTPUT);
  digitalWrite(pinNb, HIGH);

  // initialize channels
  for (int i = 0; i<NB_CHANNELS; i++){
    hiTimeTicks[i] = SERVO_CENTER_TICKS - LO_TIME_TICKS;
  }
  
  TCCR1A = 0;
  TCCR1B = _BV(CS11);          // prescale to divide by 8
  TCCR1C = 0;
  TCNT1 = 0;
  OCR1B = FRAME_TIME_TICKS;    // next interrupt at end of frame
  TIMSK1 = _BV(OCIE1B);        // enable OC1B interrupts
  TIFR1 = 0xFF;                // clear all pending interrupts
  mode = INITIAL;
}

void PpmSum::clipMsg(int channel, int us) {
  Utils::printf("Clipping channel %d to %d us.\n", channel, us);
}

void PpmSum::writeMicroseconds(int channel, int us) {
  if (us>MAX_US) {
    us = MAX_US;
    clipMsg(channel, us);
  }
  if (us<MIN_US) {
    us = MIN_US;
    clipMsg(channel, us);
  }
  int hiTime = us * TIMER_TICKS_PER_US - LO_TIME_TICKS;

  uint8_t oldSREG = SREG;
  cli();
  hiTimeTicks[channel] = hiTime;
  SREG = oldSREG;   
}

void PpmSum::dumpChannels() {
  Utils::printf("PpmSum::dumpChannels {\n");
  for (int c=0; c<NB_CHANNELS; c++) {
    int us = (hiTimeTicks[c] + LO_TIME_TICKS) / TIMER_TICKS_PER_US;
    Utils::printf("  Channel %d set to %d us\n", c, us);
  }
  Utils::printf("}\n");
}

inline void PpmSum::handleInterrupt() {
  if (mode==INITIAL) {
    digitalWrite(pinNb, LOW);
    TCNT1 = 0;
    curChannel = 0;
    OCR1B = LO_TIME_TICKS;
    mode = LO;
  } else if (mode==LO){
    digitalWrite(pinNb, HIGH);
    if(curChannel==NB_CHANNELS) {
      OCR1B = FRAME_TIME_TICKS;
      mode = INITIAL;
    } else {
      OCR1B += hiTimeTicks[curChannel];
      curChannel++;
      mode = HI;
    }
  } else {
    digitalWrite(pinNb, LOW);
    OCR1B += LO_TIME_TICKS;
    mode = LO;
  }
}

ISR(TIMER1_COMPB_vect) { 
  PpmSum::ppmSum.handleInterrupt(); 
}

PpmSum PpmSum::ppmSum;
PpmSum& ppmSum = PpmSum::ppmSum;


