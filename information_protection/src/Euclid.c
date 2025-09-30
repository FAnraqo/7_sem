long long Euclid(long long a, long long b){
    long long U[3] = {a, 1, 0};
    long long V[3] = {b, 0, 1};

    while (V[0] != 0)
    {
        long long q = U[0] / V[0];
        long long T[3] = {U[0] % V[0], U[1] - q * V[1], U[2] - q * V[2]};
        for (int i = 0; i < 3; i++)
            U[i] = V[i];
        for (int i = 0; i < 3; i++)
            V[i] = T[i];
    }

    return U[0];
}