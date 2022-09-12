#pragma once

#include <vector>

typedef void (*cb_t)(int prev);

template <typename ST_T, typename EV_T>
struct myfsm_t {

    // T* ptr;
    int size;

    std::vector<int> list;
    std::vector<EV_T> ev_q;

    // Array(T arr[], int s);
    // void print();
    void init(int f);
    void post(const EV_T ev);
    void tick(void);
};



template <typename ST_T, typename EV_T>
void myfsm_t<ST_T,EV_T>::init(int f) {
    printf("in init\r\n");
    list.push_back(f);

    for( const auto w : list) {
        printf("%d\n", w);
    }
}


template <typename ST_T, typename EV_T>
void myfsm_t<ST_T,EV_T>::post(const EV_T ev) {
    ev_q.push_back(ev);
}
