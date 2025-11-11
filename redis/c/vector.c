#include <stdlib.h>
#include <stdio.h>

constexpr int CAPACITY_STEP = 4;

#define _vector_struct(T) \
    typedef struct T##_vector_t { \
        T* elements; \
        size_t capacity; \
        size_t size; \
    } T##_vector_t;

#define _vector_new(T) \
    T##_vector_t* T##_vector_new() { \
        return (T##_vector_t*) malloc(sizeof(T##_vector_t)); \
    } \

#define _vector_init(T) \
    void T##_vector_init(T##_vector_t* v) { \
        v->elements = (T*) calloc(CAPACITY_STEP, sizeof(T)); \
        v->capacity = CAPACITY_STEP; \
        v->size = 0; \
    }

#define _vector_free(T) \
void T##_vector_free(T##_vector_t* v) { \
    free(v->elements); \
    free(v); \
}

#define _vector_resize(T) \
    void T##_vector_resize(T##_vector_t* v, size_t size) { \
        if (size > v->capacity) { \
            auto *temp = (T*) realloc(v->elements, size * sizeof(T)); \
            if (temp == NULL) { \
                T##_vector_free(v); \
                abort(); \
            } \
            v->elements = temp; \
            v->capacity = size + CAPACITY_STEP - size % CAPACITY_STEP; \
        } \
        v->size = size; \
    }

#define _vector_push_back(T) \
    void T##_vector_push_back(T##_vector_t* v, const T elem) { \
        T##_vector_resize(v, v->size + 1); \
        v->elements[v->size - 1] = elem; \
    }

#define _vector_append(T) \
    void T##_vector_append(T##_vector_t* v, const T *data, size_t len) { \
        for (size_t i = 0; i < len; i++) { \
            T##_vector_push_back(v, data[i]); \
        } \
    }

#define _vector_at(T) \
    T T##_vector_at(const T##_vector_t* v, const size_t index) { \
        return v->elements[index]; \
    }

#define _vector_clear(T) \
    void T##_vector_clear(T##_vector_t* v) { \
        v->size = 0; \
    }

#define _vector_erase(T) \
    void T##_vector_erase(T##_vector_t* v, const int start_index, const size_t erase_amount) { \
        for (int i = start_index; i + erase_amount <= v->size; i++) { \
            v->elements[i] = v->elements[i + erase_amount]; \
        } \
        v->size -= erase_amount; \
    }

#define make_vector_struct(T) \
    _vector_struct(T) \
    _vector_new(T) \
    _vector_free(T) \
    _vector_init(T) \
    _vector_resize(T) \
    _vector_push_back(T) \
    _vector_at(T) \
    _vector_clear(T) \
    _vector_erase(T) \
    _vector_append(T)
