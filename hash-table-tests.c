#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "hash-table.h"


static void test_hashitem_set_and_get() {

    hash_table table = ht_new();

    char* key = "hello";
    int value = 100;
    ht_set_value(&table, key, (void *) &value);

    int* value_get = (int *)ht_get_value(&table, key);
    assert(value == *value_get
            && "The value set is not the same as the get");
}

static void test_hashitem_collisions() {

    hash_table table = ht_new();

    /*The keys k1 and k2 create the same index.*/
    char *k1 = "2WwtnwoDx";
    int v1 = 10;
    ht_set_value(&table, k1, &v1);

    char *k2 = "2Uiihmi2/";
    int v2 = 20;
    ht_set_value(&table, k2, &v2);

    int* v1_get = (int *)ht_get_value(&table, k1);
    assert(v1 == *v1_get
            && "The value set is not the same as get");

    int* v2_get = (int *)ht_get_value(&table, k2);
    assert(v2 == *v2_get
            && "The value set is not the same as get");
}

static void test_hashitem_delete() {

    hash_table table = ht_new();

    char* key = "hello";
    int value = 100;
    ht_set_value(&table, key, (void *) &value);

    int* value_get = (int *)ht_get_value(&table, key);
    assert(value == *value_get
            && "The value set is not the same as the get");

    ht_delete_value(&table, key);

    value_get = (int *)ht_get_value(&table, key);
    if (value_get != NULL) {
        assert(0 && "The value is not deleted.");
    }
}

int main() {

    test_hashitem_set_and_get();
    test_hashitem_collisions();
    test_hashitem_delete();

    return EXIT_SUCCESS;
}
