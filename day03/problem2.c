#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "../util/vector.h"

typedef unsigned long ulong;

UseVec(ulong)

int get_binary_ulong(FILE *input, ulong *out);

ulong get_rating(Vec(ulong) *vec, Vec(ulong) *tmp_vec, int nbits, _Bool is_c02);

#define GET_BIT(VAL,NBITS,BIT_IDX) ((VAL) >> ((NBITS) - (BIT_IDX)) & 1)

FILE *open_file(int argc, char **argv);

int main(int argc, char **argv) {
    FILE *input = open_file(argc, argv);
    Vec(ulong) vec = vec_new_with_cap(ulong, 1000);
    ulong l;
    int bits = get_binary_ulong(input, &l);
    while(bits != 0) {
        vec_push(&vec, l);

        if(!get_binary_ulong(input, &l)) {
            break;
        }
    }
    Vec(ulong) vec_clone = vec_clone(&vec);
    Vec(ulong) tmp_vec = vec_new_with_cap(ulong, vec.cap);

    l = get_rating(&vec, &tmp_vec, bits, false);
    ulong l1 = get_rating(&vec_clone, &tmp_vec, bits, true);

    printf(
        "oxygen generator rating: %lu, co2 scrubber rating: %lu, life support rating: %lu\n",
        l,
        l1,
        l * l1
    );

    fclose(input);
}

int get_binary_ulong(FILE *input, ulong *out) {
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
    *out = strtoul(buf, &endptr, 2);

    return i;
}

ulong get_rating(Vec(ulong) *vec, Vec(ulong) *tmp_vec, int nbits, _Bool is_c02) {
    for(int i = 0;i < nbits;i++) {
        vec_clear(tmp_vec);
        int on_freq = 0;
        VEC_FOREACH(vec, ulong, cur, {
            int bit = GET_BIT(*cur, nbits, i+1);
            if(bit) {
                on_freq += 1;
            }
        });

        int off_freq = vec_len(vec) - on_freq;

        if(off_freq == on_freq) {
            VEC_FOREACH(vec, ulong, cur, {
                int bit = GET_BIT(*cur, nbits, i+1) ^ is_c02;
                if(bit) {
                    return *cur;
                }
            })
        }

        int search_for = (on_freq > off_freq) ^ is_c02;
        
        VEC_FOREACH(vec, ulong, cur, {
            int bit = GET_BIT(*cur, nbits, i+1);
            if(bit == search_for) {
                vec_push(tmp_vec, *cur);
            }
        });

        // NOTE this operation is cheap because Vector is trivially copyable,
        // but not trivially clonable. And this is just a handle copy
        Vec(ulong) vector_swap_tmp = *tmp_vec;
        *tmp_vec = *vec;
        *vec = vector_swap_tmp; 
    }
    assert(0 && "Reached unreachable code");
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
