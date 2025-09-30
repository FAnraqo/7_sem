#include <stdio.h>
#include <stdlib.h>

#define BMP_HEADER_SIZE 54

// Быстрое возведение в степень по модулю
unsigned long long mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Расширенный алгоритм Евклида для обратного по модулю
long long extended_gcd(long long a, long long b, long long *x, long long *y) {
    if (b == 0) { *x = 1; *y = 0; return a; }
    long long x1, y1;
    long long gcd = extended_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return gcd;
}

long long mod_inverse(long long a, long long m) {
    long long x, y;
    long long g = extended_gcd(a, m, &x, &y);
    if (g != 1) return -1;
    x %= m;
    if (x < 0) x += m;
    return x;
}

unsigned long long encrypt(unsigned long long m, unsigned long long p, unsigned long long Ca) {
    return mod_exp(m, Ca, p);
}

unsigned long long decrypt_stage1(unsigned long long c1, unsigned long long p, unsigned long long Cb) {
    return mod_exp(c1, Cb, p);
}

unsigned long long decrypt_stage2(unsigned long long c2, unsigned long long p, long long Da) {
    return mod_exp(c2, Da, p);
}

// Функция для конкатенации пути и имени файла
void make_path(char *dest, const char *folder, const char *filename) {
    sprintf(dest, "%s/%s", folder, filename);
}

int main() {
    unsigned long long p, Ca, Cb;
    printf("Введите простое число p: ");
    if (scanf("%llu", &p) != 1) { printf("Ошибка ввода p\n"); return 1; }
    printf("Введите ключ шифрования Ca: ");
    if (scanf("%llu", &Ca) != 1) { printf("Ошибка ввода Ca\n"); return 1; }
    printf("Введите ключ Cb: ");
    if (scanf("%llu", &Cb) != 1) { printf("Ошибка ввода Cb\n"); return 1; }

    long long mod = (long long)(p - 1);
    long long Da = mod_inverse((long long)Ca, mod);
    long long Db = mod_inverse((long long)Cb, mod);
    if (Da == -1 || Db == -1) {
        printf("Ошибка: обратный ключ не существует для Ca или Cb\n");
        return 1;
    }
    printf("Вычисленные обратные ключи:\nDa = %lld\nDb = %lld\n", Da, Db);

    char filename_in[256];
    char filepath_in[512], filepath_out_e[16], filepath_out_d[16];
    printf("Введите имя входного BMP файла (в папке Image): ");
    if (scanf("%s", filename_in) != 1){
        printf("Ошибка ввода названия входного файла");
        return 1;
    }

    sprintf(filepath_in, "%s/%s", "image", filename_in);
    sprintf(filepath_out_e, "%s", "temp/lab4_e.bmp");
    sprintf(filepath_out_d, "%s", "temp/lab4_d.bmp");

    int mode, exit = 0;
    while (exit == 0)
    {
        printf("Меню работы:\n1 - Шифрование\n2 - Расшифровывание:\n3 - Выход\n\n");
        printf("Выберите режим:");
        if (scanf("%d", &mode) != 1 || (mode != 1 && mode != 2 && mode != 3)) {
            printf("Неверный режим\n");
            return 1;
        }

        switch (mode)
        {
        case 1:
            FILE *fin = fopen(filepath_in, "rb");
            if (!fin) {
                printf("Ошибка открытия входного файла %s\n", filepath_in);
                return 1;
            }
            FILE *fout_e = fopen(filepath_out_e, "wb");
            if (!fout_e) {
                printf("Ошибка открытия зашифрованного файла %s\n", filepath_out_e);
                fclose(fin);
                return 1;
            }

            unsigned char header_e[BMP_HEADER_SIZE];
            if (fread(header_e, 1, BMP_HEADER_SIZE, fin) != BMP_HEADER_SIZE) {
                printf("Ошибка чтения заголовка BMP\n");
                fclose(fin); fclose(fout_e);
                return 1;
            }
            fwrite(header_e, 1, BMP_HEADER_SIZE, fout_e);

            int ch_e;
            while ((ch_e = fgetc(fin)) != EOF) {
                unsigned long long c1 = encrypt((unsigned long long)(unsigned char)ch_e, p, Ca);
                fputc((unsigned char)c1, fout_e);
            }
            printf("Шифрование завершено\n");

            fclose(fin); fclose(fout_e);

            break;
        
        case 2:
            FILE *fin_e = fopen(filepath_out_e, "rb");
            if (!fin_e) {
                printf("Ошибка открытия зашифрованого файла %s\n", filepath_out_e);
                return 1;
            }
            FILE *fout_d = fopen(filepath_out_d, "wb");
            if (!fout_d) {
                printf("Ошибка открытия расшифрованного файла %s\n", filepath_out_d);
                fclose(fin_e);
                return 1;
            }

            unsigned char header_d[BMP_HEADER_SIZE];
            if (fread(header_d, 1, BMP_HEADER_SIZE, fin_e) != BMP_HEADER_SIZE) {
                printf("Ошибка чтения заголовка BMP\n");
                fclose(fin_e); fclose(fout_d);
                return 1;
            }
            fwrite(header_d, 1, BMP_HEADER_SIZE, fout_d);

            int ch_d;
            while ((ch_d = fgetc(fin_e)) != EOF) {
                unsigned long long c2 = decrypt_stage1((unsigned long long)(unsigned char)ch_d, p, Cb);
                unsigned long long m = decrypt_stage2(c2, p, Da);
                fputc((unsigned char)m, fout_d);
            }
            printf("Расшифрование завершено\n");

            fclose(fin_e); fclose(fout_d);

            break;

        case 3:
            printf("Программа завершена\n");
            exit = 1;

            break;
        }
    }

    return 0;
}
