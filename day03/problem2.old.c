#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Vector {
    unsigned int cap;
    unsigned int len;
    unsigned long *alloc;
} Vector;

#define FOREACH(VEC,VAR,BLOCK) do { Vector *__vec__ptr__ = VEC; \
    for(int __idx_i__ = 0; __idx_i__ < __vec__ptr__->len;__idx_i__++) { \
    unsigned long *VAR = &__vec__ptr__->alloc[__idx_i__]; \
    BLOCK; \
} }while(0);

Vector vector_new_with_cap(int cap);
void vector_push(Vector *self, unsigned long val);
Vector vector_clone(Vector *self);
void vector_clear(Vector *self);

int get_binary_ulong(FILE *input, unsigned long *out);

unsigned long get_rating(Vector *vec, Vector *tmp_vec, int nbits, int is_oxygen);

#define GET_BIT(VAL,NBITS,BIT_IDX) ((VAL) >> ((NBITS) - (BIT_IDX)) & 1)

FILE *open_file(int argc, char **argv);

int main(int argc, char **argv) {
    FILE *input = open_file(argc, argv);
    Vector vec = vector_new_with_cap(1000);
    unsigned long l;
    int bits = get_binary_ulong(input, &l);
    while(bits != 0) {
        vector_push(&vec, l);

        if(!get_binary_ulong(input, &l)) {
            break;
        }
    }
    Vector vec_clone = vector_clone(&vec);
    Vector tmp_vec = vector_new_with_cap(vec.cap);

    l = get_rating(&vec, &tmp_vec, bits, 0);
    unsigned long l1 = get_rating(&vec_clone, &tmp_vec, bits, 1);

    printf(
        "oxygen generator rating: %lu, co2 scrubber rating: %lu, life support rating: %lu\n",
        l,
        l1,
        l * l1
    );

    fclose(input);
}

Vector vector_new_with_cap(int cap) {
    unsigned long *alloc = malloc(cap * sizeof(*alloc));
    Vector v = {
        .cap = cap,
        .len = 0,
        .alloc = alloc,
    };

    return v;
}

void vector_push(Vector *self, unsigned long val) {
    unsigned int next_len = self->len + 1;
    if(next_len > self->cap) {
        self->cap = self->cap + self->cap / 2 + self->cap / 4;
        self->alloc = realloc(self->alloc, self->cap * sizeof(*self->alloc));
        if(self->alloc == NULL) {
            fprintf(stderr, "[ERROR] Realloc of Vector to capacity %d ran OOM", self->cap);
            puts("realloc");
            exit(1);
        }
    }

    self->alloc[self->len] = val;
    self->len = next_len;
}

Vector vector_clone(Vector *self) {
    Vector clone = vector_new_with_cap(self->len);
    clone.len = self->len;
    int i = 0;
    FOREACH(&clone, cur_mut, *cur_mut = self->alloc[i++]);
    return clone;
}

void vector_clear(Vector *self) {
    self->len = 0;
}

int get_binary_ulong(FILE *input, unsigned long *out) {
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

unsigned long get_rating(Vector *vec, Vector *tmp_vec, int nbits, int is_oxygen) {
    for(int i = 0;i < nbits;i++) {
        vector_clear(tmp_vec);
        int on_freq = 0;
        FOREACH(vec, cur, {
            int bit = GET_BIT(*cur, nbits, i+1);
            if(bit) {
                on_freq += 1;
            }
        });

        int off_freq = vec->len - on_freq;

        printf("bit: %d on_freq: %d\n", i, on_freq);

        if(off_freq == on_freq) {
            FOREACH(vec, cur, {
                int bit = GET_BIT(*cur, nbits, i+1) ^ is_oxygen;
                if(bit) {
                    return *cur;
                }
            })
        }

        int search_for = (on_freq > off_freq) ^ is_oxygen;
        
        FOREACH(vec, cur, {
            int bit = GET_BIT(*cur, nbits, i+1);
            if(bit == search_for) {
                vector_push(tmp_vec, *cur);
            }
        });

        // NOTE this operation is cheap because Vector is trivially copyable,
        // but not trivially clonable. And this is just a handle copy
        Vector vector_swap_tmp = *tmp_vec;
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
