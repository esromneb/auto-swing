#pragma once

#include <vector>
// #include <tuple>

#ifdef TEST_ENVIRONMENT
#include <stdio.h>
#endif

// type of function for entering a state
typedef void (*cb_enter_t)(void);

// type of function for processing
// we have to put the "using" keyword instead of "typedef"
// because it allows us to pass the template type through
template <typename EV_T>
using cb_process_t = void (*)(const EV_T ev);

// template <typename EV_T>
// typedef void (*cb_process_t)(const EV_T ev);

template <typename ST_T, typename EV_T>
struct state_record_t {
    ST_T state;
    cb_enter_t enter;
    cb_process_t<EV_T> process;
};

template <typename ST_T, typename EV_T>
struct myfsm_t {

    // T* ptr;
    int size;

    std::vector<state_record_t<ST_T, EV_T>> states;
    std::vector<EV_T> ev_q;

    ST_T state = 0;

    // Array(T arr[], int s);
    // void print();
    void init(std::vector<state_record_t<ST_T, EV_T>> states);
    void post(const EV_T ev);
    void go(const ST_T st);
    void tick(void);
};



// argument to this function does not care about the order of the states
// internally we will re-order these
template <typename ST_T, typename EV_T>
void myfsm_t<ST_T,EV_T>::init(std::vector<state_record_t<ST_T, EV_T>> states) {

    size_t max = 0;
    for(const auto s : states) {
        const size_t sint = (size_t)s.state; // convert to enum
        if(sint > max) {
            max = sint;
        }
    }

    printf("Found max %zu\n", max);
    // printf("in init\r\n");
    // list.push_back(f);

    // for( const auto w : list) {
    //     printf("%d\n", w);
    // }

    // std::tuple<int, int> x;
    // x = {1,3};
    // std::function a;

    // void (*mfn)(void) = [](void) {
    //     printf("in my fn()\r\n");
    // };

    // mfn();
}


template <typename ST_T, typename EV_T>
void myfsm_t<ST_T,EV_T>::post(const EV_T ev) {
    ev_q.push_back(ev);
}

template <typename ST_T, typename EV_T>
void myfsm_t<ST_T,EV_T>::go(const ST_T st) {
    state = st;
}
