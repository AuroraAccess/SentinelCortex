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
    if (argc < 2) {
        printf("SentinelCortex RCF Signer Utility v1.0\n");
        printf("Usage:\n");
        printf("  %s keygen             - Generate Master PQC Keypair\n", argv[0]);
        printf("  %s sign <in> <out>    - Sign A-Code module with Master Key\n", argv[0]);
        return 1;
    }

    const char* mode = argv[1];

    if (strcmp(mode, "keygen") == 0) {
        uint8_t pk[1312];
        uint8_t sk[2528];
        dilithium2_keygen(pk, sk);
        
        FILE* fpk = fopen("master_public.key", "wb");
        fwrite(pk, 1, 1312, fpk);
        fclose(fpk);
        
        FILE* fsk = fopen("master_private.key", "wb");
        fwrite(sk, 1, 2528, fsk);
        fclose(fsk);
        
        printf("[SUCCESS] PQC Master Keys generated: master_public.key, master_private.key\n");
    } 
    else if (strcmp(mode, "sign") == 0) {
        if (argc < 4) {
            printf("Error: Mode 'sign' requires <input_file> and <output_file>.\n");
            return 1;
        }
        const char* in_path = argv[2];
        const char* out_path = argv[3];

        /* 1. Read A-Code Bytecode */
        FILE* fin = fopen(in_path, "rb");
        fseek(fin, 0, SEEK_END);
        long size = ftell(fin);
        fseek(fin, 0, SEEK_SET);
        uint8_t* code = malloc(size);
        fread(code, 1, size, fin);
        fclose(fin);

        /* 2. Sign with Master Private Key (Mock/Demo) */
        uint8_t signature[DILITHIUM2_BYTES];
        dilithium2_sign(signature, code, size);

        /* 3. Create RCF Envelope: [SIG] + [CODE] */
        FILE* fout = fopen(out_path, "wb");
        fwrite(signature, 1, DILITHIUM2_BYTES, fout);
        fwrite(code, 1, size, fout);
        fclose(fout);

        printf("[SUCCESS] RCF-Signed Module created: %s\n", out_path);
        free(code);
    }

    return 0;
}
