# NOTICE: This file is protected under RCF-PL v1.2.6
# [RCF:RESTRICTED]
#
# Makefile — SentinelCortex (ARM Cortex-M Build System)
#
# Режимы сборки:
#   make sim        — Симуляция на Mac/Linux (gcc)
#   make sim-strict — Строгая симуляция (-Werror -Wpedantic)
#   make tool       — Сборка rcf-signer (инструмент подписи)
#   make stm32f4    — Кросс-компиляция для STM32F4

# ─── Общие файлы ─────────────────────────────────────────────────────────────
SRCS = src/compiler.c \
       src/vm.c       \
       src/main.c     \
       hal/hal_stm32.c \
       src/pqc/fips202.c \
       src/pqc/ntt.c \
       src/pqc/dilithium2.c \
       src/pqc/kyber.c

OBJDIR  = /tmp/sentinel-cortex-build
CFLAGS_COMMON = -Wall -Wextra -Os -std=c99 \
                -Isrc -Ihal

# Host Tool (Signer)
TOOL_SRCS = src/tools/rcf_signer.c src/pqc/dilithium2.c src/pqc/fips202.c src/pqc/ntt.c src/pqc/kyber.c
TOOL_BIN  = bin/rcf-signer

.PHONY: tool
tool:
	@mkdir -p bin
	$(CC) $(CFLAGS_COMMON) $(TOOL_SRCS) -o $(TOOL_BIN)
	@echo "[HOST] Build complete: $(TOOL_BIN)"

# ─── Режим 1: Симуляция на Mac/Linux ─────────────────────────────────────────
.PHONY: sim
sim: $(OBJDIR)
	gcc $(CFLAGS_COMMON) \
	    -DSENTINEL_DESKTOP_SIM \
	    $(SRCS) \
	    -o $(OBJDIR)/sentinel_cortex_sim
	@echo ""
	@echo "  ✅  SentinelCortex (Desktop Sim) built: $(OBJDIR)/sentinel_cortex_sim"
	@echo "  Run: $(OBJDIR)/sentinel_cortex_sim"
	@echo ""

.PHONY: sim-strict
sim-strict: $(OBJDIR)
	gcc $(CFLAGS_COMMON) -Wpedantic -Werror \
	    -DSENTINEL_DESKTOP_SIM \
	    $(SRCS) \
	    -o $(OBJDIR)/sentinel_cortex_strict
	@echo "  ✅  Strict build PASSED (Military Grade)."
	@echo "  Run: $(OBJDIR)/sentinel_cortex_strict"

# ─── Режим 2: STM32F4 (Trezor One / STM32F2xx / STM32F4xx) ──────────────────
# Ledger Nano S: STM32F042  (Cortex-M0)
# Trezor One:    STM32F205  (Cortex-M3)
# Trezor T:      STM32F427  (Cortex-M4)
CC_ARM   = arm-none-eabi-gcc
CFLAGS_ARM = $(CFLAGS_COMMON) \
             -mcpu=cortex-m4 \
             -mthumb         \
             -mfloat-abi=soft \
             -ffunction-sections \
             -fdata-sections     \
             --specs=nano.specs

.PHONY: stm32f4
stm32f4: $(OBJDIR)
	$(CC_ARM) $(CFLAGS_ARM) \
	    -DSENTINEL_STM32F4 \
	    $(SRCS) \
	    -Wl,--gc-sections \
	    -o $(OBJDIR)/sentinel_stm32f4.elf
	arm-none-eabi-size $(OBJDIR)/sentinel_stm32f4.elf
	@echo "  ✅  SentinelCortex (STM32F4) built."

# ─── Режим 3: STM32WB55 (Ledger Nano X — BLE + Cortex-M4) ───────────────────
.PHONY: stm32wb
stm32wb: $(OBJDIR)
	$(CC_ARM) $(CFLAGS_ARM) \
	    -mcpu=cortex-m4 \
	    -DSENTINEL_STM32WB \
	    $(SRCS) \
	    -o $(OBJDIR)/sentinel_stm32wb.elf
	@echo "  ✅  SentinelCortex (STM32WB / Ledger Nano X) built."

# ─── Утилиты ─────────────────────────────────────────────────────────────────
$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR)
