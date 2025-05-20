# Prime Number Finder

This project provides implementations of a prime number finder in **Java**, **C**, and **Python**, supporting three algorithms: **Segmented Sieve of Eratosthenes**, **Sieve of Atkin**, and **Miller-Rabin Primality Test**. Each program uses a command-line interface to select the algorithm and specify an upper limit \( n \), listing all prime numbers up to \( n \).

## Features

- **Command-Line Interface**:
  - Format: `<program> -a <algorithm> <upper_limit>`
  - Algorithms: `segmented`, `atkin`, `miller`
- **Algorithms**:
  - `segmented`: Lists primes using Segmented Sieve (fastest for large ranges).
  - `atkin`: Lists primes using Sieve of Atkin (theoretically efficient for very large \( n \)).
  - `miller`: Tests each number from 2 to \( n \) with Miller-Rabin, optimized with a pre-sieve.
- **Error Handling**: Validates inputs and provides clear error messages.
- **Cross-Language Consistency**: All implementations produce identical output for the same inputs.

## Requirements

- **Java**: JDK 11 or later (e.g., OpenJDK).
- **C**: GCC or compatible compiler (e.g., GCC 13.3.0).
- **Python**: Python 3.6 or later.
- **Operating System**: Linux, Windows, or macOS.
- **Memory**: At least 512 MB for \( n \leq 10^7 \); more for larger \( n \).

## Installation

1. **Clone or Download**:
   - Save the following files to a directory (e.g., `~/workspace/prime`):
     - `Prime.java`
     - `prime.c`
     - `prime.py`

2. **Verify Tools**:
   ```bash
   java -version
   gcc --version
   python3 --version
   ```

## Programs

### Java Program
**File**: `Prime.java`

```java
import java.util.*;

public class Prime {
    // --- Segmented Sieve of Eratosthenes ---
    private static void simpleSieve(long limit, List<Long> primes) {
        boolean[] mark = new boolean[(int)limit + 1];
        Arrays.fill(mark, true);
        
        for (long i = 2; i * i <= limit; i++) {
            if (mark[(int)i]) {
                for (long j = i * i; j <= limit; j += i) {
                    mark[(int)j] = false;
                }
            }
        }
        
        for (long i = 2; i <= limit; i++) {
            if (mark[(int)i]) {
                primes.add(i);
            }
        }
    }
    
    private static void segmentedSieveSegment(long low, long high, List<Long> primes) {
        long segmentSize = high - low + 1;
        boolean[] mark = new boolean[(int)segmentSize];
        Arrays.fill(mark, true);
        
        for (long p : primes) {
            long firstMultiple = (low / p) * p;
            if (firstMultiple < low) {
                firstMultiple += p;
            }
            for (long j = firstMultiple; j <= high; j += p) {
                mark[(int)(j - low)] = false;
            }
        }
        
        for (int i = 0; i < segmentSize; i++) {
            if (mark[i] && (low + i) >= 2) {
                System.out.println(low + i);
            }
        }
    }
    
    private static void segmentedSieve(long n) {
        if (n < 2) {
            System.out.println("No primes less than 2");
            return;
        }
        long limit = (long)Math.sqrt(n);
        List<Long> primes = new ArrayList<>();
        simpleSieve(limit, primes);
        
        long segmentSize = limit;
        long low = 2;
        long high = segmentSize;
        
        while (low <= n) {
            if (high > n) {
                high = n;
            }
            segmentedSieveSegment(low, high, primes);
            low += segmentSize;
            high += segmentSize;
        }
    }
    
    // --- Sieve of Atkin ---
    private static void sieveOfAtkin(long n) {
        if (n < 2) {
            System.out.println("No primes less than 2");
            return;
        }
        
        boolean[] isPrime = new boolean[(int)n + 1];
        long limit = (long)Math.sqrt(n);
        
        for (long x = 1; x <= limit; x++) {
            for (long y = 1; y <= limit; y++) {
                long num = 4 * x * x + y * y;
                if (num <= n && (num % 12 == 1 || num % 12 == 5)) {
                    isPrime[(int)num] = !isPrime[(int)num];
                }
                
                num = 3 * x * x + y * y;
                if (num <= n && num % 12 == 7) {
                    isPrime[(int)num] = !isPrime[(int)num];
                }
                
                if (x > y) {
                    num = 3 * x * x - y * y;
                    if (num <= n && num % 12 == 11) {
                        isPrime[(int)num] = !isPrime[(int)num];
                    }
                }
            }
        }
        
        for (long i = 5; i <= limit; i++) {
            if (isPrime[(int)i]) {
                for (long j = i * i; j <= n; j += i * i) {
                    isPrime[(int)j] = false;
                }
            }
        }
        
        if (n >= 2) System.out.println(2);
        if (n >= 3) System.out.println(3);
        for (long i = 5; i <= n; i++) {
            if (isPrime[(int)i]) {
                System.out.println(i);
            }
        }
    }
    
    // --- Miller-Rabin Primality Test with Pre-Sieve ---
    private static long mulmod(long a, long b, long mod) {
        long res = 0;
        a = a % mod;
        while (b > 0) {
            if ((b & 1) == 1) {
                res = (res + a) % mod;
            }
            a = (a * 2) % mod;
            b >>= 1;
        }
        return res % mod;
    }
    
    private static long powmod(long base, long exp, long mod) {
        long res = 1;
        base = base % mod;
        while (exp > 0) {
            if ((exp & 1) == 1) {
                res = mulmod(res, base, mod);
            }
            base = mulmod(base, base, mod);
            exp >>= 1;
        }
        return res;
    }
    
    private static boolean millerRabinTest(long n, long d, long a) {
        long x = powmod(a, d, n);
        if (x == 1 || x == n - 1) {
            return true;
        }
        
        while (d != n - 1) {
            x = mulmod(x, x, n);
            d *= 2;
            if (x == 1) {
                return false;
            }
            if (x == n - 1) {
                return true;
            }
        }
        return false;
    }
    
    private static boolean isPrimeMillerRabin(long n) {
        if (n <= 1) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0) return false;
        
        long d = n - 1;
        while (d % 2 == 0) {
            d /= 2;
        }
        
        long[] witnesses = {2, 3, 5, 7, 11, 13, 17};
        
        for (long a : witnesses) {
            if (a >= n) break;
            if (!millerRabinTest(n, d, a)) {
                return false;
            }
        }
        return true;
    }
    
    private static void millerRabin(long n) {
        if (n < 2) {
            System.out.println("No primes less than 2");
            return;
        }
        
        // Pre-sieve with small primes
        long limit = Math.min(n, (long)Math.sqrt(n) + 1);
        List<Long> smallPrimes = new ArrayList<>();
        simpleSieve(limit, smallPrimes);
        
        boolean[] isComposite = new boolean[(int)n + 1];
        isComposite[0] = isComposite[1] = true;
        for (long p : smallPrimes) {
            if (p <= n) {
                System.out.println(p); // Print small primes
            }
            for (long j = p * 2; j <= n; j += p) {
                isComposite[(int)j] = true;