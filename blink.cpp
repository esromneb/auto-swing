/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  This example code is in the public domain.
 */

// #include "other.h"
#include <Arduino.h>
// #include <vector>
#include <ArduinoSTL.h>
// #include "Serial.h"
#include <vector>
// #include <cout>
#include <Wire.h>


std::vector<int> list;

void setup() {
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);
  Serial.begin(19200);


  Wire.begin();

  list.push_back(300);
  list.push_back(1000);
  list.push_back(300);
  list.push_back(1000);
  list.push_back(300);
  list.push_back(1000);
}

char buf[64];

int reads = 0;
static int addr = 0x68;

void loop() {
  digitalWrite(13, HIGH);
  // delay(getdelay());
  if( list.size() ) {
    int dd = list.back();
//     delay(dd);
    list.pop_back();
    snprintf(buf, 64, "list has %d\r\n", dd);
    Serial.print(buf);
  }
  // cout << "hi\r\n";
  delay(300);
  
  
  digitalWrite(13, LOW);
  delay(200);
  snprintf(buf, 64, "Hello world %d\r\n", reads);
  Serial.print(buf);

  reads++;


  Wire.requestFrom(addr, 6);    // request 6 bytes from peripheral device #8

  while (Wire.available()) { // peripheral may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }

    // Serial.print("Hellxx world.\r\n");
    // Serial.print("Hello world %d\r\n", 43);
}
