/* -*- mode: c -*-
 * $Id$
 * http://www.bagley.org/~doug/shootout/
 */

int atoi(char *);
#include <stdio.h>

#define CACHE_SIZE 100

unsigned long fib(unsigned long n) {
    struct CacheEntry {
        unsigned long key;
        unsigned long value;
        unsigned long lru_ctr;
        int valid;
    };
    static struct CacheEntry cache[CACHE_SIZE] = {0};
    static unsigned long counter = 1;

    for (int i = 0; i < CACHE_SIZE; ++i) {
        if (cache[i].valid && cache[i].key == n) {
            cache[i].lru_ctr = counter++;
            return cache[i].value;
        }
    }

    unsigned long result;
    if (n < 2) {
        result = 1;
    } else {
        result = fib(n - 1) + fib(n - 2);
    }

    int slot = -1;
    unsigned long min_ctr = counter;
    for (int i = 0; i < CACHE_SIZE; ++i) {
        if (!cache[i].valid) {
            slot = i;
            break;
        } else if (cache[i].lru_ctr < min_ctr) {
            min_ctr = cache[i].lru_ctr;
            slot = i;
        }
    }

    if (slot != -1) {
        cache[slot].key = n;
        cache[slot].value = result;
        cache[slot].lru_ctr = counter++;
        cache[slot].valid = 1;
    }

    return result;
}

int main(int argc, char *argv[]) {
#ifdef SMALL_PROBLEM_SIZE
#define LENGTH 40
#else
#define LENGTH 43
#endif
    int N = ((argc == 2) ? atoi(argv[1]) : LENGTH);
    printf("%ld\n", fib(N));
    return 0;
}
