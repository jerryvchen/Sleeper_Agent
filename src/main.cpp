#include <Arduino.h>

//  Variables
int PulseSensorPurplePin = 2;        // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
int LED13 = 13;   //  The on-board Arduion LED


int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 550;            // Determine which Signal to "count as a beat", and which to ingore. 




void setup() {
  // put your setup code here, to run once:
  pinMode(LED13, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  Signal = analogRead(PulseSensorPurplePin);
  Serial.println(Signal);

  delay(10);
}

