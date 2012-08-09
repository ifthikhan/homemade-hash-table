#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hash-table.h"
#include "main.h"

int main() {

    srand(time(NULL));

    hash_table ht = ht_new();

    char *k1 = "2WwtnwoDx";
    int v1 = 10;
    ht_set_value(k1, &v1, &ht);

    char *k2 = "2Uiihmi2/";
    int v2 = 20;
    ht_set_value(k2, &v2, &ht);

    unsigned i,
             iterations = 200;
    char keys[iterations][10];
    unsigned vals[iterations];
    for (i = 0; i < iterations; i++) {
        vals[i] = i;
        random_string(keys[i], 9);
        ht_set_value(&ht, keys[i], (void *)(vals + i));
    }

    print_hash_table(&ht);

    return EXIT_SUCCESS;
}

void random_string(char* string, unsigned max_len) {

    unsigned int random = rand();
    char ascii[] = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    unsigned int i, v;
    for (i = 0; i < max_len - 1; i++) {
        random = rand();
        v = random % 61;
        string[i] = ascii[v];
    }
    string[++i] = '\0';
}

void get_value_and_print(char* key, hash_table* table) {

    int v = *((int *) ht_get_value(table, key));
    printf("%d \n", v);
}

void print_hash_table(hash_table* ht) {

    printf("The size of the hash table is %d \n", ht->size);

    unsigned int i;
    hval* node;
    int* value;
    unsigned index_num_elements[ht->size];
    unsigned num_elements;

    for (i = 0; i < ht->size; i++) {
        num_elements = 0;

        node = ht->values[i];
        if (node == NULL) {
            index_num_elements[i] = num_elements;
            continue;
        }

        printf("[%02d] ", i);
        while (node) {
            value = (int *)node->value;
            printf("(%s => %d (*%p)) ->", node->key, *value, value);
            node = node->next;
            num_elements++;
        }
        printf("\n");

        index_num_elements[i] = num_elements;
    }

    for (i = 0; i < ht->size; i++) {
        printf("%d,%d \n", i, index_num_elements[i]);
    }
}
