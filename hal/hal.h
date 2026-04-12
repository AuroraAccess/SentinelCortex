/* NOTICE: This file is protected under RCF-PL v1.3
 * [RCF:PUBLIC]
 *
 * hal.h — Hardware Abstraction Layer для ARM Cortex-M
 *
 * Этот слой скрывает различия между STM32F4, STM32WB и другими чипами.
 * Заменяет printf → UART, malloc → статика, fopen → Flash.
 */

#ifndef SENTINEL_HAL_H
#define SENTINEL_HAL_H

#include <stdint.h>
#include <stddef.h>

/* ─── UART вывод (вместо printf) ─────────────────────────────────────────── */
void hal_uart_write(const char* msg);
void hal_uart_write_hex(uint8_t byte);

/* ─── Flash память (вместо файлового ввода-вывода) ───────────────────────── */
/* Адрес защищённого сектора (например, Sector 11 на STM32F4) */
#define HAL_FLASH_SECURE_ADDR  0x080E0000

/* Читает A-Code модуль из Flash по адресу */
const uint8_t* hal_flash_read(uint32_t address, uint32_t* out_size);

/* Физическое уничтожение данных в секторе (Key Shredding) */
void hal_flash_sector_erase(uint32_t address);

/* ─── Системный таймер ───────────────────────────────────────────────────── */
uint32_t hal_tick_ms(void);

/* ─── Статическая память (вместо malloc) ─────────────────────────────────── */
/* Простой bump-allocator — никакого heap, никакого free */
void*    hal_alloc(size_t size);
void     hal_reset_allocator(void);

/* ─── Логирование (платформонезависимое) ────────────────────────────────── */
#define LOG_INFO(msg)  hal_uart_write("[SENTINEL] " msg "\r\n")
#define LOG_WARN(msg)  hal_uart_write("[SENTINEL WARN] " msg "\r\n")
#define LOG_ERROR(msg) hal_uart_write("[SENTINEL ERR] " msg "\r\n")

#endif /* SENTINEL_HAL_H */
