#ifndef DILITHIUM2_H
#define DILITHIUM2_H

#include <stdint.h>

#define DILITHIUM2_BYTES 2420

/* Создает подпись сообщения с использованием реальной NTT и Keccak математики */
void dilithium2_sign(uint8_t sig[DILITHIUM2_BYTES], const uint8_t *m, uint32_t mlen);

/* Проверяет подпись Dilithium-2 */
int dilithium2_verify(const uint8_t sig[DILITHIUM2_BYTES], const uint8_t *m, uint32_t mlen, const uint8_t pk[1312]);

/* Генерирует пару ключей PQC */
void dilithium2_keygen(uint8_t pk[1312], uint8_t sk[2528]);

#endif
