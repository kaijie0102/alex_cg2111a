#include "Arduino.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//ultrasonic = pin12

/*
  #define ECHOPIN 13

  #define TRIGPIN 12
  #define LEDPIN 8
  #define S3PIN 7
  #define S2PIN 4
  #define S1PIN 1
  #define S0PIN 0

  float RED, BLUE, GREEN;
  const float SPEED_OF_SOUND = 0.034;
  int duration;
*/



void setupExt() {
  pinMode(ECHOPIN, INPUT);
  pinMode(LEDPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
  pinMode(S3PIN, OUTPUT);
  pinMode(S2PIN, OUTPUT);
  pinMode(S1PIN, OUTPUT);
  pinMode(S0PIN, OUTPUT);

  digitalWrite(S0PIN, HIGH);
  digitalWrite(S1PIN, LOW); //set to LOW for 20% scaling
  Serial.begin(9600);
}

/*
  int getDistance() {
    digitalWrite(TRIGPIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPIN, LOW);
    duration = pulseIn(ECHOPIN, HIGH);
    Serial.println(duration * SPEED_OF_SOUND / 2);
  }
*/

void calibrate() {
  unsigned long red_reading = 0;
  unsigned long blue_reading = 0;
  unsigned long green_reading = 0;
  int SAMPLES = 50;

  unsigned long redval, blueval, greenval;
  dbprintf("Calibrating...");
  for (int i = 0; i < SAMPLES; i++) {
    digitalWrite(S2PIN, LOW);
    digitalWrite(S3PIN, LOW);
    redval = pulseIn(LEDPIN, LOW);
    delay(100);

    digitalWrite(S2PIN, LOW);
    digitalWrite(S3PIN, HIGH);
    blueval = pulseIn(LEDPIN, LOW);
    delay(100);

    digitalWrite(S2PIN, HIGH);
    digitalWrite(S3PIN, HIGH);
    greenval = pulseIn(LEDPIN, LOW);
    delay(100);

    dbprintf("Rv: %d ", (redval));
    dbprintf("Bv: %d ", (blueval));
    dbprintf("Gv: %d", (greenval));

    red_reading += redval;
    blue_reading += blueval;
    green_reading += greenval;
  }


  red_reading = red_reading / SAMPLES;
  blue_reading = blue_reading / SAMPLES;
  green_reading = green_reading / SAMPLES;

  //    dbprintf("RED: %f",RED);
  dbprintf("Red: %d", (red_reading));
  dbprintf("Green: %d", (green_reading));
  dbprintf("Blue: %d", (blue_reading));


  delay(500);
}
void getColor() {
  int redMin = 80; // Red minimum value
  int redMax = 600; // Red maximum value
  int greenMin = 60; // Green minimum value
  int greenMax = 580; // Green maximum value
  int blueMin = 40; // Blue minimum value
  int blueMax = 400; // Blue maximum value
  //low, low = red || high,high = green || low(S2), high(S3) = blue
  int SAMPLES = 20;

  unsigned long red_reading = 0;
  unsigned long blue_reading = 0;
  unsigned long green_reading = 0;
  unsigned long redval, blueval, greenval;

  for (int i = 1; i <= 2; i++) {
    dbprintf("Detecting colour: Round %d",i);
    for (int i = 0; i < SAMPLES; i++) {
      digitalWrite(S2PIN, LOW);
      digitalWrite(S3PIN, LOW);
      redval = pulseIn(LEDPIN, LOW);
      delay(100);

      digitalWrite(S2PIN, LOW);
      digitalWrite(S3PIN, HIGH);
      blueval = pulseIn(LEDPIN, LOW);
      delay(100);

      digitalWrite(S2PIN, HIGH);
      digitalWrite(S3PIN, HIGH);
      greenval = pulseIn(LEDPIN, LOW);
      delay(100);

      //        dbprintf("Redval: %d Blueval: %d Greenval: %d",(redval, blueval,greenval));

      red_reading += redval;
      blue_reading += blueval;
      green_reading += greenval;
    }
    //map(bluePW, blueMin,blueMax,255,0);
    red_reading = red_reading / SAMPLES;
    blue_reading = blue_reading / SAMPLES;
    green_reading = green_reading / SAMPLES;
    //  dbprintf("pre mapping");
    //  dbprintf("Red: %d", (red_reading));
    //  dbprintf("Blue: %d", (blue_reading));
    //  dbprintf("Green: %d", (green_reading));

    red_reading = map( red_reading, redMin, redMax, 255, 0);
    green_reading = map( green_reading, greenMin, greenMax, 255, 0);
    blue_reading = map( blue_reading, blueMin, blueMax, 255, 0);

    if (red_reading > 255) red_reading = 255;
    if (red_reading < 0) red_reading = 0;
    if (red_reading > 255) green_reading = 255;
    if (red_reading < 0) green_reading = 0;
    if (red_reading > 255) blue_reading = 255;
    if (red_reading < 0) blue_reading = 0;


    //  dbprintf("post mapping");
    dbprintf("Red: %d", (red_reading));
    dbprintf("Green: %d", (green_reading));
    dbprintf("Blue: %d", (blue_reading));


    //    dbprintf("RED: %f",RED);
    //  dbprintf("Red: %d Blue: %d Green: %d", (red_reading, blue_reading, green_reading));

    if (red_reading <= 255 && red_reading >= 200 && green_reading >= 80 && green_reading <= 190 && blue_reading >= 70 && blue_reading <= 170) {
      dbprintf("Red is detected");
    } else if (red_reading >= 130 && green_reading >= 150 && blue_reading >= 90 && blue_reading <= 200) {
      dbprintf("Green is detected");
    } else {
      dbprintf("its nothing! move on!");
    }

    delay(500);
  }

}
