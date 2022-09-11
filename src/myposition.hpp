#pragma once


#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

struct position_t {
    MPU6050 accelgyro;
    int16_t ax = 0;
    int16_t ay = 0;
    int16_t az = 0;
    int16_t gx = 0;
    int16_t gy = 0;
    int16_t gz = 0;

    void setup(void);
    void tick(void);
};