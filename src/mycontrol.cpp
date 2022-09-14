#include "mycontrol.hpp"

#if 1


#define SHORT_MAGNET_ON_ARM


#ifdef NO_ATTACHMENTS
static int bumpers = 55;
static int dafter = 350;
#endif

#ifdef SHORT_MAGNET_ON_ARM
static int bumpers = 55;
static int dafter = 480;
#endif



static int enable = 0;

// returns true if ate a char
static bool update_keyboard_in(void) {
    if(!Serial.available()) {
        return false;
    }
    const char c = Serial.read();

    switch(c) {
        case 'q':
            bumpers += 5;
            printf("bumpers = %ld\r\n", (long)bumpers);
            break;
        case 'a':
            bumpers -= 5;
            printf("bumpers = %ld\r\n", (long)bumpers);
            break;
        case 'w':
            dafter += 10;
            printf("dafter = %ld\r\n", (long)dafter);
            break;
        case 's':
            dafter -= 10;
            printf("dafter = %ld\r\n", (long)dafter);
            break;
        case 'p':
            enable ^= 0x1;
            break;
    }
    return true;
}

static void update_keyboard(void) {
    bool ran = true;
    while(ran) {
        ran = update_keyboard_in();
    }
}

static void idle_enter(fsm_t& fsm) {


}

static void idle_process(fsm_t& fsm, const control_ev_t ev) {
    control_t* const control = fsm.vp;

    if(ev != EV_RUN) {
        return;
    }

    update_keyboard();

    if(!enable) {
        return;
    }

    // const int bumpers = 65; //4;
    const int bias = 10;
        // control->myservo.write(0+bumpers); // right
        // control->myservo.write(180-bumpers); // left
        // control->myservo.write(90+bias); // center

    static bool swingleft = true;

    if(swingleft) {
        control->myservo.write(180-bumpers+bias); // left
    } else {
        control->myservo.write(0+bumpers+bias); // right
    }

    swingleft ^= 0x1;

    fsm.go(STATE_DELAY_AFTER_CHAR);
}

static void char_enter(fsm_t& fsm) {

}

static void char_process(fsm_t& fsm, const control_ev_t ev) {
    control_t* const control = fsm.vp;
}


static void delay_enter(fsm_t& fsm) {
    control_t* const control = fsm.vp;
    control->delay_time = millis();
    // printf("delay_enter\r\n");
}

static void delay_process(fsm_t& fsm, const control_ev_t ev) {
    control_t* const control = fsm.vp;

    if(ev != EV_RUN) {
        return;
    }



    long int now = millis();

    if( (now-control->delay_time) >= dafter ) {
        fsm.go(STATE_IDLE);
        // printf("delay_exit\r\n");
    }
}
#endif

#if 0
static void idle_enter(fsm_t& fsm) {


}

static void idle_process(fsm_t& fsm, const control_ev_t ev) {
    control_t* const control = fsm.vp;

    if(ev == EV_RUN) {
        long int now = millis();

        long int dly = 150;

        if( (now-control->last_char_time) >= dly ) {
            fsm.go(STATE_CHAR);
            // handleChar2();
            control->last_char_time = now;
        }
    }
}

static void char_enter(fsm_t& fsm) {
    control_t* const control = fsm.vp;

    // printf("char_enter\r\n");


    char c = 'x';
    if(Serial.available()) {
        c = Serial.read();
        // return;
    }
    static char cp = 'x';

    // int pulse = 300;
    const int bumpers = 4;
    const int bias = 10;

    // int dafter = 150;

    // right
    if(c == 'a') {
        printf("0,0,0,0,0,0,0,2\r\n");
        control->myservo.write(0+bumpers);
        // delay(dafter);
        fsm.go(STATE_DELAY_AFTER_CHAR);
    } else if (c == 's') {
        printf("0,0,0,0,0,0,0,3\r\n");
        control->myservo.write(180-bumpers);
        fsm.go(STATE_DELAY_AFTER_CHAR);
        // delay(dafter);
    } else {
        if( cp != 'x' ) {
            printf("0,0,0,0,0,0,0,1\r\n");
        }
        control->myservo.write(90+bias);
        fsm.go(STATE_IDLE);
    }

    cp = c;
}

static void char_process(fsm_t& fsm, const control_ev_t ev) {
    control_t* const control = fsm.vp;
}


static void delay_enter(fsm_t& fsm) {
    control_t* const control = fsm.vp;
    control->delay_time = millis();
    // printf("delay_enter\r\n");
}

static void delay_process(fsm_t& fsm, const control_ev_t ev) {
    control_t* const control = fsm.vp;

    if(ev != EV_RUN) {
        return;
    }

    int dafter = 150;

    long int now = millis();

    if( (now-control->delay_time) >= dafter ) {
        fsm.go(STATE_IDLE);
        // printf("delay_exit\r\n");
    }
}
#endif



void control_t::setup(void) {
    myservo.attach(3);  // attaches the servo on pin 9 to the servo object

    fsm.init({
        {STATE_IDLE,             idle_enter,    idle_process},
        {STATE_CHAR,             char_enter,    char_process},
        {STATE_DELAY_AFTER_CHAR, delay_enter,   delay_process},
        }, this);

}

void control_t::handleChar2() {
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

void control_t::tick(void) {
    fsm.postTick(EV_RUN);
    // long int now = millis();

    // long int dly = 150;

    // if( (now-last_char_time) >= dly ) {
    //     handleChar2();
    //     last_char_time = now;
    // }
}
