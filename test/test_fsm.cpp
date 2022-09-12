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




static void test_fsm(void) {

    using fsm_t = myfsm_t<test_state_t, test_ev_t>;

    rc::check("fsm basics", [](void) {

        auto idle_enter = [](void) {
            cout << "idle_enter\n";
        };

        auto idle_process = [](fsm_t& fsm, const test_ev_t ev) {
            switch(ev) {
                case EV_RESET:
                    fsm.go(STATE_IDLE);
                    break;
                case EV_GOTA:
                    fsm.go(STATE_GOTA);
                    break;
                case EV_GOTB:
                    fsm.go(STATE_GOTB);
                    break;
            }
        };

        auto a_enter = [](void) {
            cout << "a_enter\n";
        };

        auto b_enter = [](void) {
            cout << "b_enter\n";
        };



        fsm_t fsm;
        fsm.init({
            {STATE_IDLE, idle_enter, idle_process},
            {STATE_GOTA, a_enter, idle_process},
            {STATE_GOTB, b_enter, idle_process},
        });

    });
}


int main(void) {

    test_fsm();
    // cout << "in test\n";

    // rc::check("rc is working?", [](void) {

    //     const uint32_t a_word = *rc::gen::inRange(0u, 0x3u);
    //     cout << "value: " << a_word << "\n";

    //     RC_ASSERT(
    //         1 == 1
    //         );
    // });
}