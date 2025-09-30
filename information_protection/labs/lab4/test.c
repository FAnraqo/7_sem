#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <../include/functions.h>

#define BMP_HEADER_SIZE 54

int main() {
    
    long long p;
    long long C_a, C_b;
    long long D_a, D_b;

    srand(time(NULL));

    int menu;
    printf("Выберите режим ввода для шифра Шамира:\n");
    printf("1 - Самостоятельный ввод\n2 - Рандомные числа\n\nРежим работы: ");
    if (scanf("%d", &menu) != 1 || (menu != 1 && menu != 2)){
        printf("Ошибка ввода\n");
        return 1;
    }

    switch (menu)
    {
    case 1:
        printf("Введите числа для шифра Шамира\n");
        printf("Введите p (простое число): ");
        if (scanf("%lld", &p) != 1){
            printf("Ошибка ввода\n");
            return 1;
        }
        if (Farm(p, 100) != 1){
            printf("p не простое число");
            return 1;
        }
        printf("Введите C_a (целое число): ");
        if (scanf("%lld", &C_a) != 1){
            printf("Ошибка ввода\n");
            return 1;
        }

        long long a = C_a, b = p - 1;
        while (b != 0) {
            long long t = a % b;
            a = b;
            b = t;
        }
        if (a == 1){
            return 1;
        }

        printf("Введите C_b (целое число): ");
        if (scanf("%lld", &C_b) != 1){
            printf("Ошибка ввода\n");
            return 1;
        }

        a = C_a, b = p - 1;
        while (b != 0) {
            long long t = a % b;
            a = b;
            b = t;
        }
        if (a == 1){
            return 1;
        }

        break;
    case 2:
        int correct = 0;
        while (correct != 1){
            p = rand() % 1000000 + 10000000;
            if (Farm(p, 100)){
                long long x_temp = Shamir_encrypt(10, p, 7, 5, &D_a, &D_b);
                long long result = Shamir_decrypt(x_temp, p, D_a, D_b);

                if ((D_a != -1 && D_b != -1) && result == 10){
                    correct = 1;
                }
            }
        }

        correct = 0;
        while (correct != 1){
            C_a = 2 + rand() % (p - 3);
            long long x_temp = Shamir_encrypt(10, p, C_a, 5, &D_a, &D_b);
            long long result = Shamir_decrypt(x_temp, p, D_a, D_b);

            long long a = C_a, b = p - 1;
            while (b != 0) {
                long long t = a % b;
                a = b;
                b = t;
            }

            if ((D_a != -1 && D_b != -1) && result == 10 && a == 1){
                correct = 1;
            }

            printf("C_a = %lld\n", C_a);
        }

        correct = 0;
        while (correct != 1){
            C_b = 2 + rand() % (p - 3);
            long long x_temp = Shamir_encrypt(10, p, C_a, C_b, &D_a, &D_b);
            long long result = Shamir_decrypt(x_temp, p, D_a, D_b);
            
            long long a = C_a, b = p - 1;
            while (b != 0) {
                long long t = a % b;
                a = b;
                b = t;
            }

            if ((D_a != -1 && D_b != -1) && result == 10 && a == 1){
                correct = 1;
            }
            printf("C_b = %lld\n", C_b);
        }

        printf("\nРандомные числа:\np = %lld\nC_a = %lld\nC_b = %lld\n\n", p, C_a, C_b);

        break;
    }

    long long x_2 = Shamir_encrypt(256, p, C_a, C_b, &D_a, &D_b);
    long long result = Shamir_decrypt(x_2, p, D_a, D_b);
    printf("%lld\n", result);

    // char filename_in[256];
    // char filepath_in[512], filepath_out_e[16], filepath_out_d[16];
    // printf("Введите имя входного BMP файла (в папке Image): ");
    // if (scanf("%s", filename_in) != 1){
    //     printf("Ошибка ввода названия входного файла");
    //     return 1;
    // }

    // sprintf(filepath_in, "%s/%s", "image", filename_in);
    // sprintf(filepath_out_e, "%s", "temp/lab4_e.bmp");
    // sprintf(filepath_out_d, "%s", "temp/lab4_d.bmp");

    // int mode, quit = 0;
    // while (quit == 0)
    // {
    //     printf("Меню работы:\n1 - Шифрование\n2 - Расшифровывание:\n3 - Выход\n\n");
    //     printf("Выберите режим:");
    //     if (scanf("%d", &mode) != 1 || (mode != 1 && mode != 2 && mode != 3)) {
    //         printf("Неверный режим\n");
    //         return 1;
    //     }

    //     switch (mode)
    //     {
    //     case 1:
    //         FILE *fin = fopen(filepath_in, "rb");
    //         if (!fin) {
    //             printf("Ошибка открытия входного файла %s\n", filepath_in);
    //             return 1;
    //         }
    //         FILE *fout_e = fopen(filepath_out_e, "wb");
    //         if (!fout_e) {
    //             printf("Ошибка открытия зашифрованного файла %s\n", filepath_out_e);
    //             fclose(fin);
    //             return 1;
    //         }

    //         unsigned char header_e[BMP_HEADER_SIZE];
    //         if (fread(header_e, 1, BMP_HEADER_SIZE, fin) != BMP_HEADER_SIZE) {
    //             printf("Ошибка чтения заголовка BMP\n");
    //             fclose(fin); fclose(fout_e);
    //             return 1;
    //         }
    //         fwrite(header_e, 1, BMP_HEADER_SIZE, fout_e);

    //         unsigned char byte_e;
    //         while (fread(&byte_e, 1, 1, fin) == 1) {
    //             printf("%d ", byte_e);
    //             long long enc = Shamir_encrypt(byte_e, p, C_a, C_b, &D_a, &D_b);
    //             unsigned char out = (unsigned char)(enc & 0xFF); // берем младший байт
    //             printf("%d\t\t", out);
    //             fwrite(&out, 1, 1, fout_e);
    //         }

    //         fclose(fin); fclose(fout_e);

    //         break;
        
    //     case 2:
    //         FILE *fin_e = fopen(filepath_out_e, "rb");
    //         if (!fin_e) {
    //             printf("Ошибка открытия зашифрованого файла %s\n", filepath_out_e);
    //             return 1;
    //         }
    //         FILE *fout_d = fopen(filepath_out_d, "wb");
    //         if (!fout_d) {
    //             printf("Ошибка открытия расшифрованного файла %s\n", filepath_out_d);
    //             fclose(fin_e);
    //             return 1;
    //         }

    //         unsigned char header_d[BMP_HEADER_SIZE];
    //         if (fread(header_d, 1, BMP_HEADER_SIZE, fin_e) != BMP_HEADER_SIZE) {
    //             printf("Ошибка чтения заголовка BMP\n");
    //             fclose(fin_e); fclose(fout_d);
    //             return 1;
    //         }
    //         fwrite(header_d, 1, BMP_HEADER_SIZE, fout_d);

    //         unsigned char byte_d;
    //         while (fread(&byte_d, 1, 1, fin_e) == 1) {
    //             printf("%d ", byte_d);
    //             long long dec = Shamir_decrypt(byte_d, p, D_a, D_b);
    //             unsigned char out = (unsigned char)(dec & 0xFF); // берем младший байт
    //             printf("%d\t\t", out);
    //             fwrite(&out, 1, 1, fout_d);
    //         }

    //         printf("Расшифровывание завершено\n");

    //         fclose(fin_e); fclose(fout_d);

    //         break;

    //     case 3:
    //         printf("Программа завершена\n");
    //         quit = 1;

    //         break;
    //     }
    // }

    return 0;
}