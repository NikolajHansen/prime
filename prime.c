#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

// --- Segmented Sieve of Eratosthenes ---
void simple_sieve(long long limit, long long *primes, long long *prime_count) {
    bool *mark = (bool *)calloc(limit + 1, sizeof(bool));
    for (long long i = 0; i <= limit; i++) mark[i] = true;
    
    for (long long i = 2; i * i <= limit; i++) {
        if (mark[i]) {
            for (long long j = i * i; j <= limit; j += i) {
                mark[j] = false;
            }
        }
    }
    
    *prime_count = 0;
    for (long long i = 2; i <= limit; i++) {
        if (mark[i]) {
            primes[(*prime_count)++] = i;
        }
    }
    free(mark);
}

void segmented_sieve_segment(long long low, long long high, long long *primes, long long prime_count) {
    long long segment_size = high - low + 1;
    bool *mark = (bool *)calloc(segment_size, sizeof(bool));
    for (long long i = 0; i < segment_size; i++) mark[i] = true;
    
    for (long long i = 0; i < prime_count; i++) {
        long long p = primes[i];
        long long first_multiple = (low / p) * p;
        if (first_multiple < low) first_multiple += p;
        for (long long j = first_multiple; j <= high; j += p) {
            mark[j - low] = false;
        }
    }
    
    for (long long i = 0; i < segment_size; i++) {
        if (mark[i] && (low + i) >= 2) {
            printf("%lld\n", low + i);
        }
    }
    free(mark);
}

void segmented_sieve(long long n) {
    if (n < 2) {
        printf("No primes less than 2\n");
        return;
    }
    long long limit = (long long)sqrt(n);
    long long *primes = (long long *)malloc((limit + 1) * sizeof(long long));
    long long prime_count = 0;
    
    simple_sieve(limit, primes, &prime_count);
    
    long long segment_size = limit;
    long long low = 2;
    long long high = segment_size;
    
    while (low <= n) {
        if (high > n) high = n;
        segmented_sieve_segment(low, high, primes, prime_count);
        low += segment_size;
        high += segment_size;
    }
    
    free(primes);
}

// --- Sieve of Atkin ---
void sieve_of_atkin(long long n) {
    if (n < 2) {
        printf("No primes less than 2\n");
        return;
    }
    
    bool *is_prime = (bool *)calloc(n + 1, sizeof(bool));
    long long limit = (long long)sqrt(n);
    
    for (long long x = 1; x <= limit; x++) {
        for (long long y = 1; y <= limit; y++) {
            long long num;
            num = 4 * x * x + y * y;
            if (num <= n && (num % 12 == 1 || num % 12 == 5)) {
                is_prime[num] = !is_prime[num];
            }
            num = 3 * x * x + y * y;
            if (num <= n && num % 12 == 7) {
                is_prime[num] = !is_prime[num];
            }
            if (x > y) {
                num = 3 * x * x - y * y;
                if (num <= n && num % 12 == 11) {
                    is_prime[num] = !is_prime[num];
                }
            }
        }
    }
    
    for (long long i = 5; i <= limit; i++) {
        if (is_prime[i]) {
            for (long long j = i * i; j <= n; j += i * i) {
                is_prime[j] = false;
            }
        }
    }
    
    if (n >= 2) printf("2\n");
    if (n >= 3) printf("3\n");
    for (long long i = 5; i <= n; i++) {
        if (is_prime[i]) {
            printf("%lld\n", i);
        }
    }
    free(is_prime);
}

// --- Miller-Rabin Primality Test ---
long long mulmod(long long a, long long b, long long mod) {
    long long res = 0;
    a = a % mod;
    while (b > 0) {
        if (b % 2 == 1) {
            res = (res + a) % mod;
        }
        a = (a * 2) % mod;
        b /= 2;
    }
    return res % mod;
}

long long powmod(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            res = mulmod(res, base, mod);
        }
        base = mulmod(base, base, mod);
        exp /= 2;
    }
    return res;
}

bool miller_rabin_test(long long n, long long d, long long a) {
    long long x = powmod(a, d, n);
    if (x == 1 || x == n - 1) return true;
    
    while (d != n - 1) {
        x = mulmod(x, x, n);
        d *= 2;
        if (x == 1) return false;
        if (x == n - 1) return true;
    }
    return false;
}

bool is_prime_miller_rabin(long long n) {
    if (n <= 1) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;
    
    long long d = n - 1;
    while (d % 2 == 0) {
        d /= 2;
    }
    
    long long witnesses[] = {2, 3, 5, 7, 11, 13, 17};
    int num_witnesses = 7;
    
    for (int i = 0; i < num_witnesses && witnesses[i] < n; i++) {
        if (!miller_rabin_test(n, d, witnesses[i])) {
            return false;
        }
    }
    return true;
}

void miller_rabin(long long n) {
    if (n < 2) {
        printf("No primes less than 2\n");
        return;
    }
    printf("Prime numbers up to %lld are:\n", n);
    for (long long i = 2; i <= n; i++) {
        if (is_prime_miller_rabin(i)) {
            printf("%lld\n", i);
        }
    }
}

// --- Main Function ---
int main(int argc, char *argv[]) {
    if (argc != 4 || strcmp(argv[1], "-a") != 0) {
        fprintf(stderr, "Usage: %s -a <algorithm> <upper_limit>\n", argv[0]);
        fprintf(stderr, "Algorithms: segmented, atkin, miller\n");
        fprintf(stderr, "Example: %s -a segmented 1000000\n", argv[0]);
        return 1;
    }

    char *algo = argv[2];
    if (strcmp(algo, "segmented") != 0 && strcmp(algo, "atkin") != 0 && strcmp(algo, "miller") != 0) {
        fprintf(stderr, "Error: Invalid algorithm. Use 'segmented', 'atkin', or 'miller'\n");
        return 1;
    }

    char *endptr;
    long long n = strtoll(argv[3], &endptr, 10);
    if (*endptr != '\0' || n < 0) {
        fprintf(stderr, "Error: Please provide a valid non-negative number\n");
        return 1;
    }

    printf("Running %s algorithm for n = %lld\n", algo, n);
    if (strcmp(algo, "segmented") == 0) {
        printf("Prime numbers up to %lld are:\n", n);
        segmented_sieve(n);
    } else if (strcmp(algo, "atkin") == 0) {
        printf("Prime numbers up to %lld are:\n", n);
        sieve_of_atkin(n);
    } else if (strcmp(algo, "miller") == 0) {
        miller_rabin(n);
    }
    
    return 0;
}