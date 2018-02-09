#include <stdio.h>

#include "Utils.h"
#include "PpmSum.h"
#include "ServoR.h"

const int LINE_SZ = 80;
const int DEGREES_BOWED = 60;
const int DEGREES_SQUATED = 50;
const int DEGREES_JUMPED_INCREMENT = 10;
const int RIGHT_BIAS = 0;
const int LEFT_BIAS = 8;
 
int TIME = 340;//MILISECONDS TIME OF 1 STROKE
float ROCK_TRAVEL = 18;//DEGREES
float STRIDE_TRAVEL =20;
/*for carpet say " p340 18 20 " minus the quotes
( it matters which piece of carpet you use,
it's kinda a work in progress)*/
int degrees;
int minusDegrees;
int angle;
int minusAngle;

ServoR 
  rightHip(0),
  rightKnee(1),
  rightAnkle(2),
  leftHip(3),
  leftKnee(4),
  leftAnkle(5);

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(-1);
  Utils::printf("Beginning Robbie program\n");
  ppmSum.attach(13);
  pinMode(12,OUTPUT);
  // set servo calibration curves
  rightHip.setCalibration(0);
  rightKnee.setCalibration(1);
  rightAnkle.setCalibration(2);
  //leftHip.setCalibration(5);
  leftKnee.setCalibration(4);
  leftAnkle.setCalibration(3);
  
  rightHip.setName("right hip");    
  rightKnee.setName("right knee");
  rightAnkle.setName("right ankle");
  leftHip.setName("left hip");
  leftKnee.setName("left knee");
  leftAnkle.setName("left ankle");

  // knee servos need double travel (0 - 120 degrees) in one direction
  rightKnee.setCenter(-55);
  leftKnee.setCenter(35);
  leftHip.setCenter(-8);
  rightHip.setCenter(-8);
  
  // reverse servos so normal motion uses positive angle
  leftKnee.setReverse(true);
  //leftHip.setReverse(true);
  leftAnkle.setReverse(true);
  
  // Robbie stands up when all servos are 0
  rightHip.setAngle(0);
  rightKnee.setAngle(0);
  rightAnkle.setAngle(0);
  leftHip.setAngle(0);
  leftKnee.setAngle(0);
  leftAnkle.setAngle(0);
  
}

// read a line, null terminate string 
void readLine(char buf[], int bufLen) {
  int len = Serial.readBytesUntil('\n', buf, bufLen-1);
  buf[len] = 0;
}

ServoR* abbrevToServo(char* abbrev) {
  if (strcmp(abbrev,"rk") == 0){
    return &rightKnee;
  }if (strcmp(abbrev,"rh") == 0){
    return &rightHip;
  } if (strcmp(abbrev,"ra") == 0){
    return &rightAnkle;
  } if (strcmp(abbrev,"lk") == 0){
    return &leftKnee;
  } if (strcmp(abbrev,"lh") == 0){
    return &leftHip;
  } if (strcmp(abbrev,"la") == 0){
    return &leftAnkle;
  }else{
    return NULL;
  }
}


void angleCommand(char* lineBuf) {
  char joint[LINE_SZ];
  int angle;
  sscanf(lineBuf+1, "%s %d", joint, &angle);
  ServoR* servo = abbrevToServo(joint);
  if (servo==NULL) {
    // user typed bad servo name
    Utils::printf("You typed in a bad servo name, you dummy,%s!\n",lineBuf);
    return;
  }
  Utils::printf("Set %s to %d degrees\n", servo->getName(), angle);
  servo->setAngle(angle);
  ppmSum.dumpChannels();
}

void paramCommand(char* lineBuf) {
   char joint[LINE_SZ];
  int intRockTravel;
  int intStrideTravel;
  
  sscanf(lineBuf+1, "%d %d %d", &TIME, &intRockTravel, &intStrideTravel);
  ROCK_TRAVEL = intRockTravel;
  STRIDE_TRAVEL = intStrideTravel;
  printVar();
  walk();
}

void printVar(){
  Utils::printf("Time = %d, Rock travel = %s,",
                TIME, Utils::f2s(ROCK_TRAVEL) );
  Utils::printf("Stride Travel: %s\n", Utils::f2s(STRIDE_TRAVEL));
}

void doCommand() {
  if (Serial.available()!=0) {
    char lineBuf[LINE_SZ];
    readLine(lineBuf, LINE_SZ);
    
    if (lineBuf[0]=='a') {
      angleCommand(lineBuf);
    } else if (lineBuf[0]=='p') {
      paramCommand(lineBuf);
    } else if (lineBuf[0]=='b') {
      bow();
    } else if (lineBuf[0]=='s') {
      squat();
    }else if( lineBuf[0] == 'j'){
      jump();
    } else if (lineBuf[0] == 'w'){
      walk();
    } else if(lineBuf[0] == 'i'){
      sit();
    }else {
      Utils::printf("Unrecognized command: %s\n", lineBuf);
    }
  }
}

void sit(){
  Utils::printf("Robbie sits\n");
  for(int i =0; i<65 ; i++){
   rightHip.setAngle(degrees);
   leftHip.setAngle(degrees);
   leftKnee.setAngle(degrees);
   rightKnee.setAngle(degrees);
   degrees++;
   delay(20);
  }
  delay(2000);
  for(int i =0; i<65 ; i++){
   rightHip.setAngle(degrees);
   leftHip.setAngle(degrees);
   leftKnee.setAngle(degrees);
   rightKnee.setAngle(degrees);
   degrees--;
   delay(20);
  }
}
void bow() {
  Utils::printf("Robbie takes a bow\n");
  int degrees = 0;
  for(int i = 0; i<DEGREES_BOWED; i++){
   rightHip.setAngle(degrees);
   leftHip.setAngle(degrees);
   rightAnkle.setAngle(-degrees/3);
   leftAnkle.setAngle(-degrees/3);
   degrees++;
   delay(20);
  }
  for(int i = 0; i<DEGREES_BOWED; i++){
   rightHip.setAngle(degrees);
   leftHip.setAngle(degrees);
   rightAnkle.setAngle(-degrees/3);
   leftAnkle.setAngle(-degrees/3);
   degrees--;
   delay(20);
  }
}

void jump(){
  Utils::printf("Robbie jumps\n");
  int degrees = 0;
  for(int i =0; i<DEGREES_SQUATED ; i++){
   rightHip.setAngle(degrees);
   leftHip.setAngle(degrees);
   leftKnee.setAngle(degrees*2);
   rightKnee.setAngle(degrees*2);
   rightAnkle.setAngle(degrees);
   leftAnkle.setAngle(degrees);
   degrees++;
   delay(20);
  }
  for(int i =0; i<DEGREES_SQUATED/DEGREES_JUMPED_INCREMENT ; i++){
   rightHip.setAngle(degrees);
   leftHip.setAngle(degrees);
   leftKnee.setAngle(degrees*2);
   rightKnee.setAngle(degrees*2);
   rightAnkle.setAngle(degrees);
   leftAnkle.setAngle(degrees);
   degrees-=DEGREES_JUMPED_INCREMENT;
   delay(20);
  }
  rightHip.setAngle(0);
   leftHip.setAngle(0);
   leftKnee.setAngle(0);
   rightKnee.setAngle(0);
   rightAnkle.setAngle(0);
   leftAnkle.setAngle(0);
   
}
 
//a postive rock makes the robot lean towards the right.
// A positive stride makes the right leg go forward
void moveLegs(float squat, float rock, float stride){
  float rightSquat = squat + RIGHT_BIAS;
  float leftSquat = squat +LEFT_BIAS;
  rightHip.setAngle(rightSquat+rock+stride);
  rightKnee.setAngle((rightSquat+rock)*2);
  rightAnkle.setAngle(rightSquat+rock-stride);
  leftHip.setAngle(leftSquat-rock-stride);
  leftKnee.setAngle((leftSquat-rock)*2);
  leftAnkle.setAngle(leftSquat-rock+stride);
}


void walk(){
  Utils::printf("Robbie Walks!\n");
  float rock = 0;
  float stride = 0;
  float squat =0;
  int cyclesRepeated = 10;
  int numCycles = TIME/20/2*2; 
  float rockIncr = ROCK_TRAVEL/numCycles;
  float strideIncr = STRIDE_TRAVEL /numCycles;
 
  //squats halfway
  for(int i =0; i<numCycles/2; i++){
    moveLegs(squat, 0, 0);
    squat+=rockIncr;
    delay(20);
  }
  Utils::printf("numCycles=%d\n", numCycles);
  Utils::printf("Half squat: %s\n", Utils::f2s(squat));
 
  //staggers feet halfway
  for (int i =0; i<numCycles/2; i++){
      moveLegs(squat,rock,stride);
      rock-=rockIncr;
      stride-=strideIncr;  
      delay(20);
  }
  //walking
  for(int i =0; i<cyclesRepeated; i++){
    for (int i =0; i <numCycles; i++){
      moveLegs(squat,rock,stride);
      rock+=rockIncr;
      stride+=strideIncr;
      digitalWrite(12,LOW);
      delay(20);
      digitalWrite(12,HIGH);
    } 
    for (int i =0; i<numCycles; i++){
      moveLegs(squat,rock,stride);
      rock-=rockIncr;
      stride-=strideIncr;
      digitalWrite(12,LOW);
      delay(20);
      digitalWrite(12,HIGH);
    }
  }
  //unstaggers feet
  for (int i =0; i<numCycles/2; i++){
      moveLegs(squat,rock,stride);
      rock+=rockIncr;
      stride+=strideIncr;
      delay(20);
  }
  //unsquats feet
  for (int i=0; i <numCycles/2; i++){
    moveLegs(squat,0,0);
    squat-=rockIncr;
    delay(20);
  }
   Utils::printf("numCycles=%d\n", numCycles);
  Utils::printf("Half squat: %s\n", Utils::f2s(squat));
  Utils::printf("Half stride: %s\n", Utils::f2s(stride));
  printVar();
  //moveLegs(0,0,0);
}

void squat() {
  Utils::printf("Robbie squats\n");
  int degrees = 0;
  for(int i =0; i<DEGREES_SQUATED ; i++){
   rightHip.setAngle(degrees);
   leftHip.setAngle(degrees);
   leftKnee.setAngle(degrees*2);
   rightKnee.setAngle(degrees*2);
   rightAnkle.setAngle(degrees);
   leftAnkle.setAngle(degrees);
   degrees++;
   delay(20);
  }
  for(int i =0; i<DEGREES_SQUATED ; i++){
   rightHip.setAngle(degrees);
   leftHip.setAngle(degrees);
   leftKnee.setAngle(degrees*2);
   rightKnee.setAngle(degrees*2);
   rightAnkle.setAngle(degrees);
   leftAnkle.setAngle(degrees);
   degrees--;
   delay(20);
  }
}

void loop() {
  
  doCommand();
  
}


