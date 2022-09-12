#pragma once

#include <Servo.h>

struct control_t {
    Servo myservo;
    long int last_char_time = 0;

    void setup(void);
    void handleChar2(void);
    void tick(void);
};
