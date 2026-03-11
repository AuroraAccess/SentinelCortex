# 🎬 SentinelCortex: LIVE Demo Script (MoD/B2B Presentation)

Этот документ содержит описание каждого шага для записи видео-демонстрации безопасности SentinelCortex.

## Step 1: Strict Compilation (The "Safety-First" Build)
**Command:**
```bash
make sim-strict
```
**What's happening:**
- We use `-Wall -Wextra -Wpedantic -Werror` flags.
- **Why:** This ensures the codebase has **ZERO** undefined behaviors, unused variables, or non-standard C constructs.
- **Result:** A "Military-Grade" binary proof that the code is clean and reliable.

---

## Step 2: On-Premise Signing (Key Sovereignty)
**Commands:**
```bash
# Generate Master Keys
./bin/rcf-signer keygen

# Sign the A-Code Module
./bin/rcf-signer sign src/main.c logic.rcf
```
**What's happening:**
- **KeyGen:** SentinelCortex generates a **Dilithium-2 (ML-DSA)** keypair offline. The Secret Key never leaves the client's perimeter.
- **Signing:** The `rcf-signer` tool creates a cryptographically protected envelope (`logic.rcf`) containing the A-Code bytecode and its PQC signature.

---

## Step 3: Sentinel VM Execution (Real-Time Protection)
**Command:**
```bash
./bin/sentinel_sim
```
**Logs Explained:**
- `[RCF] Verifying module envelope...`: The VM checks the PQC signature against the hardcoded **Master Public Key (MPK)**.
- `[RCF] Integrity VALID`: Proof that the module hasn't been tampered with.
- `[PQC] Initializing REAL Dilithium-2...`: The VM executes high-performance Lattice-based math (NTT + Keccak) to ensure the logic is future-proof.
- `[KEM] Initiating Kyber-512...`: Secure session establishment using Post-Quantum Key Encapsulation.
- `[REFLEX] USB_ENABLE pin pulled LOW`: Demonstrating the autonomous "Reflexive Security" feature where the kernel cuts off physical interfaces upon detecting a threat.

---

## Step 4: RCF-CLI Cryptographic Audit
**Command:**
```bash
export RCF_LICENSE_KEY=RCF-AUDIT-ADMIN-KEY-GLOBAL
rcf-cli audit .
```
**What's happening:**
- The `rcf-cli` scans the entire project for "Protected Asset" markers.
- It verifies that the source code, Makefile, and binaries have not been modified since the last official release.
- **Result:** `✅ RCF-Audit Complete`. This is the final stamp of approval for deployment.

---
**Summary for Video:**
SentinelCortex isn't just a library; it's a **Fortress on a Chip**. We've proven that we can deliver signed, future-proof logic to bare-metal devices with 100% integrity and zero complexity for the end-user.
