#include <math.h>
#include <stdio.h>

#include "../include/functions.h"

void Baby_step_Giant_step(int y, int a, int p){
    if (y < p && Farm(p, 100)){
            
        int k = (int)ceil(sqrt(p));

        int A[k][2], B[k][2];

        for (int i = 0; i < k; i++){
            A[i][0] = (y * (int)pow(a, i)) % p;
            A[i][1] = i;
            B[i][0] = (int)pow(a, (i + 1) * k) % p;
            B[i][1] = i + 1;
        }

        for (int i = 0; i < k - 1; i++){
            for (int j = 1; j < k; j++){
                if (A[j - 1][0] > A[j][0]){
                    int temp_0 = A[j - 1][0];
                    int temp_1 = A[j - 1][1];
                    A[j - 1][0] = A[j][0];
                    A[j - 1][1] = A[j][1];
                    A[j][0] = temp_0;
                    A[j][1] = temp_1;
                }
                if (B[j - 1][0] > B[j][0]){
                    int temp_0 = B[j - 1][0];
                    int temp_1 = B[j - 1][1];
                    B[j - 1][0] = B[j][0];
                    B[j - 1][1] = B[j][1];
                    B[j][0] = temp_0;
                    B[j][1] = temp_1;
                }
            }
        }

        for(int i = 0; i < k; i++){
            printf("%d\t", A[i][0]);
        }
        printf("\n");
        for(int i = 0; i < k; i++){
            printf("%d\t", A[i][1]);
        }
        printf("\n");
        for(int i = 0; i < k; i++){
            printf("%d\t", B[i][0]);
        }
        printf("\n");
        for(int i = 0; i < k; i++){
            printf("%d\t", B[i][1]);
        }
        printf("\n");

        printf("\nx = ");

        int i = 0, j = 0, flag = 0;
        while(i < k && j < k){
            //printf("%d %d\n", i, j);
            if (A[i][0] == B[j][0]){
                flag = 1;
                printf("%d ", B[j][1] * k - A[i][1]);
                if (A[i][0] == A[i + 1][0]){
                    i ++;
                }
                else{
                    j ++;
                }
            }
            else{
                if (A[i][0] > B[j][0]){
                    j ++;
                }
                else{
                    i ++;
                }
            }
        }
        
        if (flag == 0){
            printf("Решения нету\n");
        }

    }
    else{
        printf("Ошибка y >= p или p не простое число\n");
    }
}