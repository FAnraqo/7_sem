#include <stdio.h>
#include <stdlib.h>
#include "../include/functions.h"

#define BMP_HEADER_SIZE 54
#define MAX_DATA_SIZE (100 * 1024 * 1024) // 100 MB

int Encrypt_Decrypt_shamir(const char *filepath_in, long long p, long long C_a, long long C_b) {
    long long D_a = mod_inverse(C_a, p - 1);
    long long D_b = mod_inverse(C_b, p - 1);

    if (D_a == -1 || D_b == -1) {
        printf("Ошибка: не существует обратного элемента для C_a или C_b\n");
        return 0;
    }

    unsigned char *encrypted_data = malloc(MAX_DATA_SIZE);
    if (!encrypted_data) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return 0;
    }
    size_t encrypted_size = 0;

    // --- Шифрование ---
    FILE *fin = fopen(filepath_in, "rb");
    if (!fin) {
        printf("Ошибка открытия %s\n", filepath_in);
        free(encrypted_data);
        return 0;
    }

    FILE *fenc = fopen("temp/lab4_e.bmp", "wb");
    if (!fenc) {
        printf("Ошибка открытия temp/lab4_e.bmp\n");
        fclose(fin);
        free(encrypted_data);
        return 0;
    }

    unsigned char header[BMP_HEADER_SIZE];
    if (fread(header, 1, BMP_HEADER_SIZE, fin) != BMP_HEADER_SIZE) {
        printf("Ошибка чтения заголовка BMP\n");
        fclose(fin);
        fclose(fenc);
        free(encrypted_data);
        return 0;
    }
    fwrite(header, 1, BMP_HEADER_SIZE, fenc);

    unsigned char byte;
    while (fread(&byte, 1, 1, fin) == 1) {
        long long enc = Shamir_encrypt(byte, p, C_a, C_b, &D_a, &D_b);
        unsigned char out = (unsigned char)(enc & 0xFF);
        fwrite(&out, 1, 1, fenc);

        if (encrypted_size < MAX_DATA_SIZE) {
            encrypted_data[encrypted_size++] = out;
        } else {
            fprintf(stderr, "Ошибка: массив переполнен!\n");
            fclose(fin);
            fclose(fenc);
            free(encrypted_data);
            return 0;
        }
    }
    fclose(fin);
    fclose(fenc);

    if (encrypted_size == 0) {
        printf("Нет данных для дешифровки! Сначала зашифруйте.\n");
        free(encrypted_data);
        return 0;
    }

    // --- Дешифрование ---
    FILE *fdec = fopen("temp/lab4_d.bmp", "wb");
    if (!fdec) {
        printf("Ошибка открытия temp/lab4_d.bmp\n");
        free(encrypted_data);
        return 0;
    }
    fwrite(header, 1, BMP_HEADER_SIZE, fdec);

    for (size_t i = 0; i < encrypted_size; i++) {
        unsigned char byte_d = encrypted_data[i];
        long long dec = Shamir_decrypt(byte_d, p, D_a, D_b);
        unsigned char out = (unsigned char)(dec & 0xFF);
        fwrite(&out, 1, 1, fdec);
    }
    fclose(fdec);

    free(encrypted_data);

    // --- Сравнение файлов ---
    FILE *forig = fopen(filepath_in, "rb");
    FILE *fres = fopen("temp/lab4_d.bmp", "rb");
    if (!forig || !fres) {
        printf("Ошибка открытия файлов для сравнения\n");
        if (forig) fclose(forig);
        if (fres) fclose(fres);
        return 0;
    }
    int identical = 1, byte_pos = 0, diff_count = 0;
    unsigned char b1, b2;
    while (fread(&b1, 1, 1, forig) == 1 && fread(&b2, 1, 1, fres) == 1) {
        if (b1 != b2) {
            identical = 0;
            diff_count++;
        }
        byte_pos++;
    }
    fclose(forig);
    fclose(fres);

    if (identical && byte_pos > 0) {
        printf("Файлы идентичны!\n");
    } else {
        printf("Файлы различаются! Всего отличий: %d\nКоэффициент совпадения: %.2f%%\n",
               diff_count, (byte_pos - diff_count) / (float)byte_pos * 100);
    }

    return 1;
}