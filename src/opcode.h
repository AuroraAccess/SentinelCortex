/*
 * NOTICE: This file is protected under RCF-PL v1.2.6
 * [RCF:RESTRICTED]
 *
 * opcode.h — Sentinel A-Code OpCode таблица (Cortex-M edition)
 * Идентична AuroraSentinel/src/opcode.h — A-Code кросс-платформенен
 */

#ifndef SENTINEL_OPCODE_H
#define SENTINEL_OPCODE_H

#include <stdint.h>

/* ───── Ядро ───── */
#define OP_INIT_MOD          0x01
#define OP_HALT              0x99

/* ───── Идентификация ───── */
#define OP_IDENTITY_GEN      0x05
#define OP_KEM_EXCHANGE      0x06
#define OP_PQC_SET_REGIME    0x07

/* ───── VFS (Flash на Cortex-M) ───── */
#define OP_VFS_STORE         0x10
#define OP_VFS_FETCH         0x11

/* ───── System Bus (SPI/I2C на Cortex-M) ───── */
#define OP_BUS_PUB           0x20
#define OP_BUS_SUB           0x21

/* ───── Пульс / Биометрика ───── */
#define OP_PULSE_EMIT        0x40
#define OP_SYS_BIOMETRICS    0x45
#define OP_FEEL_STATE        0x50

/* ───── Рефлексы и инстинкты ───── */
#define OP_INSTINCT_TRIGGER  0x60
#define OP_REFLEX_ACTION     0x65

/* ───── Интуиция ───── */
#define OP_INTUITION_PREDICT 0x70

/* ───── Lume ───── */
#define OP_LUME_VOICE        0xA0
#define OP_LUME_SUGGEST      0xA5

/* ───── Верификация ───── */
#define OP_PURITY_VERIFY     0xFF

#endif /* SENTINEL_OPCODE_H */
