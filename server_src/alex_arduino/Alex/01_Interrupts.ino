/*
   Setup and start codes for external interrupts and
   pullup resistors.

*/

// Enable pull up resistors on pins 2 and 3
void enablePullups()
{
  // Use bare-metal to enable the pull-up resistors on pins
  // 2 and 3. These are pins PD2 and PD3 respectively.
  // We set bits 2 and 3 in DDRD to 0 to make them inputs.

  DDRD &= 0b11110011;
  PORTD |= 0b00001100;


}


// Functions to be called by INT0 and INT1 ISRs.
void leftISR()
{
  //  leftTicks++;
  //  Serial.print("LEFT: ");
  //  Serial.println(leftTicks);
  if (dir == FORWARD) {
    leftForwardTicks++;
    forwardDist += (unsigned long) ((float) leftForwardTicks / COUNTS_PER_REV * WHEEL_CIRC);
  }
  if (dir == BACKWARD) {
    leftReverseTicks++;
    reverseDist += (unsigned long) ((float) leftReverseTicks / COUNTS_PER_REV * WHEEL_CIRC);
  }
  if (dir == LEFT) {
    leftForwardTicksTurns++;
    leftAngle = (unsigned long)((float) leftForwardTicksTurns / COUNTS_PER_REV * WHEEL_CIRC);
  }
  if (dir == RIGHT) {
    leftReverseTicksTurns++;
    rightAngle = (unsigned long)((float) leftReverseTicksTurns / COUNTS_PER_REV * WHEEL_CIRC);
  }

}

void rightISR()
{
  if (dir == FORWARD) {
    rightForwardTicks++;
  }
  if (dir == BACKWARD) {
    rightReverseTicks++;
  }
  if (dir == LEFT) {
    rightForwardTicksTurns++;
  }
  if (dir == RIGHT) {
    rightReverseTicksTurns++;
  }
  //rightTicks++;

  //Serial.println(leftTicks / COUNTS_PER_REV * WHEEL_CIRC);
}

// Set up the external interrupt pins INT0 and INT1
// for falling edge triggered. Use bare-metal.
void setupEINT()
{
  // Use bare-metal to configure pins 2 and 3 to be
  // falling edge triggered. Remember to enable
  // the INT0 and INT1 interrupts.
  cli();
  EIMSK |= 0b00000011;
  EICRA |= 0b00001010;


  sei();
}

// Implement the external interrupt ISRs below.
// INT0 ISR should call leftISR while INT1 ISR
// should call rightISR.
ISR(INT0_vect) {
  leftISR();
}


ISR(INT1_vect) {
  rightISR();
}
