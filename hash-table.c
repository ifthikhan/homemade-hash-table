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

hash_table ht_new() {

    hash_table table;
    table.size = HASH_TABLE_SIZE;

    /*
     * Initializing the elements in the array to NULL to allow
     * checks of whether a proper hash_value exists or not.
     */
    unsigned i;
    for (i = 0; i < table.size; i++) {
        table.values[i] = NULL;
    }

    return table;
}

/**
 * Set the key/value pair.
 * Currently only strings (char arrays) are allowed as keys.
 */
void ht_set_value(char* key, void* value, hash_table* table) {

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


/*
 * Deleting a node from the doubly linked list needs to take
 * care of the following cases:
 *      # First: The node to be deleted is the first element.
 *            In this case the next element has to be saved in the
 *            hash table.
 *      # Last: The node to be deleted is the last element.
 *      # Between: The element is in between two elements.
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

void* ht_get_value(char* key, hash_table* table) {

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

void ht_delete_value(char* key, hash_table* table) {

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

static hval* hval_new(char* key, void* value) {

    hval* node = (hval *)malloc(sizeof(hval));
    node->key = key;
    node->value = value;
    node->next = NULL;
    node->previous = NULL;

    return node;
}

/**
 * Search the doublyLinkedList for the given key. If the key matches
 * a return the appropriate hval cotainer else return NULL.
 */
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

/**
 * Append the node to the end of the dll.
 */
static void appendto_dll(hval* node, hval* head) {

    if (head->next) {
        appendto_dll(node, head->next);
    } else {
        head->next = node;
        node->previous = head;
    }
}

/**
 * Returns the array index for a given string key. Specify
 * the maximum value to restrict the index to a maximum number.
 */
static unsigned get_index(char* key, unsigned max_val) {

    return simple_hash_generator(key) % (max_val);
}

/**
 * A simple hash generation strategy. Iterate through each of the character in
 * the key, obtain the ascii decimal value and add them up.
 */
static unsigned simple_hash_generator(char* key) {

    unsigned index = 0,
             i;
    for (i = 0; i < sizeof(*key); i++) {
        index += (unsigned) key[i];
    }

    return index;
}
