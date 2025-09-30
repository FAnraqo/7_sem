#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/functions.h"

int main(){

    srand(time(NULL));

    long long a, x, p;

    //Функция быстрого возведения числа в степень по модулю
    printf("Введите числа для решения уравнения y = a^x mod p\n");
    printf("Введите a, x, p: ");
    if(scanf("%lld %lld %lld", &a, &x, &p) != 3){
        printf("Ошибка ввода\n");
    }

    printf("y = %lld\n\n", Exponentiation(a, x, p));

    //Функция, реализующая тест простоты Ферма
    printf("Введите число для определения является ли оно простым\n");
    printf("Введите число: ");
    if(scanf("%lld", &p) != 1){
        printf("Ошибка ввода\n");
    }

    if (Farm(p, 100) == 1)
        printf("Простое число\n\n");
    else
        printf("Не является простым числом\n\n");

    //Функция, реализующая обобщённый алгоритм Евклида
    int menu;

    printf("Выберите режим работы обобщённого алгоритма Евклида для уравнения a*x + b*y = gcd(a, b)\n");
    printf("1 - Самостоятельный ввод\n2 - Рандомные числа\n3 - Простые числа\n\nРежим работы: ");
    if (scanf("%d", &menu) != 1){
        printf("Ошибка ввода\n");
    }

    long long a_E, b_E;

    switch (menu)
    {
    case 1:
        printf("\nВведите a и b: ");
        if (scanf("%lld %lld", &a_E, &b_E) != 2){
            printf("Ошибка ввода\n");
        }

        break;
    case 2:
        a_E = rand() % 1000 + 1;
        b_E = rand() % 1000 + 1;
        printf("\nРандомные числа:\na = %lld\nb = %lld\n\n", a_E, b_E);

        break;

    case 3:
        a_E = rand() % 100 + 1;
        while (Farm(a_E, 100) != 1){
            a_E = rand() % 100 + 1;
        }
        b_E = rand() % 100 + 1;
        while (Farm(b_E, 100) != 1 || b_E == a_E){
            b_E = rand() % 100 + 1;
        }

        printf("\nРандомные простые числа:\na = %lld\nb = %lld\n\n", a_E, b_E);

        break;
    }

    printf("НОД(%lld, %lld) = %lld\n", a_E, b_E, Euclid(a_E, b_E));

    return 0;
}