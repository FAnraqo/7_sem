long long Exponentiation(long long a, long long x, long long p){
    long long result = 1 % p;
    a %= p;
    if (a < 0){
        a += p;
    }
    while (x > 0) {
        if (x & 1) {
            result = (result * a) % p;
        }
        a = (a * a) % p;
        x >>= 1;
    }
    return result;
}