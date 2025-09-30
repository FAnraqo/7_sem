#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "../include/functions.h"

int main(){

    srand(time(NULL));

    int menu;
    printf("Выберите режим работы функции Диффи-Хеллмана:\n");
    printf("1 - Самостоятельный ввод\n2 - Рандомные числа\n\nРежим работы: ");
    if (scanf("%d", &menu) != 1){
        printf("Ошибка ввода\n");
    }

    long long p = 23;       // публичный параметр (простое число)   
    long long g = 9;        // публичный параметр (первообразный корень)
    long long private_a = 4, private_b = 3;
    long long public_a, public_b, shared_a, shared_b;

    switch (menu)
    {
    case 1:
        printf("Введите числа для функции Диффи-Хеллмана\n");
        printf("Введите p (публичный параметр (простое число)): ");
        if (scanf("%lld", &p) != 1){
            printf("Ошибка ввода\n");
        }
        printf("Введите g (публичный параметр (первообразный корень)): ");
        if (scanf("%lld", &g) != 1){
            printf("Ошибка ввода\n");
        }
        printf("Введите закрытый ключ Алисы: ");
        if (scanf("%lld", &private_a) != 1){
            printf("Ошибка ввода\n");
        }
        printf("Введите закрытый ключ Боба: ");
        if (scanf("%lld", &private_b) != 1){
            printf("Ошибка ввода\n");
        }

        break;
    case 2:
        p = rand() % 1000000 + 10000000;
        while (Farm(p, 100) != 1){
            p = rand() % 1000000 + 10000000;
        }

        int flag = 0;
        for (g = 2; g < p && flag == 0; g++) {
            flag = 1;
            for (long long k = 1; k < p - 1; k++) {
                if (Exponentiation(g, k, p) == 1) {
                    flag = 0; 
                    break;
                }
            }
        }

        private_a = 2 + rand() % (p - 2);
        private_b = 2 + rand() % (p - 2);

        printf("\nРандомные числа:\np = %lld\ng = %lld\nЗакрытый ключ Алисы = %lld\nЗакрытый ключ Боба = %lld\n\n", p, g, private_a, private_b);

        break;
    }

    Diffie_Hellman(p, g, private_a, private_b,
                   &public_a, &public_b, &shared_a, &shared_b);

    printf("Открытый ключ Алисы: %lld\n", public_a);
    printf("Открытый ключ Боба: %lld\n", public_b);
    printf("Общий ключ для Алисы: %lld\n", shared_a);
    printf("Общий ключ для Боба: %lld\n", shared_b);

    return 0;
}