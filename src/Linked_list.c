//*****************************************************************************
//
//! \file        Linked_list.c
//! \brief       Linked list implementation
//!
//! To use the linked list implementation, include this header file as follows:
//! \code
//! #include "Linked_list.h"
//! \endcode
//!
//! ## Overview ##
//! A Linked List is used to implement different data structures like queues 
//! and state machines. The behaviour of a Linked List is similar to a Last In 
//! First Out (LIFO) memory but methods to push and pop to the front of the 
//! list are also implemented.
//!
//! ## Usage ##
//!
//! The Linked List implementation provides APIs to write and get elements from
//! the linked list.
//! The following code example initializes the linked list, writes to the 
//! linked list, and then gets elements from the linked list.
//!
//! \code
//!     list_t ll = list_init();
//!
//!     list_push(ll, 4);
//!     list_push(ll, 17);
//!     list_push(ll, 22);
//!
//!     list_print(ll);
//!
//!     printf("Value at position 0: %d\n", list_get_by_index(ll, 0));
//!     printf("Value at position 2: %d\n", list_get_by_index(ll, 2));
//!
//!     ll_delete(l);
//! \endcode
//!
//! \author       Maximiliano Valencia.
//! \date         8/2/2018.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup linked_list
//! @{
//
//*****************************************************************************

#include "Linked_list.h"

//*****************************************************************************
//
//! \internal
//! Create and allocate memory for a new node.
//!
//! \param value Value of the new node.
//!
//! \return Allocated node.
//
//*****************************************************************************
static node_t*
create_node(int value)
{
    node_t* newNode = NULL;

    newNode = (node_t *) malloc(sizeof(struct node_t));
    newNode->value = value;
    newNode->next = NULL;

    return newNode;
}

//*****************************************************************************
//
//! Linked list constructor.
//!
//! \param list Linked list to be initialized.
//!
//! \return None.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Release the memory of the linked list elements.
//!
//! \param list Linked list.
//!
//! \return None.
//
//*****************************************************************************
void
list_free(list_t* list)
{
    node_t* iterator = list->head;
    node_t* tmp = NULL;

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

//*****************************************************************************
//
//! Release the memory of the linked list elements and of the inked list.
//!
//! \param list Linked list.
//!
//! \return None.
//
//*****************************************************************************
void
list_destroy(list_t* list)
{
    list_free(list);
    sem_destroy(&(list->lock));
    free(list);
}

//*****************************************************************************
//
//! \internal
//! Add a node to the end of the list.
//!
//! \param list Linked list.
//! \param value Value of new node to be inserted at the end of the list.
//!
//! \return None.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Add a node to the end of the list.
//!
//! \param list Linked list.
//! \param value Value of new node to be inserted at the end of the list.
//!
//! \return None.
//
//*****************************************************************************
void 
list_push(list_t* list, int value)
{
    sem_wait(&(list->lock));
        _list_push(list, value);
    sem_post(&(list->lock));
}

//*****************************************************************************
//
//! \internal
//! Add a node to the front of the list.
//!
//! \param list Linked list.
//! \param value Value of new node to be inserted at the front of the list.
//!
//! \return None.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Add a node to the front of the list.
//!
//! \param list Linked list.
//! \param value Value of new node to be inserted at the front of the list.
//!
//! \return None.
//
//*****************************************************************************
void
list_push_front(list_t* list, int value)
{
    sem_wait(&(list->lock));
        _list_push_front(list, value);
    sem_post(&(list->lock));
}

//*****************************************************************************
//
//! \internal
//! Get the node at the end of the list.
//!
//! \param list Linked list.
//!
//! \return Value of the node at the end of the list.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Get the node at the end of the list.
//!
//! \param list Linked list.
//!
//! \return Value of the node at the end of the list.
//
//*****************************************************************************
int 
list_pop(list_t* list)
{
    int retval = -1;

    sem_wait(&(list->lock));
        retval = _list_pop(list);
    sem_post(&(list->lock));

    return retval;
}

//*****************************************************************************
//
//! \internal
//! Get the node at the front of the list.
//!
//! \param list Linked list.
//!
//! \return Value of the node at the front of the list.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Get the node at the front of the list.
//!
//! \param list Linked list.
//!
//! \return Value of the node at the front of the list.
//
//*****************************************************************************
int
list_pop_front(list_t* list)
{
    int retval = -1;

    sem_wait(&(list->lock));
        retval = _list_pop_front(list);
    sem_post(&(list->lock));

    return retval;
}

//*****************************************************************************
//
//! \internal
//! Get the value of node given the index.
//!
//! \warning Doesn't delete the node.
//!
//! \param list Linked list.
//! \param index Index of node.
//!
//! \return Value of the node at the given index.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Get the value of node given the index.
//!
//! \warning Doesn't delete the node.
//!
//! \param list Linked list.
//! \param index Index of node.
//!
//! \return Value of the node at the given index.
//
//*****************************************************************************
int 
list_get_by_index(list_t* list, int index)
{
    int retval = -1;

    sem_wait(&(list->lock));
        retval = _list_get_by_index(list, index);
    sem_post(&(list->lock));

    return retval;
}

//*****************************************************************************
//
//! \internal
//! Print the values of the list.
//!
//! \param list Linked list.
//!
//! \return None.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Print the values of the list.
//!
//! \param list Linked list.
//!
//! \return None.
//
//*****************************************************************************
void
list_print(list_t* list)
{
    sem_wait(&(list->lock));
        _list_print(list);
    sem_post(&(list->lock));
}

//*****************************************************************************
//
//! \internal
//! Get the number of elements in the list.
//!
//! \param list Linked list.
//!
//! \return Number of elements in the list.
//
//*****************************************************************************
static int
_list_size(list_t* list)
{
    int retval = -1;

    retval = list->size;

    return retval;
}

//*****************************************************************************
//
//! Get the number of elements in the list.
//!
//! \param list Linked list.
//!
//! \return Number of elements in the list.
//
//*****************************************************************************
int
list_size(list_t* list)
{
    int retval = -1;

    sem_wait(&(list->lock));
        retval = _list_size(list);
    sem_post(&(list->lock));

    return retval;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
