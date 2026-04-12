/* NOTICE: This file is protected under RCF-PL v1.3
 * [RCF:PROTECTED]
 *
 * compiler.c — Однопроходной компилятор A-Code для ARM Cortex-M
 *
 * Без malloc, без heap. Работает только со стеком и статическими буферами.
 */

#include "compiler.h"
#include "opcode.h"
#include "../hal/hal.h"
#include <string.h>

/* Таблица: текстовая команда → опкод */
static const struct { const char* name; uint8_t op; } OPCODE_TABLE[] = {
    { "identity.gen",       OP_IDENTITY_GEN      },
    { "kem.exchange",       OP_KEM_EXCHANGE      },
    { "pqc.set_regime",     OP_PQC_SET_REGIME    },
    { "vfs.store",          OP_VFS_STORE         },
    { "vfs.fetch",          OP_VFS_FETCH         },
    { "bus.pub",            OP_BUS_PUB           },
    { "bus.sub",            OP_BUS_SUB           },
    { "pulse.emit",         OP_PULSE_EMIT        },
    { "sys.biometrics",     OP_SYS_BIOMETRICS    },
    { "feel.state",         OP_FEEL_STATE        },
    { "instinct.trigger",   OP_INSTINCT_TRIGGER  },
    { "reflex.action",      OP_REFLEX_ACTION     },
    { "intuition.predict",  OP_INTUITION_PREDICT },
    { "lume.voice",         OP_LUME_VOICE        },
    { "lume.suggest",       OP_LUME_SUGGEST      },
    { "purity.verify",      OP_PURITY_VERIFY     },
    { "halt",               OP_HALT              },
    { NULL, 0 }
};

/* Лексер встроен прямо в компилятор для минимизации памяти */
static const char* src;
static int pos_g;

static void skip_whitespace(void) {
    while (src[pos_g] == ' ' || src[pos_g] == '\r') pos_g++;
}

static int read_word(char* buf, int max) {
    skip_whitespace();
    int len = 0;
    while (src[pos_g] && src[pos_g] != ' ' && src[pos_g] != '\n'
           && src[pos_g] != '{' && src[pos_g] != '}' && src[pos_g] != '='
           && len < max - 1) {
        buf[len++] = src[pos_g++];
    }
    buf[len] = '\0';
    return len;
}

int compile(const char* source, uint8_t* out, int max_size) {
    src   = source;
    pos_g = 0;
    int bytecode_pos = 0;
    int in_module = 0;
    char word[64];

    while (src[pos_g]) {
        /* Пропустить комментарии */
        if (src[pos_g] == '#') {
            while (src[pos_g] && src[pos_g] != '\n') pos_g++;
            continue;
        }
        /* Пропустить переносы строк и пробелы */
        if (src[pos_g] == '\n' || src[pos_g] == '\r' || src[pos_g] == ' ') { pos_g++; continue; }

        /* Проверяем управляющие символы до вызова read_word */
        if (src[pos_g] == '{') {
            if (bytecode_pos < max_size) out[bytecode_pos++] = OP_INIT_MOD;
            in_module = 1;
            pos_g++;
            continue;
        }
        if (src[pos_g] == '}') {
            in_module = 0;
            pos_g++;
            continue;
        }

        int len = read_word(word, sizeof(word));
        if (len == 0) { pos_g++; continue; }

        if (strcmp(word, "module") == 0) {
            read_word(word, sizeof(word)); /* имя модуля */
            continue;
        }

        if (!in_module) continue;

        /* Пропустить аргументы (key=value) */
        if (strchr(word, '=')) continue;
        if (src[pos_g] == '=') { /* Если слово закончилось перед '=' */
            while (src[pos_g] && src[pos_g] != ' ' && src[pos_g] != '\n') pos_g++;
            continue;
        }

        /* Поиск опкода */
        for (int i = 0; OPCODE_TABLE[i].name != NULL; i++) {
            if (strcmp(word, OPCODE_TABLE[i].name) == 0) {
                if (bytecode_pos < max_size)
                    out[bytecode_pos++] = OPCODE_TABLE[i].op;
                break;
            }
        }
    }
    return bytecode_pos;
}
