/*
   Setup and start codes for serial communications

*/
// Set up the serial connection. For now we are using
// Arduino Wiring, you will replace this later
// with bare-metal code.
void setupSerial()
{
  // To replace later with bare-metal.
  UBRR0L = 103;
  UBRR0H = 0;
  UCSR0C = 0b00000110; //set to 8N1
  UCSR0A = 0;
  // Serial.begin(9600);
}

// Start the serial connection. For now we are using
// Arduino wiring and this function is empty. We will
// replace this later with bare-metal code.

void startSerial()
{
  // Empty for now. To be replaced with bare-metal code
  // later on.
  UCSR0B = 0b10111000;

}

// Read the serial port. Returns the read character in
// ch if available. Also returns TRUE if ch is valid.
// This will be replaced later with bare-metal code.

int readSerial(char *buffer)
{

  int count = 0;

  // while (Serial.available())
  //   buffer[count++] = Serial.read();

  TBufferResult result;

  do
  {
    result = readBuffer(&_recvBuffer, &buffer[count]);

    if (result == BUFFER_OK){
      count++;
    } 
  } while (result == BUFFER_OK);

  return count;
}
  
// Write to the serial port. Replaced later with
// bare-metal code

void writeSerial(const char *buffer, int len)
{
  // Serial.write(buffer, len);
  TBufferResult result = BUFFER_OK;
  for(int i = 1; i < len; i += 1){
    result = writeBuffer(&_xmitBuffer, buffer[i]);
  }

  UDR0 = buffer[0];

  UCSR0B |= 0b00100000;
}
