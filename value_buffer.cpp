#include <stdio.h>
#include "value_buffer.h"
#include "../MyLibraries/headers/systemdata.h"
#include <assert.h>

const int START_SIZE = 8;
const int INCREASE = 2;

static int ValueBufResize(ValueBuffer *buf);

int ValueBufCtor(ValueBuffer *buf) {

    assert(buf);

    buf->capacity = START_SIZE;
    buf->size = 0;
    buf->data = (Buf_t *) calloc (START_SIZE, sizeof (Buf_t));
    if (!buf->data)
        return NO_MEMORY;

    return SUCCESS;
}

int ValueBufDtor(ValueBuffer *buf) {

    assert(buf);

    for (size_t i = 0; i < buf->size; i++) {
        free(buf->data[i]);
        buf->data[i] = NULL;
    }

    free(buf->data);
    buf->capacity = 0;
    buf->size = 0;

    return SUCCESS;
}

int PushValue(ValueBuffer *buf, Buf_t value) {

    assert(buf);
    assert(value);

    if (ValueBufVerify(buf) != SUCCESS)
        return ERROR;

    if (buf->size + 1 >= buf->capacity) {
        if (ValueBufResize(buf) != SUCCESS)
            return NO_MEMORY;
    }

    buf->data[buf->size++] = value;

    return SUCCESS;
}

int ValueBufVerify(ValueBuffer *buf) {

    assert(buf);

    bool error = 0;

    if (!buf->data) {
        printf(RED "Null data pointer" END_OF_COLOR "\n");
        error = 1;
    }

    if (buf->size >= buf->capacity) {
        printf(RED "Incorrect size or capacity" END_OF_COLOR "\n");
        error = 0;
    }

    if (error)
        return ERROR;

    return SUCCESS;
}

static int ValueBufResize(ValueBuffer *buf) {

    assert(buf);

    if (ValueBufVerify(buf) != SUCCESS)
        return ERROR;

    buf->capacity *= INCREASE;
    buf->data = (Buf_t *) realloc (buf->data, buf->capacity * sizeof (Buf_t));
    if (!buf->data)
        return NO_MEMORY;

    return SUCCESS;
}