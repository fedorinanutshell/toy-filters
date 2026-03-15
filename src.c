#include <stdlib.h>
#include <stdint.h>
#include <math.h>

struct bilin {
  float a_0, a_1, b_0, b_1;
  float x_1, y_1;
};

struct bilin bilin_lp(float omega) {
  return (struct bilin) {
    sinf(omega) + 1.f + cosf(omega),
    sinf(omega) - 1.f - cosf(omega),
    sinf(omega),
    sinf(omega),
    .0f, .0f,
  };
}

struct bilin bilin_ls(float omega, float a) {
  return (struct bilin) {
    1.f / a * sinf(omega) + 1.f + cosf(omega),
    1.f / a * sinf(omega) - 1.f - cosf(omega),
    a * sinf(omega) + 1.f + cosf(omega),
    a * sinf(omega) - 1.f - cosf(omega),
    .0f, .0f,
  };
}

struct bilin bilin_hp(float omega) {
  return (struct bilin) {
    sinf(omega) + 1.f + cosf(omega),
    sinf(omega) - 1.f - cosf(omega),
    1.f + cosf(omega),
    -1.f - cosf(omega),
    .0f, .0f,
  };
}

struct bilin bilin_hs(float omega, float a) {
  return (struct bilin) {
    sinf(omega) + 1.f / a + 1.f / a * cosf(omega),
    sinf(omega) - 1.f / a - 1.f / a * cosf(omega),
    sinf(omega) + a + a * cosf(omega),
    sinf(omega) - a - a * cosf(omega),
    .0f, .0f,
  };
}

struct bilin bilin_ap(float omega) {
  return (struct bilin) {
    sinf(omega) + 1.f + cosf(omega),
    sinf(omega) - 1.f - cosf(omega),
    sinf(omega) - 1.f - cosf(omega),
    sinf(omega) + 1.f + cosf(omega),
    .0f, .0f,
  };
}

void bilin_proc(const float *x, float *y, size_t n, struct bilin *bilin) {
  for (uint64_t i = 0; i < n; ++i) {
    y[i] = (bilin->b_0 * x[i] + bilin->b_1 * bilin->x_1 - bilin->a_1 * bilin->y_1) / bilin->a_0;
    bilin->x_1 = x[i]; bilin->y_1 = y[i];
  }
}

struct biquad {
  float a_0, a_1, a_2, b_0, b_1, b_2;
  float x_1, x_2, y_1, y_2;
};

struct biquad biquad_peq(float omega, float q, float a) {
  float alpha = sinf(omega) / (2.f * q);
  return (struct biquad) {
    1.f + alpha / a,
    -2.f * cosf(omega),
    1.f - alpha / a,
    1.f + alpha * a,
    -2.f * cosf(omega),
    1.f - alpha * a,
    .0f, .0f, .0f, .0f,
  };
}

struct biquad biquad_lp(float omega, float q) {
  float alpha = sinf(omega) / (2.f * q);
  return (struct biquad) {
    1.f + alpha,
    -2.f * cosf(omega),
    1.f - alpha,
    (1.f - cosf(omega)) / 2.f,
    1.f - cosf(omega),
    (1.f - cosf(omega)) / 2.f,
    .0f, .0f, .0f, .0f,
  };
}

struct biquad biquad_ls(float omega, float q, float a) {
  float alpha = sinf(omega) / (2.f * q);
  return (struct biquad) {
    (a + 1.f) + (a - 1.f) * cosf(omega) + 2.f * sqrtf(a) * alpha,
    -2.f * ((a - 1.f) + (a + 1.f) * cosf(omega)),
    (a + 1.f) + (a - 1.f) * cosf(omega) - 2.f * sqrtf(a) * alpha,
    a * ((a + 1.f) - (a - 1.f) * cosf(omega) + 2.f * sqrtf(a) * alpha),
    2.f * a * ((a - 1.f) - (a + 1.f) * cosf(omega)),
    (a + 1.f) + (a - 1.f) * cosf(omega) - 2 * sqrtf(a) * alpha,
    .0f, .0f, .0f, .0f,
  };
}

struct biquad biquad_hp(float omega, float q) {
  float alpha = sinf(omega) / (2.f * q);
  return (struct biquad) {
    1.f + alpha,
    -2.f * cosf(omega),
    1.f - alpha,
    (1.f + cosf(omega)) / 2.f,
    -(1.f + cosf(omega)),
    (1.f + cosf(omega)) / 2.f,
    .0f, .0f, .0f, .0f,
  };
}

struct biquad biquad_hs(float omega, float q, float a) {
  float alpha = sinf(omega) / (2.f * q);
  return (struct biquad) {
    (a + 1.f) + (a - 1.f) * cosf(omega) + 2.f * sqrtf(a) * alpha,
    2.f * ((a - 1.f) + (a + 1.f) * cosf(omega)),
    (a + 1.f) + (a - 1.f) * cosf(omega) - 2.f * sqrtf(a) * alpha,
    a * ((a + 1.f) + (a - 1.f) * cosf(omega) + 2.f * sqrtf(a) * alpha),
    -2.f * a * ((a - 1.f) + (a + 1.f) * cosf(omega)),
    (a + 1.f) + (a + 1.f) * cosf(omega) - 2 * sqrtf(a) * alpha,
    .0f, .0f, .0f, .0f,
  };
}

struct biquad biquad_bp(float omega, float q) {
  float alpha = sinf(omega) / (2.f * q);
  return (struct biquad) {
    1.f + alpha,
    -2.f * cosf(omega),
    1.f - alpha,
    alpha,
    0,
    -alpha,
    .0f, .0f, .0f, .0f,
  };
}

struct biquad biquad_notch(float omega, float q) {
  float alpha = sinf(omega) / (2.f * q);
  return (struct biquad) {
    1.f + alpha,
    -2.f * cosf(omega),
    1.f - alpha,
    1.f,
    -2.f * cosf(omega),
    1.f,
    .0f, .0f, .0f, .0f,
  };
}

void biquad_proc(const float *x, float *y, size_t n, struct biquad *biquad) {
  for (uint64_t i = 0; i < n; ++i) {
    y[i] = (biquad->b_0 * x[i] + biquad->b_1 * biquad->x_1 + biquad->b_2 * biquad->x_2 - biquad->a_1 * biquad->y_1 - biquad->a_2 * biquad->y_2) / biquad->a_0;
    biquad->x_2 = biquad->x_1; biquad->x_1 = x[i]; biquad->y_2 = biquad->y_1; biquad->y_1 = y[i];
  }
}
