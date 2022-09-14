#include "mydsp.hpp"


void dsp_t::setup(position_t* _pos) {
    pos = _pos;
    printf("DSP setup\r\n");

}
void dsp_t::tick(void) {

    const int16_t ax = pos->ax;
    // int16_t ay = 0;
    // int16_t az = 0;
    // int16_t gx = 0;
    // int16_t gy = 0;
    // int16_t gz = 0;

    printf("x: %d\r\n", ax);
}
