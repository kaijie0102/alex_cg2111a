#include "Arduino.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*
  #define LEDPIN - Port B pin 0
  #define S3PIN - Port D pin 7  
  #define S2PIN - Port D pin 4
  #define S1PIN - Port D pin 1
  #define S0PIN - Port D pin 0 
*/

volatile long _timerTicks;

void setupExt() {
  
  cli();
  DDRB &= 0b11111110; // pin 8 as input for color sensor
  DDRD |= 0b10010011; // pin 7, 4, 1, 0 as outputs from the color sensor
  PORTD &= 11111100; // set pin1(S1) and pin0(S0) to low for 20% scaling for color sensor
  sei();
  
  Serial.begin(9600);
}


//baremetal implementation for delay function
void alex_delay (int dur) {
  
  cli();
  TCCR1B = 0b00000000;
  TCCR1A = 0b00000000;
  TIMSK1 = 0b10;
  OCR1A = 15999; // 1ms
  TCNT1 = 0;
  _timerTicks = 0; // Reset counter
  TCCR1B = 0b00001001;
  sei();

  while (_timerTicks < dur); //the actual delay function
  
}


ISR(TIMER1_COMPA_vect){
  _timerTicks++;
}



//bare metal implementation for pulseIn function
unsigned long alex_pulseIn() {
    while (PINB & 0b00000001); //wait for pin to go to low

    unsigned long pulseStart = micros();
    while (!(PINB & 0b00000001)); //wait for pin to go to high again

    return micros() - pulseStart;
}


void calibrate() {
    unsigned long red_reading = 0;
    unsigned long blue_reading = 0;
    unsigned long green_reading = 0;
    int SAMPLES = 50;
  
    unsigned long redval, blueval, greenval;
    dbprintf("Calibrating...");

    //collecting multiple readings so that an average can be taken
    for (int i = 0; i < SAMPLES; i++) {
        //setting both S3 and S2 to LOW to get the reading for red color
        PORTD &= 0b01101111; 
        redval = alex_pulseIn();
        alex_delay(100);
  
        //setting S3 to HIGH and S2 to LOW to get the reading for blue color
        PORTD &= 0b11101111;
        PORTD |= 0b10000000;
        blueval = alex_pulseIn();
        alex_delay(100);
        
        //setting both S3 and S2 to HIGH to get the reading for green color
        PORTD |= 0b10010000;
        greenval = alex_pulseIn();
        alex_delay(100);
  
        dbprintf("Rv: %d ", (redval));
        dbprintf("Bv: %d ", (blueval));
        dbprintf("Gv: %d", (greenval));
    
        red_reading += redval;
        blue_reading += blueval;
        green_reading += greenval;
    }
  
    //taking the average reading of all colors
    red_reading = red_reading / SAMPLES;
    blue_reading = blue_reading / SAMPLES;
    green_reading = green_reading / SAMPLES;
  
    
    dbprintf("Red: %d", (red_reading));
    dbprintf("Green: %d", (green_reading));
    dbprintf("Blue: %d", (blue_reading));
  
  
    alex_delay(500);
}


void getColor() {
  int redMin = 80; // Red minimum value
  int redMax = 600; // Red maximum value
  int greenMin = 60; // Green minimum value
  int greenMax = 580; // Green maximum value
  int blueMin = 40; // Blue minimum value
  int blueMax = 400; // Blue maximum value
  int SAMPLES = 20;

  unsigned long red_reading = 0;
  unsigned long blue_reading = 0;
  unsigned long green_reading = 0;
  unsigned long redval, blueval, greenval;

  for (int i = 1; i <= 2; i++) {
    dbprintf("Detecting colour: Round %d",i);

    //collecting multiple readings so that an average can be taken
    for (int i = 0; i < SAMPLES; i++) {
      
      //setting both S3 and S2 to LOW to get the reading for red color
      PORTD &= 0b01101111; 
      redval = alex_pulseIn();
      alex_delay(100);

      //setting S3 to HIGH and S2 to LOW to get the reading for blue color
      PORTD &= 0b11101111;
      PORTD |= 0b10000000;
      blueval = alex_pulseIn();
      alex_delay(100);
      
      //setting both S3 and S2 to HIGH to get the reading for green color
      PORTD |= 0b10010000;
      greenval = alex_pulseIn();
      alex_delay(100);

      red_reading += redval;
      blue_reading += blueval;
      green_reading += greenval;
    }
    
    //taking the average reading and making modifications to the value
    red_reading = red_reading / SAMPLES;
    blue_reading = blue_reading / SAMPLES;
    green_reading = green_reading / SAMPLES;
 
    red_reading = map( red_reading, redMin, redMax, 255, 0);
    green_reading = map( green_reading, greenMin, greenMax, 255, 0);
    blue_reading = map( blue_reading, blueMin, blueMax, 255, 0);
    
    red_reading = red_reading > 255 ? 255 : 
                  red_reading < 0 ? 0 : 
                  red_reading;
    green_reading = green_reading > 255 ? 255 : 
                    green_reading < 0 ? 0 : 
                    green_reading;
    blue_reading = blue_reading > 255 ? 255 : 
                   blue_reading < 0 ? 0 : 
                   blue_reading;

    
    //printing the final values to terminal to assist in judgement of color detected
    dbprintf("Red: %d", (red_reading));
    dbprintf("Green: %d", (green_reading));
    dbprintf("Blue: %d", (blue_reading));


    //printing the judgement of values
    if (red_reading <= 255 && red_reading >= 200 && green_reading >= 80 && green_reading <= 190 && blue_reading >= 70 && blue_reading <= 170) {
      dbprintf("Red is detected");
    } else if (red_reading >= 130 && green_reading >= 150 && blue_reading >= 90 && blue_reading <= 200) {
      dbprintf("Green is detected");
    } else {
      dbprintf("its nothing! move on!");
    }

    alex_delay(500);
  }

}