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
#include <pthread.h>
#include <semaphore.h>

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
typedef struct list_t list_t;
typedef struct node_t node_t;

/*! @brief Linked list structure definition */
struct list_t
{
    int size;       /**< Size of the linked list */
    node_t* head;   /**< Pointer to the head the linked list */
    node_t* tail;   /**< Pointer to the tail linked list */
    sem_t lock;     /**< Semaphore used to lock the linked list */
};

/*! @brief Node structure definition */
struct node_t
{
    int value;      /**< Integer value of the node */
    node_t* next;   /**< Pointer to the next node */
};

/******************************************************************************
* Variables
******************************************************************************/


/******************************************************************************
* Function Prototypes
******************************************************************************/
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
