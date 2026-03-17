#include <stdlib.h>
#include <stdint.h>
#include <math.h>

struct bilin {
  float a_0, a_1, b_0, b_1;
  float x_1, y_1;
};

struct bilin bilin(float a_0, float a_1,
                   float b_0, float b_1) {
  return (struct bilin) {
    a_0, a_1,
    b_0, b_1,
    .0f, .0f,
  };
}

struct bilin bilin_lowpass(float o) {
  const float s = sinf(o), c = cosf(o);
  return bilin(
    s + 1.f + c,
    s - 1.f - c,
    s,
    s
  );
}

struct bilin bilin_lowshelf(float o, float g) {
  const float s = sinf(o), c = cosf(o);
  return bilin(
    1.f / g * s + 1.f + c,
    1.f / g * s - 1.f - c,
    g * s + 1.f + c,
    g * s - 1.f - c
  );
}

struct bilin bilin_highpass(float o) {
  const float s = sinf(o), c = cos(o);
  return bilin(
    s + 1.f + c,
    s - 1.f - c,
    1.f + c,
    -1.f - c
  );
}

struct bilin bilin_highshelf(float o, float g) {
  const float s = sinf(o), c = cosf(o);
  return bilin(
    s + 1.f / g + 1.f / g * c,
    s - 1.f / g - 1.f / g * c,
    s + g + g * c,
    s - g - g * c
  );
}

struct bilin bilin_allpass(float o) {
  const float s = sinf(o), c = cosf(o);
  return bilin(
    s + 1.f + c,
    s - 1.f - c,
    s - 1.f - c,
    s + 1.f + c
  );
}

void bilin_process(const float *x, float *y, size_t n, struct bilin *bilin) {
  for (uint64_t i = 0; i < n; ++i) {
    y[i] = (bilin->b_0 * x[i] + bilin->b_1 * bilin->x_1 - bilin->a_1 * bilin->y_1) / bilin->a_0;
    bilin->x_1 = x[i]; bilin->y_1 = y[i];
  }
}

struct biquad {
  float a_0, a_1, a_2, b_0, b_1, b_2;
  float x_1, x_2, y_1, y_2;
};

struct biquad biquad(float a_0, float a_1, float a_2,
                     float b_0, float b_1, float b_2) {
  return (struct biquad) {
    a_0, a_1, a_2,
    b_0, b_1, b_2,
    .0f, .0f, .0f, .0f,
  };
}

struct biquad biquad_peakeq(float o, float q, float g) {
  const float s = sinf(o), c = cosf(o), a = s / (2.f * q);
  return biquad(
    1.f + a / g,
    -2.f * c,
    1.f - a / g,
    1.f + a * g,
    -2.f * c,
    1.f - a * g
  );
}

struct biquad biquad_lowpass(float o, float q) {
  const float s = sinf(o), c = cosf(o), a = s / (2.f * q);
  return biquad(
    1.f + a,
    -2.f * c,
    1.f - a,
    (1.f - c) / 2.f,
    1.f - c,
    (1.f - c) / 2.f
  );
}

struct biquad biquad_lowshelf(float o, float q, float g) {
  const float s = sinf(o), c = cosf(o), a = s / (2.f * q), r = sqrtf(g);
  return biquad(
    (g + 1.f) + (g - 1.f) * c + 2.f * r * a,
    -2.f * ((g - 1.f) + (g + 1.f) * c),
    (g + 1.f) + (g - 1.f) * c - 2.f * r * a,
    g * ((g + 1.f) - (g - 1.f) * c + 2.f * r * a),
    2.f * g * ((g - 1.f) - (g + 1.f) * c),
    (g + 1.f) + (g - 1.f) * c - 2 * r * a
  );
}

struct biquad biquad_highpass(float o, float q) {
  const float s = sinf(o), c = cosf(o), a = s / (2.f * q);
  return biquad(
    1.f + a,
    -2.f * c,
    1.f - a,
    (1.f + c) / 2.f,
    -(1.f + c),
    (1.f + c) / 2.f
  );
}

struct biquad biquad_highshelf(float o, float q, float g) {
  const float s = sinf(o), c = cosf(o), a = s / (2.f * q), r = sqrtf(g);
  return biquad(
    (g + 1.f) + (g - 1.f) * c + 2.f * r * a,
    2.f * ((g - 1.f) + (g + 1.f) * c),
    (g + 1.f) + (g - 1.f) * c - 2.f * r * a,
    g * ((g + 1.f) + (g - 1.f) * c + 2.f * r * a),
    -2.f * g * ((g - 1.f) + (g + 1.f) * c),
    (g + 1.f) + (g + 1.f) * c - 2 * r * a
  );
}

struct biquad biquad_bandpass(float o, float q) {
  const float s = sinf(o), c = cosf(o), a = s / (2.f * q);
  return biquad(
    1.f + a,
    -2.f * c,
    1.f - a,
    a,
    0,
    -a
  );
}

struct biquad biquad_notch(float o, float q) {
  const float s = sinf(o), c = cosf(o), a = s / (2.f * q);
  return biquad(
    1.f + a,
    -2.f * c,
    1.f - a,
    1.f,
    -2.f * c,
    1.f
  );
}

struct biquad biquad_allpass(float o, float q) {
  const float s = sinf(o), c = cosf(o), a = s / (2.f * q);
  return biquad(
    1.f + a,
    -2.f * c,
    1.f - a,
    1.f - a,
    -2.f * c,
    1.f + a
  );
}

void biquad_process(const float *x, float *y, size_t n, struct biquad *biquad) {
  for (uint64_t i = 0; i < n; ++i) {
    y[i] = (biquad->b_0 * x[i] + biquad->b_1 * biquad->x_1 + biquad->b_2 * biquad->x_2 - biquad->a_1 * biquad->y_1 - biquad->a_2 * biquad->y_2) / biquad->a_0;
    biquad->x_2 = biquad->x_1; biquad->x_1 = x[i]; biquad->y_2 = biquad->y_1; biquad->y_1 = y[i];
  }
}
