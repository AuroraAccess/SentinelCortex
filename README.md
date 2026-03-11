 [RCF:PROTECTED]
 NOTICE: This file is protected under RCF-PL v1.2.6

# ⚡ SentinelCortex: Aurora Sentinel for ARM Cortex-M

> **"The same logic. The same protection. Zero grams of overhead."**

**SentinelCortex** is the bare-metal port of [Aurora Sentinel](../AuroraSentinel) for **ARM Cortex-M** microcontrollers — the exact chips powering 90% of hardware crypto wallets.

---

## 🎯 Why Cortex-M?

| Device | Chip | Architecture |
|---|---|---|
| Ledger Nano S | STM32F042 | Cortex-M0 |
| Ledger Nano X | STM32WB55 | Cortex-M4 |
| Trezor One | STM32F205 | Cortex-M3 |
| Trezor T | STM32F427 | Cortex-M4 |
| Most IoT Sensors | STM32F4xx | Cortex-M4 |

---

## 🧠 Architecture

```
[ crypto_vault.acode ]   ← Identical A-Code logic
         ↓
[ SentinelCortex VM ]    ← Static memory, no malloc
         ↓
[ HAL Layer ]            ← UART, Flash, STM32 peripherals
         ↓
[ ARM Cortex-M Silicon ] ← Bare Metal
```

### Key Differences from AuroraSentinel (ARM64)
| Feature | AuroraSentinel | SentinelCortex |
|---|---|---|
| Memory | `malloc` / heap | Static buffers only |
| Output | `printf` / stdio | UART HAL |
| File I/O | `fopen` / OS | Flash memory read |
| Compiler | `gcc` | `arm-none-eabi-gcc` |
| A-Code modules | Same | **Identical** ✅ |

---

## 🛠️ Build & Flash

```bash
# Install cross-compiler (macOS)
brew install arm-none-eabi-gcc

# Build for STM32F4
make TARGET=stm32f4

# Flash to device (requires OpenOCD)
make flash
```

---

## 📜 License
Protected under **RCF-PL v1.2.6** — `[RCF:RESTRICTED]`
