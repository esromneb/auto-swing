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


std::vector<int> list;

void setup() {
    // initialize the digital pin as an output.
    // Pin 13 has an LED connected on most Arduino boards:
    pinMode(13, OUTPUT);
    Serial.begin(19200);
    list.push_back(300);
    list.push_back(1000);
    list.push_back(300);
    list.push_back(1000);
    list.push_back(300);
    list.push_back(1000);
}

char buf[64];

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
    snprintf(buf, 64, "Hello world %d\r\n", 43);
    Serial.print(buf);
    // Serial.print("Hellxx world.\r\n");
    // Serial.print("Hello world %d\r\n", 43);
}
