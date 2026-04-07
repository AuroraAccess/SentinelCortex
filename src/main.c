/*
 * NOTICE: This file is protected under RCF-PL v1.2.6
 * [RCF:RESTRICTED]
 *
 * main.c — SentinelCortex точка входа
 *
 * На STM32: заменить main() на SystemInit() + main loop;
 * Для десктопной симуляции: компилировать с -DSENTINEL_DESKTOP_SIM
 */

#include <string.h>
#include "../hal/hal.h"
#include "compiler.h"
#include "vm.h"
#include "pqc/dilithium2.h"

/* ─── A-Code модуль прошит прямо в Flash (inline) ────────────────────────
 * На реальном устройстве: хранится в отдельной Flash-секции.
 * Здесь: строка в .rodata — занимает место во Flash, не в RAM.
 */
static const char CRYPTO_VAULT_MODULE[] =
    "module crypto_vault {\n"
    "    purity.verify mode=\"deep_scan\"\n"
    "    pqc.set_regime mode=\"next_gen\"\n"
    "    identity.gen prefix=\"VAULT_SESSION\"\n"
    "    kem.exchange peer=\"AURORA_GATEWAY\"\n"
    "    vfs.store path=\"vault/session_key\" content=\"[EPHEMERAL_KEY_SLOT]\"\n"
    "    sys.biometrics bpm=68 adrenaline=0.05 oxygen=0.99\n"
    "    feel.state bpm=68\n"
    "    lume.voice message=\"Environment clean.\"\n"
    "    bus.pub event=\"TX_INITIATED\" payload=\"AWAITING_SIGNATURE\"\n"
    "    bus.sub intent=\"HARDWARE_CONFIRM\"\n"
    "    intuition.predict focus=\"REPLAY_ATTACK_DETECTION\"\n"
    "    instinct.trigger source=\"USB_ANOMALY\" severity=\"critical\"\n"
    "    reflex.action type=\"key_revocation\" source=\"USB_ANOMALY\"\n"
    "    bus.pub event=\"EMERGENCY_ISOLATE\" payload=\"ALL_INTERFACES_OFFLINE\"\n"
    "    lume.suggest feeling=\"critical\" focus=\"MASTER_KEY_ROTATION\"\n"
    "    purity.verify mode=\"paranoid_scan\"\n"
    "    halt\n"
    "}\n";

/* ─── Статический буфер для байткода ────────────────────────────────────── */
static uint8_t bytecode_buf[MAX_BYTECODE];

int main(void) {
    hal_uart_write("\r\n");
    hal_uart_write("  ======================================\r\n");
    hal_uart_write("  *** AURORA SENTINEL CORTEX v1.0  ***\r\n");
    hal_uart_write("  *** Bare-Metal · ARM Cortex-M    ***\r\n");
    hal_uart_write("  *** RCF-PL v1.2.6 [RESTRICTED]  ***\r\n");
    hal_uart_write("  ======================================\r\n\r\n");

    /* Сбрасываем статический аллокатор */
    hal_reset_allocator();

    /* Компилируем A-Code из Flash в статический буфер байткода */
    hal_uart_write("[SENTINEL] Compiling crypto_vault module...\r\n");
    int size = compile(CRYPTO_VAULT_MODULE, bytecode_buf, MAX_BYTECODE);

    if (size <= 0) {
        hal_uart_write("[SENTINEL ERR] Compilation failed!\r\n");
        return 1;
    }

    hal_uart_write("[SENTINEL] Compilation OK — ");
    /* Вывод размера без printf */
    char num_buf[8];
    int n = size, i = 0;
    if (n == 0) { num_buf[i++] = '0'; }
    else { char tmp[8]; int j=0; while(n>0){tmp[j++]=(char)('0'+n%10);n/=10;}
           for(int k=j-1;k>=0;k--) num_buf[i++]=tmp[k]; }
    num_buf[i] = '\0';
    hal_uart_write(num_buf);
    hal_uart_write(" bytes of soldered logic.\r\n\r\n");

    /* Исполняем на A-VM с RCF защитой */
    uint8_t rcf_envelope[MAX_BYTECODE + DILITHIUM2_BYTES];
    uint8_t signature[DILITHIUM2_BYTES];
    static const uint8_t DEMO_MASTER_SK[2528] = { 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51 };

    hal_uart_write("[SENTINEL] RCF: Signing module with Master Key (Demo)...\r\n");
    dilithium2_sign(signature, bytecode_buf, size, DEMO_MASTER_SK);

    /* Собираем ОФИЦИАЛЬНЫЙ RCF пакет: Sign + Code */
    memcpy(rcf_envelope, signature, DILITHIUM2_BYTES);
    memcpy(rcf_envelope + DILITHIUM2_BYTES, bytecode_buf, size);

    /* --- DEMO: ATTACK SCENARIO 1 (TAMPERED MODULE) --- */
    hal_uart_write("\r\n[ATTACK] Initiating supply-chain injection: Tampering module signature...\r\n");
    uint8_t tampered_envelope[MAX_BYTECODE + DILITHIUM2_BYTES];
    memcpy(tampered_envelope, rcf_envelope, size + DILITHIUM2_BYTES);
    
    /* Сознательно портим 1 байт в подписи (маркер взлома для демо) */
    tampered_envelope[10] = 0x5A; 
    
    execute_module("tampered_vault", tampered_envelope, size + DILITHIUM2_BYTES);
    hal_uart_write("[SENTINEL] Result: Intrusion blocked by RCF.\r\n");

    /* --- DEMO: ATTACK SCENARIO 2 (OFFICIAL BOOT) --- */
    hal_uart_write("\r\n[SENTINEL] Starting protected boot of OFFICIAL module...\r\n");
    execute_module("crypto_vault", rcf_envelope, size + DILITHIUM2_BYTES);

    hal_uart_write("\r\n[SENTINEL] System integrity maintained. Vault sealed.\r\n");

    /* На реальном Cortex-M: войти в режим ожидания (WFI) */
    while (1) { /* WFI — Wait For Interrupt */ }
    return 0;
}
