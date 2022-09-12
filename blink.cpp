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
// #include <Wire.h>
#include <Servo.h>

// #include "autoswinglib.hpp"
#include "blob.hpp"



#if 1

position_t pos;
control_t control;

void setup() {
    Serial.begin(230400);

    pos.setup();
    control.setup();
}
void loop() {
    pos.tick();
    control.tick();
}

#endif



#if 0

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(3);  // attaches the servo on pin 9 to the servo object
  Serial.begin(230400);
}

void handleChar2() {
  // Serial.flush();
  char c = 'x';
  if(Serial.available()) {
    c = Serial.read();
    // return;
  }
  static char cp = ' ';
  
  // int pulse = 300;
  const int bumpers = 4;
  const int bias = 10;

  int dafter = 150;

  if(c == 'a') {
    // digitalWrite(3, HIGH);
    // delay(pulse);
    // digitalWrite(3, LOW);
    // ledon();
    myservo.write(0+bumpers);
    delay(dafter);
  } else if (c == 's') {
    // digitalWrite(5, HIGH);
    // delay(pulse);
    // digitalWrite(5, LOW);
    // ledoff();
    myservo.write(180-bumpers);
    delay(dafter);
  } else {
    myservo.write(90+bias);
  }

  // snprintf(buf, 64, "read '%c'\r\n", c);
  // Serial.print(buf);
}

void loop() {
  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
  // myservo.write(90);
    
  handleChar2();
  delay(150);
}

#endif


#if 0
void setup() {
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);
  Serial.begin(38400);

  // PWM
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
  OCR2A = 180;
  OCR2B = 50;
}

char buf[64];

int reads = 0;

void loop() {
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);

  snprintf(buf, 64, "Hello world %d\r\n", reads);
  Serial.print(buf);

  // analogWrite(3, (reads % 255) );


  reads++;
}

#endif

#if 0
void setup() {
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);
  Serial.begin(38400);

  // PWM
  pinMode(3, OUTPUT);
  // pinMode(11, OUTPUT);
  // TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
  // TCCR2B = _BV(CS22);
  // OCR2A = 180;
  // OCR2B = 50;
}

char buf[64];

int reads = 0;

void loop() {
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);

  snprintf(buf, 64, "Hello world %d\r\n", reads);
  Serial.print(buf);

  analogWrite(3, (reads % 255) );


  reads++;
}

#endif

#if 0

// The char version of DC motor swing
// A/S to control


void setup() {
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);
  Serial.begin(38400);

  digitalWrite(13, LOW);


  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
}

char buf[64];

int reads = 0;
int odelay = 100;
void ledon() {
  digitalWrite(13, HIGH);
  delay(odelay);
}

void ledoff() {
  digitalWrite(13, LOW);
  delay(odelay);
}

void handleChar() {
  // Serial.flush();
  if(!Serial.available()) {
    return;
  }
  const char c = Serial.read();
  
  int pulse = 300;

  if(c == 'a') {
    digitalWrite(3, HIGH);
    delay(pulse);
    digitalWrite(3, LOW);
    ledon();
  } else if (c == 's') {
    digitalWrite(5, HIGH);
    delay(pulse);
    digitalWrite(5, LOW);
    ledoff();
  }

  // snprintf(buf, 64, "read '%c'\r\n", c);
  // Serial.print(buf);
}

void loop() {

  // snprintf(buf, 64, "Hello world %d\r\n", reads);
  // Serial.print(buf);


  handleChar();


  // if(false) {
  //   if( (reads%2) == 0) {
  //     digitalWrite(3, HIGH);
  //     delay(pulse);
  //     digitalWrite(3, LOW);
  //     ledon();
  //   } else if ((reads%2) == 1) {
  //     digitalWrite(5, HIGH);
  //     delay(pulse);
  //     digitalWrite(5, LOW);
  //     ledoff();
  //   }
  // }

  reads++;

}

#endif

#if 0

char buf[64];



void setup() {
  // Serial.begin(250000);
  Serial.begin(230400);
  // Serial.begin(460800);
  
}

void loop() {
  // Serial.print("loop\r\n");

  printf("loop asdlkfjasdlfkjasdflkjasdflkjasdlkfjasdlfkjasdlfkjsdfasdf\r\n");


  // snprintf(buf, 64, "Hello world %d\r\n", reads);
  // Serial.print(buf);


  // delay(700);
}
#endif



#if 0

#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif


MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;



#define OUTPUT_READABLE_ACCELGYRO




#define LED_PIN 13
bool blinkState = false;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(230400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // use the code below to change accel/gyro offset values
    /*
    Serial.println("Updating internal sensor offsets...");
    // -76  -2359 1688  0 0 0
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    accelgyro.setXGyroOffset(220);
    accelgyro.setYGyroOffset(76);
    accelgyro.setZGyroOffset(-85);
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    */

    // configure Arduino LED pin for output
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    #ifdef OUTPUT_READABLE_ACCELGYRO
        // display tab-separated accel/gyro x/y/z values
        Serial.print("a/g:\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.println(gz);
    #endif

    #ifdef OUTPUT_BINARY_ACCELGYRO
        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
    #endif

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
}

#endif






#if 0

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

#endif