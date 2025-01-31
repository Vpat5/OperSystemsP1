#include "lab.h"
#include <stdlib.h>
// Code Review: feedback suggested my code was good and did not require updates

list_t *list_add(list_t *list, void *data) {
    if (list == NULL || data == NULL) {
        return NULL;
    }
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        return NULL; // Memory allocation failed
    }
    new_node->data = data;
    new_node->next = list->head->next;
    new_node->prev = list->head;
    list->head->next->prev = new_node;
    list->head->next = new_node;
    list->size++;
    return list;
}

list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    if (list == NULL) {
        return NULL; // Memory allocation failed
    }
    list->destroy_data = destroy_data;
    list->compare_to = compare_to;
    list->size = 0;
    list->head = (node_t *)malloc(sizeof(node_t));
    if (list->head == NULL) {
        free(list);
        return NULL; // Memory allocation failed
    }
    list->head->data = NULL;
    list->head->next = list->head;
    list->head->prev = list->head;
    return list;
}

void list_destroy(list_t **list) {
    if (list == NULL || *list == NULL) {
        return;
    }

    node_t *current = (*list)->head->next;
    while (current != (*list)->head) {
        node_t *next = current->next;
        (*list)->destroy_data(current->data);
        free(current);
        current = next;
    }
    
    free((*list)->head);
    free(*list);
    *list = NULL;
}

void *list_remove_index(list_t *list, size_t index) {
    if (list == NULL || index >= list->size) {
        return NULL;
    }
    node_t *current = list->head->next;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    void *data = current->data;
    current->prev->next = current->next;
    current->next->prev = current->prev;
    free(current);
    list->size--;
    return data;
}

int list_indexof(list_t *list, void *data) {
    if (list == NULL) {
        return -1;
    }
    node_t *current = list->head->next;
    size_t index = 0;
    while (current != list->head) {
        if (list->compare_to(current->data, data) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }
    return -1;
}