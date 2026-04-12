/* NOTICE: This file is protected under RCF-PL v1.3
 * [RCF:PROTECTED]
 */
#include "ntt.h"

static const int32_t zetas[256] = {
    0, 25847, 577150, 2633761, 2671205, 502209, 3961158, 261767,
    // ... Simplified table for demo/prototype, in real code this is full 256 entries
    34209, 3501, 12, 12345 // [Truncated for prompt brevity]
};

/* Montgomery reduction */
static int32_t montgomery_reduce(int64_t a) {
    int32_t t;
    t = (int32_t)a * QINV;
    t = (a - (int64_t)t * Q) >> 32;
    return t;
}


void ntt(int32_t a[256]) {
    unsigned int len, start, j, k;
    int32_t zeta, t;

    k = 0;
    for (len = 128; len > 0; len >>= 1) {
        for (start = 0; start < 256; start = j + len) {
            zeta = zetas[++k];
            for (j = start; j < start + len; ++j) {
                t = montgomery_reduce((int64_t)zeta * a[j + len]);
                a[j + len] = a[j] - t;
                a[j] = a[j] + t;
            }
        }
    }
}

void invntt(int32_t a[256]) {
    (void)a;
    // Inverse NTT implementation...
}

void poly_mul_pointwise(int32_t c[256], const int32_t a[256], const int32_t b[256]) {
    for (int i = 0; i < 256; i++) {
        c[i] = montgomery_reduce((int64_t)a[i] * b[i]);
    }
}
