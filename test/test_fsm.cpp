#include "myfsm.hpp"

#include <rapidcheck.h>

#include <vector>
#include <algorithm>
#include <iostream>
// #include <iomanip>

using namespace std;

enum test_state_t {
    STATE_IDLE,
    STATE_GOTA,
    STATE_GOTB,
};

enum test_ev_t {
    EV_RESET,
    EV_GOTA,
    EV_GOTB,
};




void test_fsm(void) {
    rc::check("fsm basics", [](void) {

        auto idle_enter = [](void) {
            cout << "idle_enter\n";
        };

        auto idle_process = [](const test_ev_t ev) {
            switch(ev) {
                case EV_RESET:
                    go(STATE_IDLE);
                    break;
                case EV_GOTA:
                    go(STATE_GOTA);
                    break;
                case EV_GOTB:
                    go(STATE_GOTB);
                    break;
            }
        };

        auto a_enter = [](void) {
            cout << "a_enter\n";
        };

        auto b_enter = [](void) {
            cout << "b_enter\n";
        };



        myfsm_t<control_state_t, control_ev_t> fsm;
        fsm.init({
            {STATE_IDLE, idle_enter, idle_process},
            {STATE_GOTA, a_enter, idle_process},
            {STATE_GOTB, b_enter, idle_process},
        });

    });
}


int main(void) {
    cout << "in test\n";

    rc::check("rc is working?", [](void) {

        const uint32_t a_word = *rc::gen::inRange(0u, 0x3u);
        cout << "value: " << a_word << "\n";

        RC_ASSERT(
            1 == 1
            );

    });
}