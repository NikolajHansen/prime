import sys
import math
from typing import List

# --- Segmented Sieve of Eratosthenes ---
def simple_sieve(limit: int, primes: List[int]) -> None:
    mark = [True] * (limit + 1)
    for i in range(2, int(math.sqrt(limit)) + 1):
        if mark[i]:
            for j in range(i * i, limit + 1, i):
                mark[j] = False
    for i in range(2, limit + 1):
        if mark[i]:
            primes.append(i)

def segmented_sieve_segment(low: int, high: int, primes: List[int]) -> None:
    segment_size = high - low + 1
    mark = [True] * segment_size
    
    for p in primes:
        first_multiple = (low // p) * p
        if first_multiple < low:
            first_multiple += p
        for j in range(first_multiple, high + 1, p):
            mark[j - low] = False
    
    for i in range(segment_size):
        if mark[i] and (low + i) >= 2:
            print(low + i)

def segmented_sieve(n: int) -> None:
    if n < 2:
        print("No primes less than 2")
        return
    limit = int(math.sqrt(n))
    primes = []
    simple_sieve(limit, primes)
    
    segment_size = limit
    low = 2
    high = segment_size
    
    while low <= n:
        if high > n:
            high = n
        segmented_sieve_segment(low, high, primes)
        low += segment_size
        high += segment_size

# --- Sieve of Atkin ---
def sieve_of_atkin(n: int) -> None:
    if n < 2:
        print("No primes less than 2")
        return
    
    is_prime = [False] * (n + 1)
    limit = int(math.sqrt(n))
    
    for x in range(1, limit + 1):
        for y in range(1, limit + 1):
            num = 4 * x * x + y * y
            if num <= n and (num % 12 == 1 or num % 12 == 5):
                is_prime[num] = not is_prime[num]
            
            num = 3 * x * x + y * y
            if num <= n and num % 12 == 7:
                is_prime[num] = not is_prime[num]
            
            if x > y:
                num = 3 * x * x - y * y
                if num <= n and num % 12 == 11:
                    is_prime[num] = not is_prime[num]
    
    for i in range(5, limit + 1):
        if is_prime[i]:
            for j in range(i * i, n + 1, i * i):
                is_prime[j] = False
    
    if n >= 2:
        print(2)
    if n >= 3:
        print(3)
    for i in range(5, n + 1):
        if is_prime[i]:
            print(i)

# --- Miller-Rabin Primality Test ---
def mulmod(a: int, b: int, mod: int) -> int:
    res = 0
    a = a % mod
    while b > 0:
        if b % 2 == 1:
            res = (res + a) % mod
        a = (a * 2) % mod
        b //= 2
    return res % mod

def powmod(base: int, exp: int, mod: int) -> int:
    res = 1
    base = base % mod
    while exp > 0:
        if exp % 2 == 1:
            res = mulmod(res, base, mod)
        base = mulmod(base, base, mod)
        exp //= 2
    return res

def miller_rabin_test(n: int, d: int, a: int) -> bool:
    x = powmod(a, d, n)
    if x == 1 or x == n - 1:
        return True
    
    while d != n - 1:
        x = mulmod(x, x, n)
        d *= 2
        if x == 1:
            return False
        if x == n - 1:
            return True
    return False

def is_prime_miller_rabin(n: int) -> bool:
    if n <= 1:
        return False
    if n == 2 or n == 3:
        return True
    if n % 2 == 0:
        return False
    
    d = n - 1
    while d % 2 == 0:
        d //= 2
    
    witnesses = [2, 3, 5, 7, 11, 13, 17]
    
    for a in witnesses:
        if a >= n:
            break
        if not miller_rabin_test(n, d, a):
            return False
    return True

def miller_rabin(n: int) -> None:
    if n < 2:
        print("No primes less than 2")
        return
    print(f"Prime numbers up to {n} are:")
    for i in range(2, n + 1):
        if is_prime_miller_rabin(i):
            print(i)

# --- Main Function ---
def main() -> None:
    if len(sys.argv) != 4 or sys.argv[1] != "-a":
        print(f"Usage: {sys.argv[0]} -a <algorithm> <upper_limit>", file=sys.stderr)
        print("Algorithms: segmented, atkin, miller", file=sys.stderr)
        print(f"Example: {sys.argv[0]} -a segmented 1000000", file=sys.stderr)
        sys.exit(1)
    
    algo = sys.argv[2]
    if algo not in ["segmented", "atkin", "miller"]:
        print("Error: Invalid algorithm. Use 'segmented', 'atkin', or 'miller'", file=sys.stderr)
        sys.exit(1)
    
    try:
        n = int(sys.argv[3])
        if n < 0:
            raise ValueError
    except ValueError:
        print("Error: Please provide a valid non-negative number", file=sys.stderr)
        sys.exit(1)
    
    print(f"Running {algo} algorithm for n = {n}")
    if algo == "segmented":
        print(f"Prime numbers up to {n} are:")
        segmented_sieve(n)
    elif algo == "atkin":
        sieve_of_atkin(n)
    elif algo == "miller":
        miller_rabin(n)

if __name__ == "__main__":
    main()