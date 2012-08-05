#ifndef HOMEMADE_HASH_TABLE_H
#define HOMEMADE_HASH_TABLE_H


#define HASH_TABLE_SIZE     100

/**
 * A wrapper data structure to store the key/value pairs.
 * It's a DoublyLinkedList and is required to implement open addressing which
 * solves hash collisions.
 */
typedef struct hash_v {
    char *key;
    void *value;
    struct hash_v* next;
    struct hash_v* previous;
} hval;

typedef struct hash_t {
    hval* values[HASH_TABLE_SIZE];
    unsigned size;
} hash_table;


hash_table ht_new();
void ht_set_value(char* key, void* value, hash_table* table);
void* ht_get_value(char* key, hash_table* table);
void ht_delete_value(char* key, hash_table* table);


#endif
