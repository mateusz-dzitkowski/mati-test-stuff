#include <stdlib.h>
#include <stdio.h>

constexpr int CAPACITY_STEP = 4;

typedef struct vector_t {
    int* elements;
    int capacity;
    int size;
} vector_t;

vector_t* vector_new() {
    return malloc(sizeof(vector_t));
}

void vector_init(vector_t* v) {
    v->elements = (int*) calloc(CAPACITY_STEP, sizeof(int));
    v->capacity = CAPACITY_STEP;
    v->size = 0;
}

bool vector_is_full(const vector_t* v) {
    return v->size > 0 && v->size % v->capacity == 0;
}

void vector_free(vector_t* v) {
    free(v->elements);
    free(v);
}

void vector_push_back(vector_t* v, const int elem) {
    if (vector_is_full(v)) {
        v->capacity = v->size + CAPACITY_STEP;
        auto *temp = (int*) realloc(v->elements, v->capacity * sizeof(int));
        if (temp == NULL) {
            vector_free(v);
            abort();
        }
        v->elements = temp;
    }
    v->elements[v->size++] = elem;
}

int vector_at(const vector_t* v, const size_t index) {
    return v->elements[index];
}

void vector_pop_back(vector_t* v) {
    if (v->size == 0) {
        return;
    }
    v->elements[v->size - 1] = 0;
    v->size--;
}

void vector_erase(vector_t* v, const int start_index, const uint8_t erase_amount) {
    for (int i = start_index; i + erase_amount <= v->size; i++) {
        v->elements[i] = v->elements[i + erase_amount];
    }

    v->size -= erase_amount;
}

void vector_erase_one(vector_t* v, const int index) {
    vector_erase(v, index, 1);
}

void vector_print_elements(const vector_t* v) {
    printf("[");
    for (int i = 0; i < v->size; i++) {
        if (i != 0) {
            printf(",");
        }
        printf("%d", vector_at(v, i));
    }
    printf("]\n");
}

void vector_print_stats(const vector_t* v) {
    printf("size: %d, cap: %d\n", v->size, v->capacity);
}
