/* NOTICE: This file is protected under RCF-PL v1.3
 * [RCF:PROTECTED]
 *
 * hal_stm32.c — Реализация HAL для STM32 (Cortex-M3/M4)
 *
 * На реальном железе здесь подключаются CMSIS/HAL заголовки STM32.
 * Для симуляции на десктопе используется stdio.
 */

#include "hal.h"
#include <string.h>

/* ─── Режим: симуляция на десктопе (для тестов без чипа) ──────────────────
 * При сборке для реального STM32 заменить на регистры UART:
 *   USART2->DR = *msg++;
 */
#ifdef SENTINEL_DESKTOP_SIM
#include <stdio.h>
void hal_uart_write(const char* msg) {
    fputs(msg, stdout);
    fflush(stdout);
}
void hal_uart_write_hex(uint8_t byte) {
    printf("0x%02X ", byte);
}
uint32_t hal_tick_ms(void) {
    return 0; /* На десктопе не нужен */
}
#else
/* ─── Реальная STM32 UART реализация ─────────────────────────────────────── */
/* Подключить: #include "stm32f4xx_hal.h" */
extern void UART_Transmit(const char* msg); /* Реализуется BSP слоем */
void hal_uart_write(const char* msg) { UART_Transmit(msg); }
void hal_uart_write_hex(uint8_t byte) {
    char buf[5] = "0x00 ";
    const char* hex = "0123456789ABCDEF";
    buf[2] = hex[(byte >> 4) & 0xF];
    buf[3] = hex[byte & 0xF];
    hal_uart_write(buf);
}
uint32_t hal_tick_ms(void) {
    return 0; /* Заменить на HAL_GetTick() */
}
#endif

/* ─── Flash чтение (имитация для десктопа) ──────────────────────────────── */
static uint8_t  _flash_buf[512];
static uint32_t _flash_size = 0;

void hal_flash_load(const uint8_t* data, uint32_t size) {
    if (size > 512) size = 512;
    memcpy(_flash_buf, data, size);
    _flash_size = size;
}

const uint8_t* hal_flash_read(uint32_t address, uint32_t* out_size) {
    (void)address;
    *out_size = _flash_size;
    return _flash_buf;
}

void hal_flash_sector_erase(uint32_t address) {
    (void)address;
    hal_uart_write("[HAL] FLASH: Erasing Sector 11 (0x080E0000)...\r\n");
    memset(_flash_buf, 0xFF, 512); // Стирание во Flash обычно превращает всё в 0xFF
    _flash_size = 0;
    hal_uart_write("[HAL] FLASH: Sector erased. Secrets destroyed.\r\n");
}

/* ─── Статический bump-allocator (вместо heap/malloc) ───────────────────── */
/* На Cortex-M heap может быть всего 4–16KB — не расходуем его */
#define STATIC_HEAP_SIZE 4096
static uint8_t  _heap[STATIC_HEAP_SIZE];
static uint32_t _heap_ptr = 0;

void* hal_alloc(size_t size) {
    /* Выравнивание по 4 байта */
    size = (size + 3) & ~3U;
    if (_heap_ptr + size > STATIC_HEAP_SIZE) {
        hal_uart_write("[SENTINEL ERR] Out of static memory!\r\n");
        return 0;
    }
    void* ptr = &_heap[_heap_ptr];
    _heap_ptr += size;
    return ptr;
}

void hal_reset_allocator(void) {
    _heap_ptr = 0;
}
