#include <stdio.h>
#include <Arduino.h>
#include "Utils.h"

void Utils::printf(const char* fmt, ... ) {
  const int lineSz = 200;
  char line[lineSz];
  va_list(ap);
  va_start(ap, fmt);
  vsnprintf(line, lineSz, fmt, ap);
  va_end(ap);
  Serial.print(line);
}

char ans[10];
char* Utils::f2s(float f){
  int i = f*1000;
  int ipart = i/1000;
  int fpart = abs(i%1000);
  sprintf(ans,"%d.%03d",ipart,fpart);
  return ans;
}
