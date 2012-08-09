#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash-table.h"

static hval* hval_new(char* key, void* value);
static hval* search_dll(char* key, hval* head);
static void appendto_dll(hval* node, hval* head);
static void deletefrom_dll(hval* node);
static unsigned get_index(char* key, unsigned max_val);
static unsigned simple_hash_generator(char* key);

/**
 * Note:
 *      The abbreviation "dll" refers to DoublyLinkedList. The key/value pairs
 *      are stored in a container "hval". This container comprises of pointer
 *      next and previous which points to other key/value pairs which resolves
 *      to the same index (collisions).
 */

/**
 * Creates a new hash table structure, initializes all the elements of
 * the container to null and returns the struct.
 */
hash_table ht_new() {

    hash_table table;
    table.size = HASH_TABLE_SIZE;

    unsigned i;
    for (i = 0; i < table.size; i++) {
        table.values[i] = NULL;
    }

    return table;
}

/**
 * Set the key/value pair. Currently only strings (char arrays) are allowed as
 * keys.
 */
void ht_set_value(hash_table* table, char* key, void* value) {

    unsigned index = get_index(key, table->size);
    hval* head = table->values[index];

    if (head == NULL) {
        table->values[index] = hval_new(key, value);
    } else {
        hval* found = search_dll(key, head);
        if (found) {
            found->value = value;
        } else {
            hval* node = hval_new(key, value);
            appendto_dll(node, head);
        }
    }
}


/**
 * Return the value for the given key, if the key
 * is not found NULL is returned.
 */
void* ht_get_value(hash_table* table, char* key) {

    unsigned index = get_index(key, table->size);

    hval* node = table->values[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            break;
        }

        node = node->next;
    }

    return node->value;
}

/**
 * Deletes the key/value for the given key from the table.
 */
void ht_delete_value(hash_table* table, char* key) {

    unsigned index = get_index(key, table->size);

    hval* node = table->values[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            /*
             * If it's first element then save the pointer of
             * the next element in the hash table.
             */
            if (node->previous == NULL && node->next) {
                table->values[index] = node->next;
            }
            deletefrom_dll(node);
            break;
        }
    }
}

/**
 * Private functions
 *     ||
 *    ||||
 *   ||  ||
 *  ||    ||
 * ||      ||
 * ||||||||||
 * |         |
 * |         |
 * |  Lock   |
 * |_ _ _ _ _|
 */

static hval* hval_new(char* key, void* value) {

    hval* node = (hval *)malloc(sizeof(hval));
    node->key = key;
    node->value = value;
    node->next = NULL;
    node->previous = NULL;

    return node;
}

static hval* search_dll(char* key, hval* head) {

    hval* node = head;
    while (node) {
        if (strcmp(key, node->key) == 0) {
            break;
        }

        node = node->next;
    }

    return node;
}

static void appendto_dll(hval* node, hval* head) {

    if (head->next) {
        appendto_dll(node, head->next);
    } else {
        head->next = node;
        node->previous = head;
    }
}

/*
 * Deleting a node from the doubly linked list needs to take
 * care of the following cases:
 *      # First: The node to be deleted is the first element.
 *      In this case the next hval has to be made the head of the dll.
 *      # Last: The node to be deleted is the last element.
 *      The pointer of the previous hval's next has to refer to NULL.
 *      # Between: The element is in between two elements.
 *      The next and the previous pointers of the hvals on either side
 *      needs to changed.
 */
static void deletefrom_dll(hval* node) {

    if (node->previous == NULL) {
        node->next->previous = NULL;
    } else if (node->next == NULL) {
        node->previous->next = NULL;
    } else {
        node->previous->next = node->next;
        node->next->previous = node->previous;
    }

    free(node);
}

static unsigned get_index(char* key, unsigned max_val) {

    return simple_hash_generator(key) % (max_val);
}

static unsigned simple_hash_generator(char* key) {

    unsigned index = 0,
             i;
    for (i = 0; i < sizeof(*key); i++) {
        index += (unsigned) key[i];
    }

    return index;
}
