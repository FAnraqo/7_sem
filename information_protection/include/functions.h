#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/**
 * Быстрое возведение в степень по модулю
 * Вычисляет a^x mod p эффективно
 * @param a - основание
 * @param x - показатель степени
 * @param p - модуль
 * @return результат a^x mod p
 */
long long Exponentiation(long long a, long long x, long long p);

/**
 * Тест простоты Ферма
 * Проверяет число на простоту с помощью малой теоремы Ферма
 * @param p - число для проверки на простоту
 * @param k - количество итераций теста (точность)
 * @return 1 если число вероятно простое, 0 если составное
 */
int Farm(long long p, int k);

/**
 * Расширенный алгоритм Евклида
 * Находит НОД(a, b) и коэффициенты для уравнения Безу
 * @param a - первое число
 * @param b - второе число
 * @return НОД(a, b)
 */
long long Euclid(long long a, long long b);

/**
 * Алгоритм "Шаг младенца, шаг великана" для решения задачи дискретного логарифма
 * Решает уравнение y = a^x mod p
 * @param y - результат
 * @param a - основание
 * @param p - модуль (должно быть простым числом)
 */
void Baby_step_Giant_step(int y, int a, int p);

/**
 * Реализация алгоритма Диффи-Хеллмана
 * Он генерирует общие и открытые ключи для двух пользователей
 * @param p - простое число (публичный параметр)
 * @param g - основание (публичный параметр)
 * @param private_a - закрытый ключ пользователя Алисы
 * @param private_b - закрытый ключ пользователя Боба
 * @param public_a - открытый ключ пользователя Алисы
 * @param public_b - открытый ключ пользователя Боба
 * @param shared_a - общий ключ для Алисы
 * @param shared_b - общий ключ для Боба
 */
void Diffie_Hellman(long long p, long long g,
                    long long private_a, long long private_b,
                    long long *public_a, long long *public_b,
                    long long *shared_a, long long *shared_b);

long long Shamir_encrypt(long long m, long long p,
                        long long C_a, long long C_b,
                        long long *D_a, long long *D_b);

long long Shamir_decrypt(long long x_1, long long p,
                        long long D_a, long long D_b);

long long mod_inverse(long long a, long long m);

long long NOD(long long a, long long b);

int Manual_input_shamir(long long *p, long long *C_a, long long *C_b);

int Automatic_input_shamir(const char *filepath_in, long long *best_p, long long *best_Ca, long long *best_Cb);

int Encrypt_Decrypt_shamir(const char *filepath_in, long long p, long long C_a, long long C_b);

#endif