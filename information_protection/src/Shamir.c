#include "../include/functions.h"

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

long long Shamir_encrypt(long long m, long long p,
                        long long C_a, long long C_b,
                        long long *D_a, long long *D_b){

    *D_a = mod_inverse(C_a, p - 1);
    *D_b = mod_inverse(C_b, p - 1);

    long long x_1 = Exponentiation(m, C_a, p);
    long long x_2 = Exponentiation(x_1, C_b, p);

    return x_2;
}

long long Shamir_decrypt(long long x_2, long long p,
                        long long D_a, long long D_b){
    
    long long x_3 = Exponentiation(x_2, D_a, p);
    long long x_4 = Exponentiation(x_3, D_b, p);

    return x_4;
}