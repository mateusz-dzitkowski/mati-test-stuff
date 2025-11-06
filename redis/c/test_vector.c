#include <assert.h>

#include "vector.c"

void test_vector_is_full() {
    auto* v = vector_new();
    vector_init(v);

    assert(vector_is_full(v) == false);

    for (int i = 0; i < CAPACITY_STEP; i++) {
        vector_push_back(v, 0);
    }

    assert(vector_is_full(v) == true);

    vector_free(v);
}

void test_vector_push_back() {
    vector_t* v = vector_new();
    vector_init(v);

    vector_push_back(v, 123);

    assert(v->size == 1);
    assert(v->capacity == CAPACITY_STEP);
    assert(v->elements[0] == 123);

    vector_free(v);
}

void test_vector_at() {
    vector_t* v = vector_new();
    vector_init(v);

    vector_push_back(v, 123);
    vector_push_back(v, 321);

    assert(vector_at(v, 0) == 123);
    assert(vector_at(v, 1) == 321);

    vector_free(v);
}

void test_vector_pop_back() {
    vector_t* v = vector_new();
    vector_init(v);

    vector_push_back(v, 123);
    vector_push_back(v, 321);

    assert(v->size == 2);
    assert(vector_at(v, 0) == 123);
    assert(vector_at(v, 1) == 321);

    vector_pop_back(v);

    assert(v->size == 1);
    assert(vector_at(v, 0) == 123);

    vector_free(v);
}

void test_vector_erase_1() {
    vector_t* v = vector_new();
    vector_init(v);

    vector_push_back(v, 111);
    vector_push_back(v, 222);
    vector_push_back(v, 333);
    vector_push_back(v, 444);
    vector_push_back(v, 555);

    assert(v->size == 5);
    assert(vector_at(v, 1) == 222);

    vector_erase(v, 2, 2);

    assert(v->size == 3);
    assert(vector_at(v, 1) == 222);
    assert(vector_at(v, 2) == 555);

    vector_free(v);
}

void test_vector_erase_2() {
    vector_t* v = vector_new();
    vector_init(v);

    vector_push_back(v, 111);
    vector_push_back(v, 222);
    vector_push_back(v, 333);

    assert(v->size == 3);
    assert(vector_at(v, 1) == 222);

    vector_erase(v, 1, 2);

    assert(v->size == 1);
    assert(vector_at(v, 0) == 111);

    vector_free(v);
}

void test_erase_one() {
    vector_t* v = vector_new();
    vector_init(v);

    vector_push_back(v, 111);
    vector_push_back(v, 222);
    vector_push_back(v, 333);

    assert(v->size == 3);

    vector_erase_one(v, 0);

    assert(v->size == 2);
    assert(vector_at(v, 0) == 222);
    assert(vector_at(v, 1) == 333);

    vector_free(v);
}

int main() {
    test_vector_is_full();
    test_vector_push_back();
    test_vector_at();
    test_vector_pop_back();
    test_vector_erase_1();
    test_vector_erase_2();
    test_erase_one();
}
