#ifndef STR_BUFFER_TREE
#define STR_BUFFER_TREE

#include <stdio.h>
#include "diff_values.h"

typedef NodeValue* Buf_t;

struct ValueBuffer {
    Buf_t *data;
    size_t size;
    size_t capacity;
};

int ValueBufVerify(ValueBuffer *buf);

int ValueBufCtor(ValueBuffer *buf);

int ValueBufDtor(ValueBuffer *buf);

int PushValue(ValueBuffer *buf, Buf_t value);

#endif