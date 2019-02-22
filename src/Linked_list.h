/******************************************************************************
* Title                 :   Linked list header file  
* Filename              :   Linked_list.h
* Author                :   Maximiliano Valencia
* Origin Date           :   09/02/2019
* Version               :   1.0.0
* Compiler              :   gcc
* Target                :   STM32
* Notes                 :   None
******************************************************************************/
/** @file Linked_list.h
 *  @brief Defines the prototypes of the linked list.
 * 
 *  This is the header file for the definition of the node and linked list
 *  structures and typedefs as well as the function prototypes of the methods
 *  of the linked list.
 */
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/******************************************************************************
* Includes
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/******************************************************************************
* Preprocessor Constants
******************************************************************************/


/******************************************************************************
* Configuration Constants
******************************************************************************/


/******************************************************************************
* Macros
******************************************************************************/


/******************************************************************************
* Typedefs
******************************************************************************/
/**
 * Linked list type definition
 */
typedef struct list_t list_t;
/**
 * Node type definition
 */
typedef struct node_t node_t;

/*! @brief Linked list structure definition */
struct list_t
{
    uint8_t numElements;    /**< Number of elements in the linked list */
    size_t dataSize;        /**< Size of data of the nodes */
    node_t* head;           /**< Pointer to the head the linked list */
    node_t* tail;           /**< Pointer to the tail linked list */
    pthread_mutex_t lock;   /**< Mutex used to lock the linked list */
};

/*! @brief Node structure definition */
struct node_t
{
    void* data;     /**< Pointer to the data of the node */
    node_t* next;   /**< Pointer to the next node */
};

/******************************************************************************
* Variables
******************************************************************************/


/******************************************************************************
* Function Prototypes
******************************************************************************/
void list_init(list_t* list, size_t dataSize);
void list_free(list_t* list);
void list_destroy(list_t* list);
void list_push(list_t* list, const void* data);
void list_push_front(list_t* list, const void* data);
uint8_t list_pop(list_t* list, void* data);
uint8_t list_pop_front(list_t* list, void* data);
uint8_t list_get_by_index(list_t* list, uint8_t index, void* data);
void list_print(list_t* list, void (*printFn)(const void* data));
void list_for_each(list_t* list, void (*eachFn)(const void* data, void* arg), void* arg);
uint8_t list_size(list_t* list);

#endif /* LINKED_LIST_H */
