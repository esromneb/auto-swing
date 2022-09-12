#pragma once

#include <vector>
// #include <tuple>

#ifdef TEST_ENVIRONMENT
#include <stdio.h>
#endif

//------------------------------------------------------------------------------
// FSM with two enums
// pass functions or lambdas
// idle is allowed to be NULL, but process must be defined
// order of states passed can be sparse and un-ordered
//------------------------------------------------------------------------------

// forward declare
template <typename ST_T, typename EV_T, typename VP_T>
struct myfsm_t;

// type of function for entering a state
// we have to put the "using" keyword instead of "typedef"
// because it allows us to pass the template type through
template <typename ST_T, typename EV_T, typename VP_T>
using cb_enter_t = void (*)(myfsm_t<ST_T, EV_T, VP_T>& fsm);

// type of function for processing
template <typename ST_T, typename EV_T, typename VP_T>
using cb_process_t = void (*)(myfsm_t<ST_T, EV_T, VP_T>& fsm, const EV_T ev);

// A record, this is what is loaded into the init() function
// as well as used for local storage inside the class
template <typename ST_T, typename EV_T, typename VP_T>
struct state_record_t {
    ST_T state;
    cb_enter_t<ST_T, EV_T, VP_T> enter;
    cb_process_t<ST_T, EV_T, VP_T> process;
};

template <typename ST_T, typename EV_T, typename VP_T>
struct myfsm_t {
    std::vector<state_record_t<ST_T, EV_T, VP_T>> states;
    std::vector<EV_T> ev_q;
    ST_T state = (ST_T)0;
    bool changing = false;

    VP_T* vp;

    void init(std::vector<state_record_t<ST_T, EV_T, VP_T>> _states, VP_T* _vp = NULL);
    void post(const EV_T ev);
    void postTick(const EV_T ev);
    void go(const ST_T st);
    void tick(void);
};


// argument to this function does not care about the order of the states
// internally we will re-order these
template <typename ST_T, typename EV_T, typename VP_T>
void myfsm_t<ST_T,EV_T,VP_T>::init(std::vector<state_record_t<ST_T, EV_T, VP_T>> _states, VP_T* _vp) {
    vp = _vp;

    size_t max = 0;
    for(const auto s : _states) {
        const size_t sint = (size_t)s.state; // convert to size_t from enum
        if(sint > max) {
            max = sint;
        }
    }
    // printf("Found max %zu\n", max);

    states.resize(max+1); // +1 to account for max vs count

    for(const auto s : _states) {
        const size_t sint = (size_t)s.state; // convert to size_t from enum

        if(s.process == NULL) {
            printf("illegal NULL process for state %zu\r\n", sint);
        }
        states[sint] = {s.state, s.enter, s.process};
    }
}


template <typename ST_T, typename EV_T, typename VP_T>
void myfsm_t<ST_T,EV_T,VP_T>::post(const EV_T ev) {
    ev_q.push_back(ev);
}

template <typename ST_T, typename EV_T, typename VP_T>
void myfsm_t<ST_T,EV_T,VP_T>::postTick(const EV_T ev) {
    ev_q.push_back(ev);
    tick();
}

template <typename ST_T, typename EV_T, typename VP_T>
void myfsm_t<ST_T,EV_T,VP_T>::go(const ST_T st) {
    state = st;
    changing = true;
}

template <typename ST_T, typename EV_T, typename VP_T>
void myfsm_t<ST_T,EV_T,VP_T>::tick(void) {
    while(ev_q.size() != 0) {
        const size_t sint = (size_t)state; // convert to size_t from enum

        const state_record_t<ST_T, EV_T, VP_T> record = states[sint];

        EV_T ev = ev_q.front();
        // printf("Got event %zu\r\n", (size_t)ev);
        ev_q.erase(ev_q.begin());


        record.process(*this, ev);
        if(changing) {
            const size_t sint2 = (size_t)state; // convert to size_t from enum
            const state_record_t<ST_T, EV_T, VP_T> record2 = states[sint2];

            changing = false;

            record2.enter(*this);
        }
    }
}
