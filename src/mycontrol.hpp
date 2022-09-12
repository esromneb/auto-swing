#pragma once

#include <Servo.h>

enum control_state_t {
    STATE_IDLE,
    STATE_CHAR,
};

enum control_ev_t {
    EV_RESET,
    EV_RUN,
};

// forward declare
struct control_t;

using fsm_t = myfsm_t<control_state_t, control_ev_t, control_t>;

struct control_t {
    Servo myservo;
    long int last_char_time = 0;

    fsm_t fsm;

    void setup(void);
    void handleChar2(void);
    void tick(void);
};
