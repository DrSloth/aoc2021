#ifndef VEC_DECL_H
#define VEC_DECL_H

#define CONCAT_(A, B) A##B
#define CONCAT(A, B)  CONCAT_(A, B)

/// A vector type storing T.
/// Writing any of the fields is undefined behavior
#define Vec(T) Vector__##T

#define vec_new_with_cap(T, cap) (vector_new_with_cap__##T(cap))
#define vec_push(self, val) ((self)->vtable->push(self, val))
#define vec_clone(self) ((self)->vtable->clone(self))
#define vec_len(self) ((self)->len)
#define vec_clear(self)  do { (self)->len = 0; } while(0);

#define Vec_vtable(T) CONCAT(Vec(T), __vtable)
#define Vec_vtable_static(T) CONCAT(static__, CONCAT(Vec_vtable(T), _impl))

/// Declare Vector data structure that can store instances of T.
/// This would generally be placed inside a Header file
#define DeclVec(T) \
    struct Vec_vtable(T); \
    typedef struct Vec(T) { \
        struct Vec_vtable(T) *vtable; \
        unsigned int cap; \
        unsigned int len; \
        T *alloc; \
    } Vec(T); \
    Vec(T) vector_new_with_cap__##T(int cap); \
    void vector_push__##T(Vec(T) *self, T val); \
    Vec(T) vector_clone__##T(Vec(T) *self); 

#define VEC_FOREACH(VEC, T, VAR, BLOCK) do { Vec(T) *__vec__ptr__ = VEC; \
    for(int __idx_i__ = 0; __idx_i__ < __vec__ptr__->len;__idx_i__++) { \
    T *VAR = &__vec__ptr__->alloc[__idx_i__]; \
    BLOCK; \
} }while(0);

/// Implement a Vector that is able to store instances of T.
/// This requires a previously used `DeclVec(T)`
#define ImplVec(T) \
struct Vec_vtable(T) {\
        void (*push)(Vec(T) *self, T val);\
        Vec(T) (*clone)(Vec(T) *self); \
}; \
static struct Vec_vtable(T) Vec_vtable_static(T) = { \
    .push = vector_push__##T,\
    .clone = vector_clone__##T \
}; \
    Vec(T) vector_new_with_cap__##T(int cap) { \
        T *alloc = malloc(cap * sizeof(*alloc)); \
        Vec(T) v = { \
            .vtable = &Vec_vtable_static(T),\
            .cap = cap, \
            .len = 0, \
            .alloc = alloc, \
        }; \
     \
        return v; \
    } \
    void vector_push__##T(Vec(T) *self, T val) { \
        unsigned int next_len = self->len + 1; \
        if(next_len > self->cap) { \
            self->cap = self->cap + self->cap / 2 + self->cap / 4 + 1; \
            self->alloc = realloc(self->alloc, self->cap * sizeof(*self->alloc)); \
            if(self->alloc == NULL) { \
                fprintf(stderr, "[ERROR] Realloc of Vector to capacity %d ran OOM", self->cap); \
                exit(1); \
            } \
        } \
     \
        self->alloc[self->len] = val; \
        self->len = next_len; \
    } \
    \
    Vec(T) vector_clone__##T(Vec(T) *self) { \
        Vec(T) clone = vec_new_with_cap(ulong, self->len); \
        clone.len = self->len; \
        int i = 0; \
        VEC_FOREACH(&clone, T, cur_mut, *cur_mut = self->alloc[i++]); \
        return clone; \
    } \

/// Include and directly declare as well as implement vector type able to store instances of T.
/// To avoid redundancy the Vector declarations and implementations should be seperated into 
/// source and header files. This is of course not necessary if you know you won't declare 
/// the same Vec two times.
#define UseVec(T) \
    DeclVec(T) \
    ImplVec(T) \

#endif //VEC_DECL_H
