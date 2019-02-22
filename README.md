# linked_list

linked_list is an implementation of a singly-linked list in C, featuring
a simple API, ease-of-use, and type-safety.

## Example

```c
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

void printInt(const void *data)
{
    printf("%d :: ", *(const int *)data);
}

int main(int argc, char** argv)
{
    int data;
    list_t l;

    list_init(&l, sizeo(int));    

    data = 10;
    list_push(&l, (void *) &data);
    data = 20;
    list_push(&l, (void *) &data);
    data = 30;
    list_push(&l, (void *) &data);

    list_print(&l, printInt);

    list_get_by_index(&l, 0, (void *) &data);
    printf("Value: %d\n", data);
    list_get_by_index(&l, 1, (void *) &data);
    printf("Value: %d\n", data);
    list_get_by_index(&l, 2, (void *) &data);
    printf("Value: %d\n", data);

    ll_delete(&l);

    return 0;
}
```

## Creating a list

Create a list by using the function `list_init` which receives a pointer 
to a variable of type `list_t`.

```c
list_t l;
list_init(&l, sizeof(int));
```

## Adding to a list

To add an element to the list use the function `list_push` passing as
parameters the list and the value to add.

```c
int data = 10;
list_push(&l, (void *) &data);
```

## Getting items from a list

To get an element from the list use the function `list_get_by_index` passing as
parameters the list, the position of the element and a pointer to the return
variable.

```c
int data;
list_get_by_index(&l, 0, (void *) &data);
```

## Print elements in a list

To print the elements of a list use the function `list_print` passing as
arguments the list and a pointer to the function that correctly prints the
elements.

```c
list_print(&l, printFn);
```

## Destroying a list

To destroy a list use the function `ll_delete` passing as parameter
the list. This function uses `free` to deallocate the memory of the list.

```c
ll_delete(&l);
```

## Build

To build the project use the following command in the root directory
of the project.

    $ make

