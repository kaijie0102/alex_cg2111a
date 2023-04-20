/*
   Alex's motor drivers.
*/

  
// Set up Alex's motors. Right now this is empty, but
// later you will replace it with code to set up the PWMs
// to drive the motors.
void setupMotors()
{
  /* Our motor set up is:
        A1IN - Pin 5, PD5, OC0B
        A2IN - Pin 6, PD6, OC0A
        B1IN - Pin 10, PB2, OC1B
        B2In - pIN 11, PB3, OC2A
  */

  // pinMode(LF, OUTPUT);
  // pinMode(RF, OUTPUT);
  // pinMode(LR, OUTPUT);
  // pinMode(RR, OUTPUT);

  // set pins to output
  // LF = PB3, LR = PB2
  DDRB = ( (LFpin) | (LRpin) )
  // RF = PD5, RR = PD6
  DDRD = ( (RFpin) | (RRpin) )


}

// Start the PWM for Alex's motors.
void startMotors()
{

}

// Convert percentages to PWM values
int pwmVal(float speed)
{
  if (speed < 0.0)
    speed = 0;

  if (speed > 100.0)
    speed = 100.0;

  return (int) ((speed / 100.0) * 255.0);
}

// Move Alex forward "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// move forward at half speed.
// Specifying a distance of 0 means Alex will
// continue moving forward indefinitely.
void forward(float dist, float speed)
{
  dir = FORWARD;
  if (dist==0 && speed==0){

    // inching forward
    int val = pwmVal(MPWM+5);
    int val2 = pwmVal(MPWM);
    analogWrite(LF, val);
    analogWrite(RF, val2);
    analogWrite(LR, 0);
    analogWrite(RR, 0);
    delay(MTIME);
    analogWrite(LF, 0);
    analogWrite(RF, 0);
    analogWrite(LR, 0);
    analogWrite(RR, 0);
  } else {

    // moving forward based on distance set
    int val = pwmVal(speed);

    // For now we will ignore dist and move
    // forward indefinitely. We will fix this
    // in Week 9.
    if (dist > 0) {
      deltaDist = dist;
    } else {
      deltaDist = 999999;
    }
    newDist = forwardDist + deltaDist;
    analogWrite(LF, val);
    analogWrite(RF, val);
    analogWrite(LR, 0);
    analogWrite(RR, 0);
  }

  // LF = Left forward pin, LR = Left reverse pin
  // RF = Right forward pin, RR = Right reverse pin
  // This will be replaced later with bare-metal code.

}

// Reverse Alex "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// reverse at half speed.
// Specifying a distance of 0 means Alex will
// continue reversing indefinitely.
void reverse(float dist, float speed)
{
  dir = BACKWARD;

  if (dist==0 && speed ==0){
    // for inching back
    int val = pwmVal(MPWM+5);
    int val2 = pwmVal(MPWM);
    analogWrite(LR, val);
    analogWrite(RR, val2);
    analogWrite(LF, 0);
    analogWrite(RF, 0);
    delay(MTIME);
    analogWrite(LR, 0);
    analogWrite(RR, 0);
    analogWrite(LF, 0);
    analogWrite(RF, 0);
  }else if(dist==1&&speed==1){
    int val = pwmVal(MPWM+5);
    int val2 = pwmVal(MPWM);
    analogWrite(LR, val);
    analogWrite(RR, val2);
    analogWrite(LF, 0);
    analogWrite(RF, 0);
    delay(MTIME*6);
    analogWrite(LR, 0);
    analogWrite(RR, 0);
    analogWrite(LF, 0);
    analogWrite(RF, 0);
    
  }
  else{
    int val = pwmVal(speed);

    // For now we will ignore dist and
    // reverse indefinitely. We will fix this
    // in Week 9.

    // LF = Left forward pin, LR = Left reverse pin
    // RF = Right forward pin, RR = Right reverse pin
    // This will be replaced later with bare-metal code.
    analogWrite(LR, val);
    analogWrite(RR, val);
    analogWrite(LF, 0);
    analogWrite(RF, 0);

  }
}

// Turn Alex left "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn left indefinitely.
void left(float ang, float speed)
{

  dir = LEFT;
  if (ang==0 && speed ==0){
    // for inching left
    int val = pwmVal(MPWM-10);
    int val2 = pwmVal(MPWM-15);
    analogWrite(LF, val);
    analogWrite(RR, val2);
    analogWrite(RF, 0);
    analogWrite(LR, 0);
    delay(MTIME-50);
    analogWrite(LR, 0);
    analogWrite(RF, 0);
    analogWrite(LF, 0);
    analogWrite(RR, 0);
  }else if (ang==180, speed == 0){

    // perform an about turn

    int val = pwmVal(MPWM-10);
    int val2 = pwmVal(MPWM-15);

    for (int i=0;i<12;i++){
      analogWrite(LF, val);
      analogWrite(RR, val);
      analogWrite(RF, 0);
      analogWrite(LR, 0);
      delay(MTIME-50);
      analogWrite(LR, 0);
      analogWrite(RF, 0);
      analogWrite(LF, 0);
      analogWrite(RR, 0);
      delay(MTIME);
    }
    
  }
  else{

    int val = pwmVal(speed);

    // For now we will ignore ang. We will fix this in Week 9.
    // We will also replace this code with bare-metal later.
    // To turn left we reverse the left wheel and move
    // the right wheel forward.
    analogWrite(LR, val);
    analogWrite(RF, val);
    analogWrite(LF, 0);
    analogWrite(RR, 0);
  }
}

// Turn Alex right "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn right indefinitely.
void right(float ang, float speed)
{

  dir = RIGHT;
  if (ang==0 && speed ==0){
    // for inching right
    int val = pwmVal(MPWM-10);
    int val2 = pwmVal(MPWM-15);
    analogWrite(RF, val);
    analogWrite(LR, val2);
    analogWrite(LF, 0);
    analogWrite(RR, 0);
    delay(MTIME-50);
    analogWrite(RF, 0);
    analogWrite(LR, 0);
    analogWrite(LF, 0);
    analogWrite(RR, 0);
  }else{

    int val = pwmVal(speed);

    // For now we will ignore ang. We will fix this in Week 9.
    // We will also replace this code with bare-metal later.
    // To turn right we reverse the right wheel and move
    // the left wheel forward.
    analogWrite(RR, val);
    analogWrite(LF, val);
    analogWrite(LR, 0);
    analogWrite(RF, 0);
  }
}


// Stop Alex. To replace with bare-metal code later.
void stop()
{
  dir = STOP;
  analogWrite(LF, 0);
  analogWrite(LR, 0);
  analogWrite(RF, 0);
  analogWrite(RR, 0);
}

/*
   Alex's setup and run codes

*/

// Clears all our counters
void clearCounters()
{
  leftForwardTicks = 0;
  rightForwardTicks = 0;
  leftReverseTicks = 0;
  rightReverseTicks = 0;

  leftForwardTicksTurns = 0;
  rightForwardTicksTurns = 0;
  leftReverseTicksTurns = 0;
  rightReverseTicksTurns = 0;

  leftRevs = 0;
  rightRevs = 0;
  forwardDist = 0;
  reverseDist = 0;
}

// Clears one particular counter
void clearOneCounter(int which)
{
  clearCounters();
  
    switch (which)
    {
    case 0:
      clearCounters();
      break;

    case 1:
      leftForwardTicks = 0;
      break;

    case 2:
      rightForwardTicks = 0;
      break;

    case 3:
      leftReverseTicks = 0;
      break;

    case 4:
      rightReverseTicks = 0;
      break;
    case 5:
      leftForwardTicksTurns = 0;
      break;

    case 6:
      rightForwardTicksTurns = 0;
      break;
    case 7:
      leftReverseTicksTurns = 0;
      break;
    case 8:
      rightReverseTicksTurns = 0;
      break;
    case 9:
      leftRevs = 0;
      break;
    case 10:
      rightRevs = 0;
      break;
    case 11:
      forwardDist = 0;
      break;
    case 12:
      reverseDist = 0;
      break;
    }
}
// Intialize Vincet's internal states
void initializeState()
{
  clearCounters();
}

void handleCommand(TPacket *command)
{
  
  switch (command->command)
  {
    // For movement commands, param[0] = distance, param[1] = speed.
    case COMMAND_FORWARD:
      sendOK();
      forward((float) command->params[0], (float) command->params[1]);
      break;
      
    // case COMMAND_M_FORWARD:
    //   sendOK();
    //   forward(0, 0);
    //   break;

    /*
       Implement code for other commands here.

    */
    case COMMAND_REVERSE:
      sendOK();
      reverse((float) command->params[0], (float) command->params[1]);
      break;

    case COMMAND_TURN_LEFT:
      sendOK();
      left((float) command->params[0], (float) command->params[1]);
      break;

    case COMMAND_TURN_RIGHT:
      sendOK();
      right((float) command->params[0], (float) command->params[1]);
      break;

    case COMMAND_STOP:
      sendOK();
      stop();
      break;


    case COMMAND_GET_STATS:
      sendOK();
      sendStatus();
      break;

    case COMMAND_CLEAR_STATS:
      sendOK();
      clearCounters();

    case COMMAND_COLOUR:
      sendOK();
      getColor();
//      calibrate();

    default:
      sendBadCommand();
  }
}

void handlePacket(TPacket *packet)
{
  switch (packet->packetType)
  {
    case PACKET_TYPE_COMMAND:
      handleCommand(packet);
      break;

    case PACKET_TYPE_RESPONSE:
      break;

    case PACKET_TYPE_ERROR:
      break;

    case PACKET_TYPE_MESSAGE:
      break;

    case PACKET_TYPE_HELLO:
      break;
  }
}
