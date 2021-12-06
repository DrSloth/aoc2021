#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

int get_binary_i64(FILE *input, uint64_t *out);
uint64_t bitmask(int used_bits);
FILE *open_file(int argc, char **argv);

int main(int argc, char **argv) {
    FILE *input = open_file(argc, argv);
    uint64_t l = 0;
    int num_lines = 0;
    int bits = get_binary_i64(input, &l);
    uint64_t mask = bitmask(bits);
    int bit_freqs[bits];
    memset(bit_freqs, 0, bits * sizeof(*bit_freqs));
    while(1) {
        num_lines += 1;
        for(int i = 0;i < bits;i++) {
            int bit = ((l >> i) & 1);
            if(bit == 1) {
                bit_freqs[i] += 1;
            }
        }
        if(get_binary_i64(input, &l) == 0) {
            break;
        }
    }

    uint64_t gamma_rate = 0;
    for(int i = 0;i < bits;i++) {
        if(bit_freqs[i] > num_lines/2) {
            gamma_rate |= 1UL << i;
        }
    } 

    uint64_t epsilon_rate = ~gamma_rate & mask;
    printf(
        "gamma rate: %lu epsilon rate: %lu, power_consumption: %lu", 
        gamma_rate,
        epsilon_rate,
        gamma_rate * epsilon_rate
    );
    fclose(input);
}

int get_binary_i64(FILE *input, uint64_t *out) {
    char buf[64];
    int i = 0;

    while(!feof(input)) {
        char c = fgetc(input);
        if(c == '\n') {
            break;
        }

        buf[i] = c;
        i += 1;
    }

    buf[i] = '\0';

    if(i == 0) {
        return 0;
    }

    char *endptr; 
    *out = strtol(buf, &endptr, 2);

    return i;
}

uint64_t bitmask(int used_bits) {
    uint64_t bitmask = 1;
    for(int i=0;i<used_bits;i++) {
        bitmask |= 1 << i;
    }

    return bitmask;
}

FILE *open_file(int argc, char **argv) {
    if(argc < 2) {
        return stdin;
    } else {
        FILE *f = fopen(argv[1], "r");
        if(f == NULL) {
            fprintf(stderr, "[ERROR] File not found: %s", argv[1]);
            exit(1);
        }

        return f;
    }
}

