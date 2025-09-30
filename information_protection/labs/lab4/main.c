#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <../include/functions.h>

#define BMP_HEADER_SIZE 54
#define MAX_DATA_SIZE (100 * 1024 * 1024) // 100 MB для безопасного хранения в массиве

// НОД
long long gcd_ll(long long a, long long b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

// Глобальные буферы
unsigned char *encrypted_data = NULL; 
size_t encrypted_size = 0; 

int main() {
    long long p;
    long long C_a = 0, C_b = 0;
    long long D_a = -1, D_b = -1;

    srand((unsigned)time(NULL));

    // -------------------- выбор режима --------------------
    int menu;
    printf("Выберите режим ввода для шифра Шамира:\n");
    printf("1 - Самостоятельный ввод\n2 - Рандомные числа\n\nРежим работы: ");
    if (scanf("%d", &menu) != 1 || (menu != 1 && menu != 2)){
        printf("Ошибка ввода\n");
        return 1;
    }

    switch (menu) {
    case 1: {
        printf("Введите p (простое число): ");
        if (scanf("%lld", &p) != 1){
            printf("Ошибка ввода\n");
            return 1;
        }
        if (Farm(p, 100) != 1){
            printf("p не простое число\n");
            return 1;
        }

        long long phi = p - 1;

        do {
            printf("Введите C_a: ");
            scanf("%lld", &C_a);
        } while (gcd_ll(C_a, phi) != 1);

        do {
            printf("Введите C_b: ");
            scanf("%lld", &C_b);
        } while (gcd_ll(C_b, phi) != 1);

        break;
    }

    case 2: {
        int ok = 0;
        while (!ok) {
            p = (rand() % 1000000) + 10000000;
            if (Farm(p, 100)) ok = 1;
        }

        long long phi = p - 1;
        do { C_a = 2 + rand() % (phi - 2); } while (gcd_ll(C_a, phi) != 1);
        do { C_b = 2 + rand() % (phi - 2); } while (gcd_ll(C_b, phi) != 1);

        printf("Случайные числа:\np = %lld\nC_a = %lld\nC_b = %lld\n", p, C_a, C_b);
        break;
    }
    }

    // -------------------- вычисление обратных --------------------
    if (D_a == -1 || D_b == -1) {
        Shamir_encrypt(1, p, C_a, C_b, &D_a, &D_b);
    }
    if (D_a == -1 || D_b == -1) {
        fprintf(stderr, "Не удалось вычислить обратные D_a/D_b\n");
        return 1;
    }

    // -------------------- имя файла --------------------
    char filename_in[256];
    printf("Введите имя входного BMP файла (в папке Image): ");
    scanf("%s", filename_in);

    char filepath_in[512];
    sprintf(filepath_in, "image/%s", filename_in);

    // -------------------- выделение памяти под массив --------------------
    encrypted_data = malloc(MAX_DATA_SIZE);
    if (!encrypted_data) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return 1;
    }
    encrypted_size = 0;

    // -------------------- меню --------------------
    int mode, quit = 0;
    while (!quit) {
        printf("Меню:\n1 - Шифрование\n2 - Дешифрование\n3 - Выход\nВыберите: ");
        scanf("%d", &mode);

        switch (mode) {
        case 1: {
            FILE *fin = fopen(filepath_in, "rb");
            if (!fin) { printf("Ошибка открытия %s\n", filepath_in); break; }

            FILE *fout = fopen("temp/lab4_e.bmp", "wb");
            if (!fout) { printf("Ошибка открытия temp/lab4_e.bmp\n"); fclose(fin); break; }

            unsigned char header[BMP_HEADER_SIZE];
            fread(header, 1, BMP_HEADER_SIZE, fin);
            fwrite(header, 1, BMP_HEADER_SIZE, fout);

            unsigned char byte;
            while (fread(&byte, 1, 1, fin) == 1) {
                long long enc = Shamir_encrypt(byte, p, C_a, C_b, &D_a, &D_b);
                unsigned char out = (unsigned char)(enc & 0xFF);

                fwrite(&out, 1, 1, fout);

                if (encrypted_size < MAX_DATA_SIZE) {
                    encrypted_data[encrypted_size++] = out;
                } else {
                    fprintf(stderr, "Ошибка: массив переполнен!\n");
                    break;
                }
            }

            fclose(fin); fclose(fout);
            printf("Шифрование завершено. В массиве сохранено %zu байт\n", encrypted_size);
            break;
        }

        case 2: {
            if (encrypted_size == 0) {
                printf("Нет данных для дешифровки! Сначала зашифруйте.\n");
                break;
            }

            FILE *fin = fopen(filepath_in, "rb");
            if (!fin) { printf("Ошибка открытия %s\n", filepath_in); break; }

            FILE *fout = fopen("temp/lab4_d.bmp", "wb");
            if (!fout) { printf("Ошибка открытия temp/lab4_d.bmp\n"); fclose(fin); break; }

            unsigned char header[BMP_HEADER_SIZE];
            fread(header, 1, BMP_HEADER_SIZE, fin);
            fwrite(header, 1, BMP_HEADER_SIZE, fout);
            fclose(fin);

            for (size_t i = 0; i < encrypted_size; i++) {
                unsigned char byte_d = encrypted_data[i];
                long long dec = Shamir_decrypt(byte_d, p, D_a, D_b);
                unsigned char out = (unsigned char)(dec & 0xFF);
                fwrite(&out, 1, 1, fout);
            }

            fclose(fout);
            printf("Дешифрование завершено. Файл temp/lab4_d.bmp создан\n");
            break;
        }

        case 3:
            quit = 1;
            break;
        }
    }

    free(encrypted_data);
    return 0;
}
