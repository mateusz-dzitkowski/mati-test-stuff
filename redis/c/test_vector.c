#include <assert.h>

#include "vector.c"

make_vector_struct(int)

void test_vector_push_back() {
    int_vector_t* v = int_vector_new();
    int_vector_init(v);

    int_vector_push_back(v, 123);

    assert(v->size == 1);
    assert(v->capacity == CAPACITY_STEP);
    assert(v->elements[0] == 123);

    int_vector_free(v);
}

void test_vector_at() {
    int_vector_t* v = int_vector_new();
    int_vector_init(v);

    int_vector_push_back(v, 123);
    int_vector_push_back(v, 321);

    assert(int_vector_at(v, 0) == 123);
    assert(int_vector_at(v, 1) == 321);

    int_vector_free(v);
}

void test_vector_erase_1() {
    int_vector_t* v = int_vector_new();
    int_vector_init(v);

    int_vector_push_back(v, 111);
    int_vector_push_back(v, 222);
    int_vector_push_back(v, 333);
    int_vector_push_back(v, 444);
    int_vector_push_back(v, 555);

    assert(v->size == 5);
    assert(int_vector_at(v, 1) == 222);

    int_vector_erase(v, 2, 2);

    assert(v->size == 3);
    assert(int_vector_at(v, 1) == 222);
    assert(int_vector_at(v, 2) == 555);

    int_vector_free(v);
}

void test_vector_erase_2() {
    int_vector_t* v = int_vector_new();
    int_vector_init(v);

    int_vector_push_back(v, 111);
    int_vector_push_back(v, 222);
    int_vector_push_back(v, 333);

    assert(v->size == 3);
    assert(int_vector_at(v, 1) == 222);

    int_vector_erase(v, 1, 2);

    assert(v->size == 1);
    assert(int_vector_at(v, 0) == 111);

    int_vector_free(v);
}

void test_clear() {
    int_vector_t* v = int_vector_new();
    int_vector_init(v);

    int_vector_push_back(v, 111);
    int_vector_push_back(v, 222);
    int_vector_push_back(v, 333);

    assert(v->size == 3);

    int_vector_clear(v);

    assert(v->size == 0);

    int_vector_free(v);
}

void test_append() {
    int_vector_t* v = int_vector_new();
    int_vector_init(v);

    constexpr int data[] = {123, 456, 789};
    int_vector_append(v, data, 3);

    assert(v->size == 3);
    assert(int_vector_at(v, 0) == 123);
    assert(int_vector_at(v, 1) == 456);
    assert(int_vector_at(v, 2) == 789);

    int_vector_free(v);
}

void test_resize() {
    int_vector_t* v = int_vector_new();
    int_vector_init(v);

    constexpr int data[] = {123, 456, 789};
    int_vector_append(v, data, 3);

    assert(v->size == 3);
    assert(v->capacity == 4);

    int_vector_resize(v, 5);

    assert(v->size == 5);
    assert(v->capacity == 8);

    int_vector_free(v);
}

int main() {
    test_vector_push_back();
    test_vector_at();
    test_vector_erase_1();
    test_vector_erase_2();
    test_clear();
    test_append();
    test_resize();
}
