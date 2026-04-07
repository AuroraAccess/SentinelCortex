#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../pqc/dilithium2.h"

/* Mock HAL for Host-side tool */
void hal_uart_write(const char* s) {
    if (getenv("SENTINEL_VERBOSE")) printf("%s", s);
}

void hal_uart_write_hex(uint8_t b) {
    if (getenv("SENTINEL_VERBOSE")) printf("%02X", b);
}

int main(int argc, char** argv) {
    if (argc < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("==================================================\n");
        printf("  SentinelCortex RCF Signer Utility v1.1 (Native)\n");
        printf("  Protected under RCF-PL v1.2.6 [RCF:RESTRICTED]\n");
        printf("==================================================\n\n");
        printf("Usage:\n");
        printf("  %s keygen             - Generate Master PQC Keypair\n", argv[0]);
        printf("  %s sign <in> <out>    - Sign A-Code module with Master Key\n", argv[0]);
        printf("  %s --version          - Show version info\n", argv[0]);
        return 1;
    }

    const char* mode = argv[1];

    if (strcmp(mode, "--version") == 0) {
        printf("RCF-Signer v1.1 (Build: arm64-apple-darwin)\n");
        return 0;
    }

    if (strcmp(mode, "keygen") == 0) {
        uint8_t pk[1312];
        uint8_t sk[2528];
        printf("[RCF] Generating Post-Quantum Master Keys (Dilithium-2)...\n");
        dilithium2_keygen(pk, sk);
        
        FILE* fpk = fopen("master_public.key", "wb");
        fwrite(pk, 1, 1312, fpk);
        fclose(fpk);
        
        FILE* fsk = fopen("master_private.key", "wb");
        fwrite(sk, 1, 2528, fsk);
        fclose(fsk);
        
        printf("[SUCCESS] Master Keys created: master_public.key, master_private.key\n");
    } 
    else if (strcmp(mode, "sign") == 0) {
        if (argc < 4) {
            printf("Error: Mode 'sign' requires <input_file> and <output_file>.\n");
            return 1;
        }
        const char* in_path = argv[2];
        const char* out_path = argv[3];

        /* 1. Load Master Private Key */
        uint8_t sk[2528];
        FILE* fsk = fopen("master_private.key", "rb");
        if (!fsk) {
            printf("[!!] RCF ERROR: master_private.key not found in current directory!\n");
            printf("     Run 'rcf-signer keygen' first or provide the key.\n");
            return 1;
        }
        fread(sk, 1, 2528, fsk);
        fclose(fsk);

        /* 2. Read A-Code Bytecode */
        FILE* fin = fopen(in_path, "rb");
        if (!fin) {
            printf("[!!] ERROR: Cannot open input file: %s\n", in_path);
            return 1;
        }
        fseek(fin, 0, SEEK_END);
        long size = ftell(fin);
        fseek(fin, 0, SEEK_SET);
        uint8_t* code = malloc(size);
        fread(code, 1, size, fin);
        fclose(fin);

        /* 3. Real PQC Signature with Master Key */
        printf("[RCF] Signing %s (Size: %ld bytes)...\n", in_path, size);
        uint8_t signature[DILITHIUM2_BYTES];
        dilithium2_sign(signature, code, size, sk);

        /* 4. Construct RCF Envelope: [SIG] + [CODE] */
        FILE* fout = fopen(out_path, "wb");
        if (!fout) {
            printf("[!!] ERROR: Cannot open output file: %s\n", out_path);
            return 1;
        }
        fwrite(signature, 1, DILITHIUM2_BYTES, fout);
        fwrite(code, 1, size, fout);
        fclose(fout);

        printf("[SUCCESS] RCF-Signed Module: %s\n", out_path);
        printf("[DETAIL] Integrity Hash attached. PQC Seal active.\n");
        free(code);
    }

    return 0;
}
