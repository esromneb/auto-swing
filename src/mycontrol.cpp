#include "mycontrol.hpp"


static void idle_enter(fsm_t& fsm) {
    control_t* const control = fsm.vp;
}

static void idle_process(fsm_t& fsm, const control_ev_t ev) {
    control_t* const control = fsm.vp;
    
}

static void char_enter(fsm_t& fsm) {
    control_t* const control = fsm.vp;

}

static void char_process(fsm_t& fsm, const control_ev_t ev) {
    control_t* const control = fsm.vp;
    
}



void control_t::setup(void) {
    myservo.attach(3);  // attaches the servo on pin 9 to the servo object

    // fsm.init(44);
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
    long int now = millis();

    long int dly = 150;

    if( (now-last_char_time) >= dly ) {
        handleChar2();
        last_char_time = now;
    }
}
