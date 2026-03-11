/*
 * NOTICE: This file is protected under RCF-PL v1.2.6
 * [RCF:RESTRICTED]
 *
 * compiler.h — Однопроходной компилятор A-Code (Cortex-M)
 */

#ifndef SENTINEL_COMPILER_H
#define SENTINEL_COMPILER_H

#include <stdint.h>

/* Компиляция inline A-Code строки в байткод
 * source   — указатель на A-Code текст в Flash
 * out      — статический буфер для байткода
 * max_size — размер буфера
 * Возвращает число записанных байт, -1 при ошибке
 */
int compile(const char* source, uint8_t* out, int max_size);

#endif
