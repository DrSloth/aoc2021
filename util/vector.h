#ifndef VEC_DECL_H
#define VEC_DECL_H

#define Vec(T) Vector__##T

#define DeclVec(T) typedef struct Vec(T) { \
        unsigned int cap; \
        unsigned int len; \
        T *alloc; \
    } Vec(T); \
    Vec(T) vector_new_with_cap__##T(int cap); \
    void vector_push__##T(Vec(T) *self, T val); \
    Vec(T) vector_clone__##T(Vec(T) *self); \
    void vector_clear__##T(Vec(T) *self);

#define VEC_FOREACH(VEC, T, VAR, BLOCK) do { Vec(T) *__vec__ptr__ = VEC; \
    for(int __idx_i__ = 0; __idx_i__ < __vec__ptr__->len;__idx_i__++) { \
    T *VAR = &__vec__ptr__->alloc[__idx_i__]; \
    BLOCK; \
} }while(0);


#define ImplVec(T) Vec(T) vector_new_with_cap__##T(int cap) { \
    T *alloc = malloc(cap * sizeof(*alloc)); \
    Vec(T) v = { \
        .cap = cap, \
        .len = 0, \
        .alloc = alloc, \
    }; \
 \
    return v; \
} \
\
void vector_push__##T(Vec(T) *self, T val) { \
    unsigned int next_len = self->len + 1; \
    if(next_len > self->cap) { \
        self->cap = self->cap + self->cap / 2 + self->cap / 4; \
        self->alloc = realloc(self->alloc, self->cap * sizeof(*self->alloc)); \
        if(self->alloc == NULL) { \
            fprintf(stderr, "[ERROR] Realloc of Vector to capacity %d ran OOM", self->cap); \
            puts("realloc"); \
            exit(1); \
        } \
    } \
 \
    self->alloc[self->len] = val; \
    self->len = next_len; \
} \
\
Vec(T) vector_clone__##T(Vec(T) *self) { \
    Vec(T) clone = vector_new_with_cap(ulong, self->len); \
    clone.len = self->len; \
    int i = 0; \
    VEC_FOREACH(&clone, T, cur_mut, *cur_mut = self->alloc[i++]); \
    return clone; \
} \
\
void vector_clear__##T(Vec(T) *self) { \
    self->len = 0; \
} \

#define vector_new_with_cap(T, cap) (vector_new_with_cap__##T(cap))
#define vector_push(T, self, val) (vector_push__##T(self, val))
#define vector_clone(T, self) (vector_clone__##T(self))
#define vector_clear(T, self) (vector_clear__##T(self))

#endif //VEC_DECL_H
