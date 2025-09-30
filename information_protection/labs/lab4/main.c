#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <../include/functions.h>

int main() {
    long long p;
    long long C_a, C_b;

    srand((unsigned)time(NULL));

    char filename_in[256];
    printf("Введите имя входного BMP файла (в папке Image): ");
    if (scanf("%s", filename_in) != 1) {
        printf("Ошибка ввода имени файла\n");
        return 1;
    }

    char filepath_in[512];
    sprintf(filepath_in, "image/%s", filename_in);

    int menu;
    printf("Выберите режим ввода для шифра Шамира:\n");
    printf("1 - Самостоятельный ввод\n2 - Рандомные числа\n\nРежим работы: ");
    if (scanf("%d", &menu) != 1 || (menu != 1 && menu != 2)){
        printf("Ошибка ввода\n");
        return 1;
    }

    switch (menu) {
        case 1: {
            if (Manual_input_shamir(&p, &C_a, &C_b) != 1) return 1;

            break;
        }

        case 2: {
            if (Automatic_input_shamir(filepath_in, &p, &C_a, &C_b) != 1) {
                printf("Не удалось найти подходящие параметры автоматически.\n");
                return 1;
            }

            printf("Автоматически подобранные числа:\np = %lld\nC_a = %lld\nC_b = %lld\n", p, C_a, C_b);
            break;
        }
    }

    int mode, quit = 0;
    while (!quit) {
        printf("Меню:\n1 - Шифрование и Дешифрование\n2 - Ручной ввод\n3 - Автоматический подбор\n4 - Выход\nВыберите: ");
        if (scanf("%d", &mode) != 1 || (mode != 1 && mode != 2 && mode != 3 && mode != 4)){
            printf("Ошибка ввода\n");
            return 1;
        }

        switch (mode) {
            case 1: {
                if (Encrypt_Decrypt_shamir(filepath_in, p, C_a, C_b) != 1) return 1;

                break;
            }

            case 2: {
                if (Manual_input_shamir(&p, &C_a, &C_b) != 1) return 1;

                break;
            }

            case 3: {
                if (Automatic_input_shamir(filepath_in, &p, &C_a, &C_b) != 1) {
                    printf("Не удалось найти подходящие параметры автоматически.\n");
                    return 1;
                }

                printf("Автоматически подобранные числа:\np = %lld\nC_a = %lld\nC_b = %lld\n", p, C_a, C_b);
                break;
            }
            
            case 4: {
                quit = 1; break;
            }
        }
    }

    

    return 0;
}

