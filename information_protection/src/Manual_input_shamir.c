#include <stdio.h>
#include <../include/functions.h>

int Manual_input_shamir(long long *p, long long *C_a, long long *C_b) {

    printf("Режим самостоятельного ввода параметров шифра Шамира\n");
    
    do
    {
        printf("Введите p (простое число): ");
        if (scanf("%lld", p) != 1) {printf("Ошибка ввода\n"); return 0;}
        if (Farm(*p, 100) != 1 && *p <= 255){
            printf("p не простое число или p слишком маленькое, используйте p > 255\n");
        }
    } while (Farm(*p, 100) != 1 && *p <= 255);
    
    do {
        printf("Введите C_a: ");
        if (scanf("%lld", C_a) != 1){
            printf("Ошибка ввода\n");
            return 0;
        }
        if (*C_a <= 1 || *C_a >= *p - 1) {
            printf("C_a должно быть в диапазоне (1, p-1)\n");
        }
    } while (NOD(*C_a, *p - 1) != 1);

    do {
        printf("Введите C_b: ");
        if (scanf("%lld", C_b) != 1){
            printf("Ошибка ввода\n");
            return 0;
        }
        if (*C_b <= 1 || *C_b >= *p - 1) {
            printf("C_b должно быть в диапазоне (1, p-1)\n");
        }
    } while (NOD(*C_b, *p - 1) != 1);

    return 1;
}