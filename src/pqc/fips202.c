#include "fips202.h"
#include <string.h>

#define NROUNDS 24
#define ROL(a, offset) ((a << offset) ^ (a >> (64 - offset)))

static const uint64_t KeccakF_RoundConstants[NROUNDS] = {
    0x0000000000000001ULL, 0x0000000000008082ULL, 0x000000008000808aULL,
    0x0000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
    0x0000000080008081ULL, 0x0000000000008009ULL, 0x000000000000008aULL,
    0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
    0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
    0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
};


/* Compact version for A-VM */
static void keccak_permute(uint64_t state[25]) {
    for (int r = 0; r < 24; r++) {
        uint64_t C[5], D[5], T[25];
        for (int i = 0; i < 5; i++) C[i] = state[i] ^ state[i+5] ^ state[i+10] ^ state[i+15] ^ state[i+20];
        for (int i = 0; i < 5; i++) D[i] = C[(i+4)%5] ^ ROL(C[(i+1)%5], 1);
        for (int i = 0; i < 25; i++) state[i] ^= D[i%5];
        
        static const int rho[24] = {1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 2, 14, 27, 41, 56, 8, 25, 43, 62, 18, 39, 61, 20, 44};
        static const int pi[24] = {10, 7, 11, 17, 18, 3, 5, 16, 8, 21, 24, 4, 15, 23, 19, 13, 12, 2, 20, 14, 22, 9, 6, 1};
        uint64_t temp = state[1];
        for (int i = 0; i < 24; i++) {
            int j = pi[i];
            T[j] = ROL(temp, rho[i]);
            temp = state[j];
        }
        T[0] = state[0];
        
        for (int j = 0; j < 5; j++) {
            for (int i = 0; i < 5; i++)
                state[i + 5*j] = T[i + 5*j] ^ ((~T[(i+1)%5 + 5*j]) & T[(i+2)%5 + 5*j]);
        }
        state[0] ^= KeccakF_RoundConstants[r];
    }
}

static void shake(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen, uint8_t rate) {
    uint64_t state[25] = {0};
    uint8_t *tstate = (uint8_t*)state;
    size_t i;

    while (inlen >= rate) {
        for (i = 0; i < rate; i++) tstate[i] ^= in[i];
        keccak_permute(state);
        in += rate; inlen -= rate;
    }
    for (i = 0; i < inlen; i++) tstate[i] ^= in[i];
    tstate[inlen] ^= 0x1f; // SHAKE padding
    tstate[rate-1] ^= 0x80;
    keccak_permute(state);

    while (outlen > rate) {
        memcpy(out, tstate, rate);
        keccak_permute(state);
        out += rate; outlen -= rate;
    }
    memcpy(out, tstate, outlen);
}

void shake128(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen) {
    shake(out, outlen, in, inlen, SHAKE128_RATE);
}

void shake256(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen) {
    shake(out, outlen, in, inlen, SHAKE256_RATE);
}
