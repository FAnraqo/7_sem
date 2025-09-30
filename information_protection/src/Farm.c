#include "../include/functions.h"

#include <stdlib.h>
#include <time.h>

int Farm(long long p, int k){
    if (p < 2) return 0;
    if (p == 2) return 1;
    if (p % 2 == 0) return 0;
    for (int i = 0; i < k; i++){
        long long a = rand() % (p - 2) + 2;
        if (Euclid(a, p) != 1 || Exponentiation(a, p-1, p) != 1){
            return 0;
        }
    }
    return 1;
}