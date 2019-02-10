/******************************************************************************
* Title                 :   Linked list source file 
* Filename              :   Linked_list.c
* Author                :   Maximiliano Valencia
* Origin Date           :   09/02/2019
* Version               :   1.0.0
* Compiler              :   gcc
* Target                :   STM32
* Notes                 :   None
******************************************************************************/
/*! @file Linked_list.c
 *  @brief Linked list implementation
 * 
 *  To use the linked list implementation, include this header file as follows:
 *  @code
 *  #include "Linked_list.h"
 *  @endcode
 * 
 *  ## Overview ##
 *  A Linked List is used to implement different data structures like queues 
 *  and state machines. The behaviour of a Linked List is similar to a Last In 
 *  First Out (LIFO) memory but methods to push and pop to the front of the 
 *  list are also implemented.
 * 
 *  ## Usage ##
 * 
 *  The Linked List implementation provides APIs to write and get elements from
 *  the linked list.
 *  The following code example initializes the linked list, writes to the 
 *  linked list, and then gets elements from the linked list.
 * 
 *  @code
 *      list_t* ll = list_init();
 * 
 *      list_push(ll, 4);
 *      list_push(ll, 17);
 *      list_push(ll, 22);
 * 
 *      list_print(ll);
 * 
 *      printf("Value at position 0: %d\n", list_get_by_index(ll, 0));
 *      printf("Value at position 2: %d\n", list_get_by_index(ll, 2));
 * 
 *      ll_destroy(ll);
 *  @endcode
 */
/******************************************************************************
* Includes
******************************************************************************/
#include "Linked_list.h"        /* For TODO: WHY ME? */

/******************************************************************************
* Module Preprocessor Constants
******************************************************************************/


/******************************************************************************
* Module Preprocessor Macros
******************************************************************************/


/******************************************************************************
* Module Typedefs
******************************************************************************/


/******************************************************************************
* Module Variable Definitions
******************************************************************************/


/******************************************************************************
* Function Prototypes
******************************************************************************/
static node_t* create_node(int value);
static void _list_push(list_t* list, int value);
static void _list_push_front(list_t* list, int value);
static int _list_pop(list_t* list);
static int _list_pop_front(list_t* list);
static int _list_get_by_index(list_t* list, unsigned int index);
static void _list_print(list_t* list);
static int _list_size(list_t* list);

/******************************************************************************
* Function Definitions
******************************************************************************/


/*****************************************************************************/
/*!
 *
 * @addtogroup linked_list
 * @{
 *
 */
/*****************************************************************************/


/*****************************************************************************/
/*!
 * 
 * @internal
 * 
 * \b Description:
 * 
 * This function is used to create and allocate memory for a new node. This 
 * function is private and it must only be used by internal methods.
 * 
 * @param value Value of the new node.
 * 
 * @return A pointer to the new allocated node.
 * 
 * \b Example:
 * @code
 *      node_t* newNode = NULL;
 *      newNode = create_node(value);
 * @endcode
 *
 * /
/*****************************************************************************/
static node_t*
create_node(int value)
{
    node_t* newNode = NULL;

    newNode = (node_t *) malloc(sizeof(struct node_t));
    newNode->value = value;
    newNode->next = NULL;

    return newNode;
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to intialize a linked list structure.
 * 
 * @param list Linked list to be initialized.
 * 
 * @return None.
 * 
 * \b Example:
 * @code
 *      list_t* list;
 *      list_init(&list);
 * @endcode
 *
 */
/*****************************************************************************/
void
list_init(list_t** list)
{
    *list = malloc(sizeof(struct list_t));

    // Initialize the structure of the linked list
    (*list)->size = 0;
    (*list)->head = NULL;
    (*list)->tail = NULL;

    // Initialize R/W semaphore
    // It is used to avoid working with a busy linked list
    sem_init(&( (*list)->lock ), 0, 1);
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to release the memory of the linked list elements.
 * 
 * @param list Linked list to free the memory of its elements.
 * 
 * @return None.
 * 
 * \b Example:
 * @code
 *      list_free(list);
 * @endcode
 *
 */
/*****************************************************************************/
void
list_free(list_t* list)
{
    node_t* iterator = list->head;
    node_t* tmp = NULL;

    // Traverse the list and free every element
    while (iterator != NULL)
      {
          tmp = iterator->next;
          free(iterator);
          iterator = tmp;
      }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to free the memory of the linked list elements and 
 * of the linked list.
 * 
 * @param list Linked list to free the memory of the elements and the linked 
 *             list.
 * 
 * @return None.
 * 
 * \b Example:
 * @code
 *      list_destroy(list);
 * @endcode
 *
 */
/*****************************************************************************/
void
list_destroy(list_t* list)
{
    list_free(list);
    sem_destroy(&(list->lock));
    free(list);
}

/*****************************************************************************/
/*!
 * 
 * @internal
 * 
 * \b Description:
 * 
 * This function is used to add a node to the end of the list.
 * 
 * @param list Linked list.
 * @param value Value of new node to be inserted at the end of the list.
 * 
 * @return None.
 *
 */
/*****************************************************************************/
static void
_list_push(list_t* list, int value)
{
    node_t* newNode = NULL;
    newNode = create_node(value);

    if (list->size == 0)  // Linked list has no elements
      { 
          list->head = newNode;
          list->tail = list->head;
      }
    else
      {
          list->tail->next = newNode;
          // Set linked list tail to the new node
          list->tail = newNode; 
      }
    
    list->size++;
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to add a node to the end of the list.
 * 
 * @param list Linked list.
 * @param value Value of new node to be inserted at the end of the list.
 * 
 * @return None.
 * 
 * \b Example:
 * @code
 *      list_push(list, value);
 * @endcode
 *
 */
/*****************************************************************************/
void 
list_push(list_t* list, int value)
{
    sem_wait(&(list->lock));
        _list_push(list, value);
    sem_post(&(list->lock));
}

/*****************************************************************************/
/*!
 * 
 * @internal
 * 
 * \b Description:
 * 
 * This function is used to add a node to the front of the list.
 * 
 * @param list Linked list.
 * @param value Value of new node to be inserted at the front of the list.
 * 
 * @return None.
 *
 */
/*****************************************************************************/
static void
_list_push_front(list_t* list, int value)
{
    node_t* newNode = NULL;

    newNode = create_node(value);
    newNode->next = list->head;

    if (list->size == 0) // Linked list has no elements
            list->tail = newNode; 
    
    list->head = newNode; // Insert new node to the front of the list

    list->size++;
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to add a node to the front of the list.
 * 
 * @param list Linked list.
 * @param value Value of new node to be inserted at the front of the list.
 * 
 * @return None.
 * 
 * \b Example:
 * @code
 *      list_push_front(list, value);
 * @endcode
 *
 */
/*****************************************************************************/
void
list_push_front(list_t* list, int value)
{
    sem_wait(&(list->lock));
        _list_push_front(list, value);
    sem_post(&(list->lock));
}

/*****************************************************************************/
/*!
 * 
 * @internal
 * 
 * \b Description:
 * 
 * This function is used to get the node at the end of the list.
 * 
 * @param list Linked list.
 * 
 * @return Value of the node at the end of the list.
 * 
 */
/*****************************************************************************/
static int
_list_pop(list_t* list)
{
    int retval = -1;
    node_t* iterator = list->head;

    // If the linked list is empty, return error
    if (list->size == 0)
            return retval;

    // If there is only one item in the list, remove it
    if (list->size == 1)
      {
          retval = list->head->value;
          free(list->head);
          list->head = NULL;
          list->size--;
          
          return retval;
      }

    // Get the penultimate node
    // The penultimate node is required to make it's next variable NULL
    while (iterator->next->next != NULL)
            iterator = iterator->next;

    // Get the last node and delete it
    retval = iterator->next->value;
    free(iterator->next);
    iterator->next = NULL;
    list->size--;
    list->tail = iterator;

    return retval;
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to get the node at the end of the list.
 * 
 * @param list Linked list.
 * 
 * @return Value of the node at the end of the list.
 * 
 * \b Example:
 * @code
 *      int retval = list_pop(list);
 * @endcode
 *
 */
/*****************************************************************************/
int 
list_pop(list_t* list)
{
    int retval = -1;

    sem_wait(&(list->lock));
        retval = _list_pop(list);
    sem_post(&(list->lock));

    return retval;
}

/*****************************************************************************/
/*!
 * 
 * @internal
 * 
 * \b Description:
 * 
 * This function is used to get the node at the front of the list.
 * 
 * @param list Linked list.
 * 
 * @return Value of the node at the front of the list.
 * 
 */
/*****************************************************************************/
static int
_list_pop_front(list_t* list)
{
    int retval = -1;
    node_t* temp;

    // If the linked list is empty, return error
    if (list->size == 0)
            return retval;

    retval = list->head->value;
    temp = list->head->next;
    free(list->head);
    list->head = temp;
    list->size--;

    return retval;
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to get the node at the front of the list.
 * 
 * @param list Linked list.
 * 
 * @return Value of the node at the front of the list.
 * 
 * \b Example:
 * @code
 *      int retval = list_pop_front(list);
 * @endcode
 *
 */
/*****************************************************************************/
int
list_pop_front(list_t* list)
{
    int retval = -1;

    sem_wait(&(list->lock));
        retval = _list_pop_front(list);
    sem_post(&(list->lock));

    return retval;
}

/*****************************************************************************/
/*!
 * 
 * @internal
 * 
 * \b Description:
 * 
 * This function is used to get the value of a node at a given index.
 * It doesn't delete the node.
 * 
 * @param list Linked list.
 * @param index Index of the node.
 * 
 * @return Value of the node at the given index.
 *
 */
/*****************************************************************************/
static int
_list_get_by_index(list_t* list, unsigned int index)
{
    int i;
    node_t* iterator = list->head;

    // Check if index is between limits of linked list
    if(index > list->size-1)
            return -1;

    for(i = 0; i < index; i++)
            iterator = iterator->next;

    return iterator->value;
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to get the value of a node at a given index.
 * It doesn't delete the node.
 * 
 * @param list Linked list.
 * @param index Index of the node.
 * 
 * @return Value of the node at the given index.
 * 
 * \b Example:
 * @code
 *      int retval = list_get_by_index(list, index);
 * @endcode
 *
 */
/*****************************************************************************/
int 
list_get_by_index(list_t* list, int index)
{
    int retval = -1;

    sem_wait(&(list->lock));
        retval = _list_get_by_index(list, index);
    sem_post(&(list->lock));

    return retval;
}

/*****************************************************************************/
/*!
 * 
 * @internal
 * 
 * \b Description:
 * 
 * This function is used to print the values of the list.
 * 
 * @param list Linked list.
 * 
 * @return None.
 *
 */
/*****************************************************************************/
static void 
_list_print(list_t* list)
{
    node_t* iterator = list->head;

    while (iterator != NULL)
      {
          printf("%d :: ", iterator->value);
          iterator = iterator->next;
      }

    printf("\n");
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to print the values of the list.
 * 
 * @param list Linked list.
 * 
 * @return None.
 * 
 * \b Example:
 * @code
 *      list_print(list);
 * @endcode
 *
 */
/*****************************************************************************/
void
list_print(list_t* list)
{
    sem_wait(&(list->lock));
        _list_print(list);
    sem_post(&(list->lock));
}

/*****************************************************************************/
/*!
 * 
 * @internal
 * 
 * \b Description:
 * 
 * This function is used to get the number of elements in the list.
 * 
 * @param list Linked list.
 * 
 * @return Number of elements in the list.
 *
 */
/*****************************************************************************/
static int
_list_size(list_t* list)
{
    int retval = -1;

    retval = list->size;

    return retval;
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to get the number of elements in the list.
 * 
 * @param list Linked list.
 * 
 * @return Number of elements in the list.
 * 
 * \b Example:
 * @code
 *      int listSize = list_size(list);
 * @endcode
 *
 */
/*****************************************************************************/
int
list_size(list_t* list)
{
    int retval = -1;

    sem_wait(&(list->lock));
        retval = _list_size(list);
    sem_post(&(list->lock));

    return retval;
}

/*****************************************************************************/
/*!
 *
 * Close the Doxygen group.
 * @}
 *
 */
/*****************************************************************************/
