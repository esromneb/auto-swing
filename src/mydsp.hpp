#pragma once

struct position_t;

struct dsp_t {
    position_t* pos = NULL;

    void setup(position_t* _pos);
    void tick(void);
};
