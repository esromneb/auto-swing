#include "mydsp.hpp"

#include <math.h>


void dsp_t::setup(position_t* _pos) {
    pos = _pos;
    printf("DSP setup\r\n");

}


static void run(const int angle) {
    static int ap = 0;
    static int app = 0;
    static int appp = 0;

    static int calls = 0;

    if(ap <= app) {
        if(app <= appp) {
            printf("%04dleft\r\n", calls);
            calls ++;
        }
    }

    // printf("a: %d\r\n", angle);
    appp = app;
    app = ap;
    ap = angle;
}

void dsp_t::tick(void) {

    const int16_t ax = pos->ax;
    // int16_t ay = 0;
    const int16_t az = pos->az;
    // int16_t gx = 0;
    // int16_t gy = 0;
    // int16_t gz = 0;

    int angle = (10000) * atan2(ax,az);

    run(angle);

    // printf("x: %d\r\n", ax);
}
