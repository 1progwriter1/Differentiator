#ifndef VECTOR_CPU
#define VECTOR_CPU

#include "../data/diff_values.h"

struct Variable {
    char name;
    double value;
};

typedef Variable Vec_t;

struct Vector {
    Vec_t *data;
    size_t size;
    size_t capacity;
};

int VectorCtor(Vector *vec, size_t size);

int VectorDtor(Vector *vec);

int PushBack(Vector *vec, Vec_t num);

int Pop(Vector *vec, Vec_t *num);

int VarsVerify(Vector *vec);

#endif