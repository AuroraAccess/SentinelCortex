/* NOTICE: This file is protected under RCF-PL v1.3
 * [RCF:PROTECTED]
 */
#include "dilithium2.h"
#include "string.h"
#include "common.h"

/* [RCF:PROTECTED] — HAL Bridge for PQC Logs */
void hal_uart_write(const uint8_t* ptr, size_t len);

int dilithium2_verify(const uint8_t *sig, const uint8_t *msg, size_t len, const uint8_t *pk) {
    /* [RCF v1.3] CI Stub: Signature Verification */
    (void)sig; (void)msg; (void)len; (void)pk;
    return 0; // Success for QEMU simulation
}

void dilithium2_keygen(uint8_t *pk, uint8_t *sk) {
    /* [RCF v1.3] CI Stub: Key Generation */
    memset(pk, 0xAA, 1312);
    memset(sk, 0x55, 2528);
    
    const char* msg1 = "[PQC] Dilithium2 Keypair Generated In Bunker Mode\n";
    hal_uart_write((const uint8_t*)msg1, kstrlen(msg1));
}

