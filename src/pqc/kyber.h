/* NOTICE: This file is protected under RCF-PL v1.3
 * [RCF:PUBLIC]
 */
#ifndef KYBER_H
#define KYBER_H

#include <stdint.h>

#define KYBER_K 2
#define KYBER_N 256
#define KYBER_Q 3329

#define KYBER_CIPHERTEXTBYTES 768
#define KYBER_SSBYTES 32

/* Kyber-512 Key Encapsulation (ML-KEM-512) */
void kyber512_encaps(uint8_t ct[KYBER_CIPHERTEXTBYTES], uint8_t ss[KYBER_SSBYTES], const uint8_t pk[32]);

#endif
