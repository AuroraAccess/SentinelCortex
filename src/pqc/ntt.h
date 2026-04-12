/* NOTICE: This file is protected under RCF-PL v1.3
 * [RCF:PUBLIC]
 */
#ifndef NTT_H
#define NTT_H

#include <stdint.h>

#define Q 8380417
#define MONT 4194304 // 2^22
#define QINV 58728449 // -1/Q mod 2^32

void ntt(int32_t a[256]);
void invntt(int32_t a[256]);
void poly_mul_pointwise(int32_t c[256], const int32_t a[256], const int32_t b[256]);

#endif
