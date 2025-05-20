# Prime Number Finder

This project provides implementations of a prime number finder in **Java**, **C**, and **Python**, supporting three algorithms: **Segmented Sieve of Eratosthenes**, **Sieve of Atkin**, and **Miller-Rabin Primality Test**. Each program uses a command-line interface to select the algorithm and specify an upper limit \( n \), listing all prime numbers up to \( n \).

The purpose is to run these with a time measuring tool in order to compare single thread speed in the three languages solving a well known CPU intensive problem.

```bash
	 time ./prime -a atkin 10
	 time python3 prime.py -a atkin 10
	 time java Prime -a atkin 10
   ```

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

