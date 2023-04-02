#include <serialize.h>
#include <stdarg.h>
#include "packet.h"
#include "constants.h"

/*
   Alex's configuration constants
*/
typedef enum {
  STOP = 0,
  FORWARD = 1,
  BACKWARD = 2,
  LEFT = 3,
  RIGHT = 4
} TDirection;

volatile TDirection dir = STOP;

// Number of ticks per revolution from the
// wheel encoder.

#define COUNTS_PER_REV      122

// Wheel circumference in cm.
// We will use this to calculate forward/backward distance traveled
// by taking revs * WHEEL_CIRC

#define WHEEL_CIRC          20.1062

// Motor control pins. You need to adjust these till
// Alex moves in the correct direction
#define LF                  10  // Left forward pin
#define LR                  11   // Left reverse pin
#define RR                  5  // Right reverse pin
#define RF                  6  // Right forward pin

/*
      Alex's State Variables
*/

// Store the ticks from Alex's left and
// right encoders.
//volatile unsigned long leftTicks;
//volatile unsigned long rightTicks;
volatile unsigned long leftForwardTicks;
volatile unsigned long rightForwardTicks;
volatile unsigned long leftReverseTicks;
volatile unsigned long rightReverseTicks;

volatile unsigned long leftForwardTicksTurns;
volatile unsigned long rightForwardTicksTurns;
volatile unsigned long leftReverseTicksTurns;
volatile unsigned long rightReverseTicksTurns;


// Store the revolutions on Alex's left
// and right wheels
volatile unsigned long leftRevs;
volatile unsigned long rightRevs;

// Forward and backward distance traveled
volatile unsigned long forwardDist;
volatile unsigned long reverseDist;

unsigned long deltaDist;
unsigned long newDist;

// 01_Interrupts.ino
void enablePullups();
void leftISR();
void righttISR();
void setupEINT();
ISR(INT0_vect);
ISR(INT1_vect);


// 02_Communications.ino
TResult readPacket(TPacket *packet);
void sendStatus();
void sendMessage(const char *message);
void sendBadPacket();
void sendBadChecksum();
void sendBadCommand();
void sendBadResponse();
void sendOK();
void sendResponse(TPacket *packet);
void waitForHello();

// dbprintf("hello", a);
void dbprintf(char *format, ...) {
  va_list args;
  char buffer[128];
  va_start(args, format);
  vsprintf(buffer, format, args);
  sendMessage(buffer);
}

// 03_Serial_Communications.ino
void setupSerial();
void startSerial();
int readSerial(char *buffer);
void writeSerial(const char *buffer, int len);

// 04_Movement.ino
void setupMotors();
void startMotors();
int pwmVal(float speed);
void forward(float dist, float speed);
void reverse(float dist, float speed);
void left(float ang, float speed);
void right(float ang, float speed);
void stop();
void clearCounters();
void clearOneCounter(int which);
void initializeState();
void handleCommand(TPacket *command);
void handlePacket(TPacket *packet);





void setup() {
  // put your setup code here, to run once:

  cli();
  setupEINT();
  setupSerial();
  startSerial();
  setupMotors();
  startMotors();
  enablePullups();
  initializeState();
  sei();
}

void loop() {

  //digitalWrite(LF,HIGH);
  //digitalWrite(RF,HIGH);
  //digitalWrite(LR,HIGH);
  //digitalWrite(RR,HIGH);
  //analogWrite(LF,50);
  //analogWrite(RF,50);


  // Uncomment the code below for Step 2 of Activity 3 in Week 8 Studio 2

  //forward(0, 100);

  // Uncomment the code below for Week 9 Studio 2
  // put your main code here, to run repeatedly:
  TPacket recvPacket; // This holds commands from the Pi

  TResult result = readPacket(&recvPacket);

  if (result == PACKET_OK)
    handlePacket(&recvPacket);
  else if (result == PACKET_BAD)
  {
    sendBadPacket();
  }
  else if (result == PACKET_CHECKSUM_BAD)
  {
    sendBadChecksum();
  }


}
