#include "myfsm.hpp"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#include <rapidcheck.h>
#pragma GCC diagnostic pop

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;


enum control_state_t {
    STATE_IDLE,              // 0
    STATE_CHAR,              // 1
    STATE_DELAY_AFTER_CHAR,  // 2
};

enum control_ev_t {
    EV_RESET,
    EV_RUN,
};

using fsm_t = myfsm_t<control_state_t, control_ev_t, int>;

static long int millis_val = 0;
long int millis(void) {
    return millis_val;
}

void set_millis(const long int m) {
    millis_val = m;
}


std::vector<std::string> fns;

static void idle_enter(fsm_t& fsm) {
    fns.push_back("idle_enter");

}

static long int last_char_time = 0;
static void idle_process(fsm_t& fsm, const control_ev_t ev) {
    fns.push_back("idle_process");

    if(ev == EV_RUN) {
        long int now = millis();

        long int dly = 150;

        if( (now-last_char_time) >= dly ) {
            fsm.go(STATE_CHAR);
            // handleChar2();
            last_char_time = now;
        }
    }
}

static void char_enter(fsm_t& fsm) {
    fns.push_back("char_enter");
    printf("char_enter\r\n");

    fsm.go(STATE_DELAY_AFTER_CHAR);
}

static void char_process(fsm_t& fsm, const control_ev_t ev) {
    fns.push_back("char_process");
    printf("char_process\r\n");
}

static size_t count_delay_enter = 0;

static void delay_enter(fsm_t& fsm) {
    fns.push_back("delay_enter");
    // control->delay_time = millis();
    printf("delay_enter\r\n");
    count_delay_enter++;
}

static void delay_process(fsm_t& fsm, const control_ev_t ev) {
    fns.push_back("delay_process");
    printf("delay_process\r\n");

    // Including or excluding this breaks the tests
    // fsm.go(STATE_IDLE);
}

auto is_process = [](std::string s){ return s == "delay_process"; };
auto is_enter   = [](std::string s){ return s == "delay_enter"; };


static void single_test(void) {
    count_delay_enter = 0;
    last_char_time = 0;
    fns.resize(0);
    cout << "single_test()--------------------------------------------\n";

    fsm_t fsm;

    fsm.init({
        {STATE_IDLE,             idle_enter,    idle_process},
        {STATE_CHAR,             char_enter,    char_process},
        {STATE_DELAY_AFTER_CHAR, delay_enter,   delay_process},
        }, NULL);

    set_millis(0);
    fsm.postTick(EV_RUN);
    set_millis(200);
    fsm.postTick(EV_RUN);
    fsm.postTick(EV_RUN);
    // fsm.postTick(EV_RUN);

    cout << "\n";

    for(const auto &f : fns) {
        cout << f << "\n";
    }

    cout << "\n";

    auto first_enter = (std::find(begin(fns), end(fns), "delay_enter")-fns.begin());
    auto first_process =  (std::find(begin(fns), end(fns), "delay_process")-fns.begin());

    cout << "First enter " << first_enter << "\n";
    cout << "First process " <<first_process << "\n";

    // test that we called enter
    RC_ASSERT(count_delay_enter == 1);

    // test that enter is called BEFORE process. don't use <= because if they both fail
    // they are both equal to 1+ the size of the vector
    RC_ASSERT(first_enter < first_process);

}


void test_control_fsm(void);
void test_control_fsm(void) {
    rc::check("char fsm", single_test);
}
