#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct list_t list_t;
typedef struct node_t node_t;

/*! @brief Linked list structure */
struct list_t
{
    int size; /**< Size of the linked list */
    node_t* head; /**< Pointer to the head the linked list */
    node_t* tail; /**< Pointer to the tail linked list */
    sem_t lock; /**< Semaphore used to lock the linked list */
};

/*! @brief Node structure */
struct node_t
{
    int value; /**< Integer value of the node */
    node_t* next; /**< Pointer to the next node */
};

void list_init(list_t** list);
void list_free(list_t* list);
void list_destroy(list_t* list);
void list_push(list_t* list, int value);
void list_push_front(list_t* list, int value);
int list_pop(list_t* list);
int list_pop_front(list_t* list);
int list_get_by_index(list_t* list, int index);
void list_print(list_t* list);
int list_size(list_t* list);

#endif /* LINKED_LIST_H */
