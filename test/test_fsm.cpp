#include "myfsm.hpp"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#include <rapidcheck.h>
#pragma GCC diagnostic pop

#include <vector>
#include <algorithm>
#include <iostream>
// #include <iomanip>

//------------------------------------------------------------------------------
// forward declare
//------------------------------------------------------------------------------
void test_control_fsm(void);

using namespace std;

enum test_state_t {
    STATE_IDLE,
    STATE_GOTA,
    STATE_GOTB,
    STATE_GOTC,
};

enum test_ev_t {
    EV_RESET,
    EV_GOTA,
    EV_GOTB,
    EV_GOTC,
};



static size_t count_a = 0;
static size_t count_b = 0;

static void test_fsm(void) {

    using fsm_t = myfsm_t<test_state_t, test_ev_t, int>;

    rc::check("fsm basics", [](void) {

        count_a = 0;
        count_b = 0;

        auto idle_enter = [](fsm_t& fsm) {
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
                case EV_GOTC:
                    fsm.go(STATE_GOTC);
                    break;
            }
        };

        auto a_enter = [](fsm_t& fsm) {
            cout << "a_enter\n";
            count_a++;
        };

        auto b_enter = [](fsm_t& fsm) {
            cout << "b_enter\n";
            count_b++;
        };


        // pass out of order on purpose
        fsm_t fsm;
        fsm.init({
            {STATE_IDLE, idle_enter, idle_process},
            {STATE_GOTB, b_enter, idle_process},
            {STATE_GOTA, a_enter, idle_process},
            {STATE_GOTC, NULL,    idle_process},
        });

        fsm.postTick(EV_GOTA);
        RC_ASSERT(fsm.state == STATE_GOTA);

        fsm.postTick(EV_GOTB);
        RC_ASSERT(fsm.state == STATE_GOTB);

        fsm.postTick(EV_RESET);
        RC_ASSERT(fsm.state == STATE_IDLE);

        RC_ASSERT(count_a == 1);
        RC_ASSERT(count_b == 1);


        // test that posting 2 events will run back to back
        // with 1 tick
        // I would like to test order here but it doesn't
        // I checked in the debugger however
        fsm.post(EV_GOTA);
        fsm.post(EV_GOTB);
        fsm.tick();

        RC_ASSERT(count_a == 2);
        RC_ASSERT(count_b == 2);

        fsm.postTick(EV_GOTC);
        RC_ASSERT(fsm.state == STATE_GOTC);
    });
}


int main(void) {

    test_fsm();
    test_control_fsm();
    // cout << "in test\n";

    // rc::check("rc is working?", [](void) {

    //     const uint32_t a_word = *rc::gen::inRange(0u, 0x3u);
    //     cout << "value: " << a_word << "\n";

    //     RC_ASSERT(
    //         1 == 1
    //         );
    // });
}