#pragma once

#include <vector>

typedef void (*cb_t)(int prev);

template <typename T> struct myfsm_t {

    T* ptr;
    int size;

    std::vector<int> list;

    // Array(T arr[], int s);
    // void print();
    void init(int f);
};



template <typename T> void myfsm_t<T>::init(int f) {
    printf("in init\r\n");
    list.push_back(f);

    for( const auto w : list) {
        printf("%d\n", w);
    }
}
