/* NOTICE: This file is protected under RCF-PL v1.3
 * [RCF:PUBLIC]
 *
 * vm.h — Aurora Sentinel VM для ARM Cortex-M
 */

#ifndef SENTINEL_VM_H
#define SENTINEL_VM_H

#include <stdint.h>

/* Максимальный размер байткода (ограничен Flash) */
#define MAX_BYTECODE 256

/* Выполнение модуля из байткода */
void execute_module(const char* name, const uint8_t* bytecode, int size);

#endif
