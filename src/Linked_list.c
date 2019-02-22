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
 *      int data;
 *      list_t ll;
 * 
 *      list_init(&ll, sizeof(int));
 * 
 *      data = 4;
 *      list_push(&ll, (void *) &data);
 *      data = 17;
 *      list_push(&ll, (void *) &data);
 *      data = 22;
 *      list_push(&ll, (void *) &data);
 * 
 *      list_print(&ll, printInt);
 * 
 *      list_get_by_index(&ll, 0, (void *) &data);
 *      printf("Value at position 0: %d\n", data);
 *      list_get_by_index(&ll, 2, (void *) &data);
 *      printf("Value at position 2: %d\n", data);
 * 
 *      ll_destroy(&ll);
 *  @endcode
 */
/******************************************************************************
* Includes
******************************************************************************/
#include "Linked_list.h"        /* Node and linked list structures typedefs*/

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
static node_t* create_node(size_t dataSize, const void* data);
static void free_node(node_t* node);
static void _list_push(list_t* list, const void* data);
static void _list_push_front(list_t* list, const void* data);
static uint8_t _list_pop(list_t* list, void* data);
static uint8_t _list_pop_front(list_t* list, void* data);
static uint8_t _list_get_by_index(list_t* list, uint8_t index, void* data);
static void _list_print(list_t* list, void (*printFn)(const void *data));
static void _list_for_each(list_t* list, void (*eachFn)(const void* data, void* arg), void* arg);
static uint8_t _list_size(list_t* list);

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
 *      newNode = create_node(list->size, (void *) &data);
 * @endcode
 *
 */
/*****************************************************************************/
static node_t*
create_node(size_t dataSize, const void* data)
{
    node_t* newNode = NULL;

    newNode = (node_t *) calloc(1, sizeof(struct node_t));
    newNode->data = calloc(1, dataSize);
    newNode->next = NULL;
    memcpy(newNode->data, data, dataSize);

    return newNode;
}

/*****************************************************************************/
/*!
 * 
 * @internal
 * 
 * \b Description:
 * 
 * This function is used to free the memory of a node. This function is private 
 * and it must only be used by internal methods.
 * 
 * @param node Node to free memory.
 * 
 * @return None.
 * 
 * \b Example:
 * @code
 *      free_node(node);
 * @endcode
 *
 */
/*****************************************************************************/
static void 
free_node(node_t* node)
{
    free(node->data);
    free(node);
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to intialize a linked list structure.
 * 
 * @param list Linked list to be initialized.
 * @param dataSize Size of the data of the nodes.
 * 
 * @return None.
 * 
 * \b Example:
 * @code
 *      list_t* list;
 *      list_init(&list, sizeof(uint32_t));
 * @endcode
 *
 */
/*****************************************************************************/
void
list_init(list_t* list, size_t dataSize)
{
    // Initialize the structure of the linked list
    list->numElements = 0;
    list->dataSize = dataSize;
    list->head = NULL;
    list->tail = NULL;

    // Initialize R/W mutex
    // It is used to avoid working with a busy linked list
    pthread_mutex_init(&(list->lock), NULL);
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
 *      list_free(&list);
 * @endcode
 *
 */
/*****************************************************************************/
void
list_free(list_t* list)
{
    node_t* iterator = list->head;
    node_t* temp = NULL;

    // Traverse the list and free every element
    while (iterator != NULL)
      {
          temp = iterator->next;
          free_node(iterator);
          iterator = temp;
      }

    list->head = NULL;
    list->tail = NULL;
    list->numElements = 0;
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to free the memory of the linked list elements and 
 * of the mutex.
 * 
 * @param list Linked list to free the memory of the elements and mutex.
 * 
 * @return None.
 * 
 * \b Example:
 * @code
 *      list_destroy(&list);
 * @endcode
 *
 */
/*****************************************************************************/
void
list_destroy(list_t* list)
{
    list_free(list);
    pthread_mutex_destroy(&(list->lock));
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
 * @param value Pointer to the variable which value will be inserted at the end 
 *              of the list.
 * 
 * @return None.
 *
 */
/*****************************************************************************/
static void
_list_push(list_t* list, const void* data)
{
    node_t* newNode = NULL;
    newNode = create_node(list->dataSize, data);

    if (list->numElements == 0)
      { 
          list->head = newNode;
          list->tail = list->head;
      }
    else
      {
          list->tail->next = newNode;
          list->tail = newNode; 
      }
    
    list->numElements++;
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to add a node to the end of the list.
 * 
 * @param list Linked list.
 * @param data Pointer to the variable which value will be inserted at the end 
 *             of the list.
 * 
 * @return None.
 * 
 * \b Example:
 * @code
 *      list_push(&list, (void *) &data);
 * @endcode
 *
 */
/*****************************************************************************/
void 
list_push(list_t* list, const void* data)
{
    pthread_mutex_lock(&(list->lock));
        _list_push(list, data);
    pthread_mutex_unlock(&(list->lock));
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
 * @param value Pointer to the variable which value will be inserted at the 
 *              front of the list.
 * 
 * @return None.
 *
 */
/*****************************************************************************/
static void
_list_push_front(list_t* list, const void* data)
{
    node_t* newNode = NULL;

    newNode = create_node(list->dataSize, data);
    newNode->next = list->head;

    if (list->numElements == 0)
      {
          list->tail = newNode; 
      }

    // Insert new node to the front of the list
    list->head = newNode; 

    list->numElements++;
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to add a node to the front of the list.
 * 
 * @param list Linked list.
 * @param data Pointer to the variable which value will be inserted at the 
 *             front of the list.
 * 
 * @return None.
 * 
 * \b Example:
 * @code
 *      list_push_front(&list, (void *) &data);
 * @endcode
 *
 */
/*****************************************************************************/
void
list_push_front(list_t* list, const void* data)
{
    pthread_mutex_lock(&(list->lock));
        _list_push_front(list, data);
    pthread_mutex_unlock(&(list->lock));
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
 * @param data Pointer to the variable to which will be copied the value of the
 *             node at the end of the list.
 * 
 * @return 1 if there are no elements, 0 otherwise.
 * 
 */
/*****************************************************************************/
static uint8_t
_list_pop(list_t* list, void* data)
{
    node_t* iterator = list->head;

    // If the linked list is empty, return error
    if (list->numElements == 0)
      {
          return 1;
      }
    // If there is only one item in the list, remove it
    else if (list->numElements == 1)
      {
          memcpy(data, list->head->data, list->dataSize);
          free_node(list->head);
          list->head = NULL;
          list->tail = NULL;
          list->numElements--;
          
          return 0;
      }

    // Get the penultimate node
    // The penultimate node is required to make it's next variable NULL
    while (iterator->next->next != NULL)
      {
          iterator = iterator->next;
      }

    // Get the last node and delete it
    memcpy(data, iterator->next->data, list->dataSize);
    free_node(iterator->next);
    iterator->next = NULL;
    list->numElements--;
    list->tail = iterator;

    return 0;
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to get the node at the end of the list.
 * 
 * @param list Linked list.
 * @param data Pointer to the variable to which will be copied the value of the
 *             node at the end of the list.
 * 
 * @return 1 if there are no elements, 0 otherwise.
 * 
 * \b Example:
 * @code
 *      uint8_t error = list_pop(&list, (void *) &data);
 * @endcode
 *
 */
/*****************************************************************************/
uint8_t 
list_pop(list_t* list, void* data)
{
    uint8_t retval;

    pthread_mutex_lock(&(list->lock));
        retval = _list_pop(list, data);
    pthread_mutex_unlock(&(list->lock));

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
 * @param data Pointer to the variable to which will be copied the value of the
 *             node at the front of the list.
 * 
 * @return 1 if there are no elements, 0 otherwise.
 * 
 */
/*****************************************************************************/
static uint8_t
_list_pop_front(list_t* list, void* data)
{
    node_t* temp;

    // If the linked list is empty, return error
    if (list->numElements == 0)
      {
          return 1;
      }
    else if (list->numElements == 1)
      {
          memcpy(data, list->head->data, list->dataSize);
          free_node(list->head);
          list->head = NULL;
          list->tail = NULL;
          list->numElements--;

          return 0;
      }

    memcpy(data, list->head->data, list->dataSize);
    temp = list->head;
    list->head = list->head->next;
    free_node(temp);
    list->numElements--;

    return 0;
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to get the node at the front of the list.
 * 
 * @param list Linked list.
 * @param data Pointer to the variable to which will be copied the value of the
 *             node at the front of the list.
 * 
 * @return 1 if there are no elements, 0 otherwise.
 * 
 * \b Example:
 * @code
 *      uint8_t error = list_pop_front(&list, (void *) &data);
 * @endcode
 *
 */
/*****************************************************************************/
uint8_t
list_pop_front(list_t* list, void* data)
{
    uint8_t retval;

    pthread_mutex_lock(&(list->lock));
        retval = _list_pop_front(list, data);
    pthread_mutex_unlock(&(list->lock));

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
 * @param data Pointer to the variable to which will be copied the value of the
 *             node at the given index.
 * 
 * @return 1 if the given index is out of limits, 0 otherwise.
 *
 */
/*****************************************************************************/
static uint8_t
_list_get_by_index(list_t* list, uint8_t index, void* data)
{
    int i;
    node_t* iterator = list->head;

    // Check if index is between limits of linked list
    if(index > list->numElements-1)
      {
          return 1;
      }

    for(i = 0; i < index; i++)
      {
          iterator = iterator->next;
      }

    memcpy(data, iterator->data, list->dataSize);
    return 0;
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
 * @param data Pointer to the variable to which will be copied the value of the
 *             node at the given index.
 * 
 * @return 1 if the given index is out of limits, 0 otherwise.
 * 
 * \b Example:
 * @code
 *      uint8_t error = list_get_by_index(&list, index, (void *) &data);
 * @endcode
 *
 */
/*****************************************************************************/
uint8_t 
list_get_by_index(list_t* list, uint8_t index, void* data)
{
    uint8_t retval;

    pthread_mutex_lock(&(list->lock));
        retval = _list_get_by_index(list, index, data);
    pthread_mutex_unlock(&(list->lock));

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
 * @param printFn Pointer to the function used to print the elements.
 * 
 * @return None.
 *
 */
/*****************************************************************************/
static void 
_list_print(list_t* list, void (*printFn)(const void *data))
{
    node_t* iterator = list->head;

    while (iterator != NULL)
      {
          printFn(iterator->data);
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
 * @param printFn Pointer to the function used to print the elements.
 * 
 * @return None.
 * 
 * \b Example:
 * @code
 *      list_print(&list, printInt);
 * @endcode
 *
 */
/*****************************************************************************/
void
list_print(list_t* list, void (*printFn)(const void* data))
{
    pthread_mutex_lock(&(list->lock));
        _list_print(list, printFn);
    pthread_mutex_unlock(&(list->lock));
}

/*****************************************************************************/
/*!
 * 
 * @internal
 * 
 * \b Description:
 * 
 * This function is used to iterate over the elements of the list.
 * 
 * @param list Linked list.
 * @param eachFn Pointer to the function that will be executed on each element.
 * @param arg Argument passed to eachFn.
 * 
 * @return None.
 *
 */
/*****************************************************************************/
static void 
_list_for_each(list_t* list, 
               void (*eachFn)(const void* data, void* arg), 
               void* arg)
{
    node_t* iterator = list->head;

    while (iterator != NULL)
      {
          eachFn(iterator->data, arg);
          iterator = iterator->next;
      }
}

/*****************************************************************************/
/*!
 * 
 * \b Description:
 * 
 * This function is used to iterate over the elements of the list.
 * 
 * @param list Linked list.
 * @param eachFn Pointer to the function that will be executed on each element.
 * @param arg Argument passed to eachFn.
 * 
 * @return None.
 * 
 * \b Example:
 * @code
 *      list_for_each(list, functionPtr, (void *) &arg);
 * @endcode
 *
 */
/*****************************************************************************/
void
list_for_each(list_t* list, 
              void (*eachFn)(const void* data, void* arg), 
              void* arg)
{
    pthread_mutex_lock(&(list->lock));
        _list_for_each(list, eachFn, arg);
    pthread_mutex_unlock(&(list->lock));
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
static uint8_t
_list_size(list_t* list)
{
    uint8_t retval;

    retval = list->numElements;

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
 *      uint8_t listSize = list_size(&list);
 * @endcode
 *
 */
/*****************************************************************************/
uint8_t
list_size(list_t* list)
{
    uint8_t retval;

    pthread_mutex_lock(&(list->lock));
        retval = _list_size(list);
    pthread_mutex_unlock(&(list->lock));

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
