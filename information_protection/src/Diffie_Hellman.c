#include "../include/functions.h"

void Diffie_Hellman(long long p, long long g,
                    long long private_a, long long private_b,
                    long long *public_a, long long *public_b,
                    long long *shared_a, long long *shared_b){

    *public_a = Exponentiation(g, private_a, p);
    *public_b = Exponentiation(g, private_b, p);

    *shared_a = Exponentiation(*public_b, private_a, p);
    *shared_b = Exponentiation(*public_a, private_b, p);
}