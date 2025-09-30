#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/functions.h"

int main(){

    srand(time(NULL));

    long long y, a, p;

    //Функция "Шаг младенца, шаг великана"

    int menu;

    printf("Выберите режим работы алгоритма \"Шаг младенца, шаг великана\" для уравнения y = a^x mod p\n");
    printf("1 - Самостоятельный ввод\n2 - Рандомные числа\n\nРежим работы: ");
    if (scanf("%d", &menu) != 1){
        printf("Ошибка ввода\n");
    }

    switch (menu)
    {
    case 1:
        printf("Введите числа для решения уравнения y = a^x mod p\n");
        printf("Введите y, a, p: ");
        if (scanf("%lld %lld %lld", &y, &a, &p) != 3){
            printf("Ошибка ввода\n");
        }

        break;
    case 2:
        p = rand() % 100 + 1;
        while (Farm(p, 100) != 1){
            p = rand() % 100 + 1;
        }
        y = rand() % 100 + 1;
        while (y >= p){
            y = rand() % 100 + 1;
        }
        a = rand() % 100 + 1;
        printf("\nРандомные числа:\ny = %lld\na = %lld\np = %lld\n\n", y, a, p);

        break;
    }

    Baby_step_Giant_step(y, a, p);

    return 0;
}