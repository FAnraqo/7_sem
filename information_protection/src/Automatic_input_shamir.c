#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/functions.h"

#define BMP_HEADER_SIZE 54

int Automatic_input_shamir(const char *bmp_path, long long *best_p, long long *best_Ca, long long *best_Cb) {

    int N = rand() % 100 + 1;

    FILE *fin = fopen(bmp_path, "rb");
    if (!fin) {
        printf("Не удалось открыть файл: %s\n", bmp_path);
        return 0;
    }
    fseek(fin, 0, SEEK_END);
    size_t file_size = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    unsigned char *orig_data = malloc(file_size);
    size_t read_bytes = fread(orig_data, 1, file_size, fin);
    if (read_bytes != file_size) {
        printf("Ошибка чтения файла: считано %zu из %zu байт\n", read_bytes, file_size);
        fclose(fin);
        free(orig_data);
        return 0;
    }
    fclose(fin);

    unsigned char *enc_data = malloc(file_size);
    unsigned char *dec_data = malloc(file_size);
    if (!enc_data || !dec_data) {
        printf("Ошибка выделения памяти\n");
        free(orig_data);
        if (enc_data) free(enc_data);
        if (dec_data) free(dec_data);
        return 0;
    }

    int found_count = 0;

    for (long long p = 257; p <= 300; p++) {
        if (Farm(p, 100) != 1) continue;
        long long phi = p - 1;
        for (long long C_a = 2; C_a < phi; C_a++) {
            if (NOD(C_a, phi) != 1) continue;
            for (long long C_b = 2; C_b < phi; C_b++) {
                if (NOD(C_b, phi) != 1) continue;

                memcpy(enc_data, orig_data, BMP_HEADER_SIZE);
                for (size_t i = BMP_HEADER_SIZE; i < file_size; i++) {
                    long long D_a, D_b;
                    long long enc = Shamir_encrypt(orig_data[i], p, C_a, C_b, &D_a, &D_b);
                    enc_data[i] = (unsigned char)(enc & 0xFF);
                }

                memcpy(dec_data, enc_data, BMP_HEADER_SIZE);
                for (size_t i = BMP_HEADER_SIZE; i < file_size; i++) {
                    long long dec = Shamir_decrypt(enc_data[i], p, mod_inverse(C_a, phi), mod_inverse(C_b, phi));
                    dec_data[i] = (unsigned char)(dec & 0xFF);
                }

                int match = 0, total = 0;
                for (size_t i = 0; i < file_size; i++) {
                    if (orig_data[i] == dec_data[i]) match++;
                    total++;
                }
                if (total > 0 && match == total) {
                    found_count++;
                    if (found_count == N) {
                        *best_p = p;
                        *best_Ca = C_a;
                        *best_Cb = C_b;
                        free(orig_data);
                        free(enc_data);
                        free(dec_data);
                        return 1;
                    }
                }
            }
        }
    }

    free(orig_data);
    free(enc_data);
    free(dec_data);
    return 0;
}