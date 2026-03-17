#pragma once

#include <stdlib.h>
#include <stdint.h>

struct bilin {
  float a_0, a_1, b_0, b_1;
  float x_1, y_1;
};

void bilin_process(const float *x, float *y, size_t n, struct bilin *bilin);

struct bilin bilin_lowpass(float o);
struct bilin bilin_lowshelf(float o, float g);
struct bilin bilin_highpass(float o);
struct bilin bilin_highshelf(float o, float g);
struct bilin bilin_allpass(float o);

struct biquad {
  float a_0, a_1, a_2, b_0, b_1, b_2;
  float x_1, x_2, y_1, y_2;
};

void biquad_process(const float *x, float *y, size_t n, struct biquad *biquad);

struct biquad biquad_lowpass(float o, float q);
struct biquad biquad_lowshelf(float o, float q, float g);
struct biquad biquad_highpass(float o, float q);
struct biquad biquad_highshelf(float o, float q, float g);
struct biquad biquad_bandpass(float o, float q);
struct biquad biquad_notch(float o, float q);
struct biquad biquad_peakeq(float o, float q, float g);
struct biquad biquad_allpass(float o, float q);
