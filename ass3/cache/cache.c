/* 216303990 Arbel Feldman */

#include <stdlib.h>
#include <stdio.h>

#include "cache.h"

void print_cache(cache_t cache) {
    int S = 1 << cache.s;
    int B = 1 << cache.b;

    for (int i = 0; i < S; i++) {
        printf("Set %d\n", i);
        for (int j = 0; j < cache.E; j++) {
            printf("%1d %d 0x%0*lx ", cache.cache[i][j].valid,
                    cache.cache[i][j].frequency, cache.t, cache.cache[i][j].tag);
            for (int k = 0; k < B; k++) {
                printf("%02x ", cache.cache[i][j].block[k]);
            }
            puts("");
        }
    }
}

cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E) {
    int S = 1 << s;
    int B = 1 << b;
    // All sets:
    cache_t cache = {s, t, b, E, malloc(S * sizeof(cache_line_t*))};

    int i, j;
    // All lines:
    for (i = 0; i < S; i++) {
        cache.cache[i] = malloc(E * sizeof(cache_line_t));
    }
    // Each line:
    for (i = 0; i < S; i++) {
        for (j = 0; j < E; j++) {
            cache.cache[i][j].valid = 0;
            cache.cache[i][j].frequency = 0;
            cache.cache[i][j].tag = 0;
            cache.cache[i][j].block = calloc(B, sizeof(uchar));
        }
    }

    return cache;
}

uchar read_byte(cache_t cache, uchar* start, long int off) {
    uchar b = cache.b;
    uchar s = cache.s;
    uchar E = cache.E;
    int B = 1 << b;
    int S = 1 << s;

    long int tag = off >> (s + b);
    uchar set = (off >> b) % S;
    // Check for hit:
    for (int i = 0; i < E; i++) {
        if (cache.cache[set][i].valid == 1 && cache.cache[set][i].tag == tag) {
            cache.cache[set][i].frequency += 1;
            return start[off];
        }
    }
    // Miss:
    uchar minFreq = cache.cache[set][0].frequency;
    uchar freq;
    uchar minEntry = 0;
    // Check for minimal frequency:
    for (int i = 1; i < E; i++) {
        freq = cache.cache[set][i].frequency;
        if (freq < minFreq) {
            minFreq = freq;
            minEntry = i;
        }
    }
    // put block in line with min freq:
    cache.cache[set][minEntry].valid = 1;
    cache.cache[set][minEntry].frequency = 1;
    cache.cache[set][minEntry].tag = tag;
    for (int i = 0; i < B; i++) {
        cache.cache[set][minEntry].block[i] = start[off - (off % B) + i];
    }
    return start[off];
}

void write_byte(cache_t cache, uchar* start, long int off, uchar write) {
    start[off] = write;

    uchar b = cache.b;
    uchar s = cache.s;
    uchar E = cache.E;
    int S = 1 << s;
    long int tag = off >> (s + b);
    uchar set = (off >> b) % S;
    // Check if present in cache:
    for (int i = 0; i < E; i++) {
        if (cache.cache[set][i].valid == 1 && cache.cache[set][i].tag == tag) {
            read_byte(cache, start, off);
            return;
        }
    }
}

int main() {
    int n;
    printf("Size of data: ");
    scanf("%d", &n);
    uchar* mem = malloc(n);
    printf("Input data >> ");
    for (int i = 0; i < n; i++)
        scanf("%hhd", mem + i);

    int s, t, b, E;
    printf("s t b E: ");
    scanf("%d %d %d %d", &s, &t, &b, &E);
    cache_t cache = initialize_cache(s, t, b, E);

    while (1) {
        scanf("%d", &n);
        if (n < 0) break;
        read_byte(cache, mem, n);
    }

    puts("");
    print_cache(cache);

    free(mem);
}
