#pragma once

#include <Servo.h>

enum control_state_t {
    STATE_IDLE,
    STATE_READ,
};

struct control_t {
    Servo myservo;
    long int last_char_time = 0;

    myfsm_t<control_state_t> fsm;

    void setup(void);
    void handleChar2(void);
    void tick(void);
};
