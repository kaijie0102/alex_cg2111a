/*
   Alex's motor drivers.
*/

  
// Set up Alex's motors. Right now this is empty, but
// later you will replace it with code to set up the PWMs
// to drive the motors.
void setupMotors()
{

  // pinMode(LF, OUTPUT);
  // pinMode(RF, OUTPUT);
  // pinMode(LR, OUTPUT);
  // pinMode(RR, OUTPUT);



  /* Our motor set up is:
        A1IN - Pin 5, PD5, OC0B = RF
        A2IN - Pin 6, PD6, OC0A = RR
        B2In - pIN 9, PB1, OC1A = LF
        B1IN - Pin 10, PB2, OC1B = LR
  */
  TCNT0 = 0;
  TCCR0A = 0b10100001;
  
  OCR0A = 0;
  OCR0B = 0;
  
  TIMSK0 |= 0b110;
  
  // set up timer1
  TCNT1 = 0;
  TCCR1A = 0b10100001;;
  
  OCR1A = 0;
  OCR1B = 0;
  
  TIMSK1 |= 0b110;


}

// Start the PWM for Alex's motors.
void startMotors()
{

  TCCR0B = 0b00000011;
  TCCR1B = 0b00000011;
  
  // set pins to output
  // LF = PB1, LR = PB2
  DDRB = ( (LFpin) | (LRpin) )
  // RF = PD5, RR = PD6
  DDRD = ( (RFpin) | (RRpin) )
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
    OCR0A = 0;
    OCR0B = val;
    OCR1A = val2;
    OCR1B = 0;
    delay(MTIME);
    stop();
  } else {

    // moving forward based on distance set
    int val = pwmVal(speed);

    if (dist > 0) {
      deltaDist = dist;
    } else {
      // moving forward indefiitely
      deltaDist = 999999;
    }
    newDist = forwardDist + deltaDist;
    while (forwardDist!=newDist){
      OCR0A = 0;
      OCR0B = val;
      OCR1A = val;
      OCR1B = 0;
    }

    // finished moving
    
  }
  stop();

  // LF = Left forward pin, LR = Left reverse pin
  // RF = Right forward pin, RR = Right reverse pin
  
  // clear leftForwardTicks counter
  clearOneCounter(1);

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
    OCR0A = val;
    OCR0B = 0;
    OCR1A = 0;
    OCR1B = val2;
    delay(MTIME);

  }else if(dist==1&&speed==1){
    int val = pwmVal(MPWM+5);
    int val2 = pwmVal(MPWM);
    OCR0A = val;
    OCR0B = 0;
    OCR1A = 0;
    OCR1B = val2;
    delay(MTIME*6);
    
  }
  else{
    int val = pwmVal(speed);

    if (dist > 0) {
      deltaDist = dist;
    } else {
      // moving backwards indefiitely
      deltaDist = 999999;
    }
    newDist = reverseDist + deltaDist;
    while (reverseDist!=newDist){
      OCR0A = val;
      OCR0B = 0;
      OCR1A = 0;
      OCR1B = val;
    }

    // finished moving
  }
  stop();
  // clear leftReverseTicks counter
  clearOneCounter(2);

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
    OCR0A = 0;
    OCR0B = val;
    OCR1A = 0;
    OCR1B = val2;
    
    delay(MTIME-50);

  }else if (ang==180, speed == 0){

    // perform an about turn

    int val = pwmVal(MPWM-10);
    int val2 = pwmVal(MPWM-15);

    for (int i=0;i<12;i++){
      OCR0A = 0;
      OCR0B = val;
      OCR1A = 0;
      OCR1B = val2;
      delay(MTIME-50);
      stop();
      delay(MTIME); // delay to avoid too much movement
    }
    
  }
  else{

    int val = pwmVal(speed);
    if (ang > 0) {
      deltaAngle = dist;
    } else {
      // moving backwards indefiitely
      deltaAngle = 999999;
    }
    newAngle = leftAngle + deltaAngle;
    while (leftAngle!=newAngle){
      // analogWrite(LR, val);
      // analogWrite(RF, val);
      // analogWrite(LF, 0);
      // analogWrite(RR, 0);
      OCR0A = 0;
      OCR0B = val;
      OCR1A = 0;
      OCR1B = val2;
    
    }

    // finished moving
  }
  stop();

  // clear leftForwardTicksTurns counter
  clearOneCounter(5);
    
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
    // analogWrite(RF, val);
    // analogWrite(LR, val2);
    // analogWrite(LF, 0);
    // analogWrite(RR, 0);

    OCR0A = val;
    OCR0B = 0;
    OCR1A = val2;
    OCR1B = 0;
    delay(MTIME-50);
    stop();
  }else{

    int val = pwmVal(speed);
    if (ang > 0) {
      deltaAngle = ang;
    } else {
      // moving backwards indefiitely
      deltaAngle = 999999;
    }
    newAngle = rightAngle + deltaAngle;
    while (rightAngle!=newAngle){
      // analogWrite(RR, val);
      // analogWrite(LF, val);
      // analogWrite(LR, 0);
      // analogWrite(RF, 0);
      OCR0A = val;
      OCR0B = 0;
      OCR1A = val;
      OCR1B = 0;
    }

    // finished moving
    stop();
    
  }

  // clear leftReverseTicksTurns counter
  clearOneCounter(7);
}


// Stop Alex. To replace with bare-metal code later.
void stop()
{
  dir = STOP;
  // analogWrite(LF, 0);
  // analogWrite(LR, 0);
  // analogWrite(RF, 0);
  // analogWrite(RR, 0);
  OCR0A = 0;
  OCR0B = 0;
  OCR1A = 0;
  OCR1B = 0;
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
