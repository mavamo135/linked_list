# linked_list

linked_list is an implementation of a singly-linked list (stack) in C, featuring
a simple API, ease-of-use, and type-safety.

## Example

```c
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

int main(int argc, char** argv)
{
    list_t* l = list_init();    

    list_push(l, 10);
    list_push(l, 20);
    list_push(l, 30);

    list_print(l);

    printf("Value: %d\n", list_get_by_index(l, 0));
    printf("Value: %d\n", list_get_by_index(l, 1));
    printf("Value: %d\n", list_get_by_index(l, 2));

    ll_delete(l);

    return 0;
}
```

## Creating a list

Create a list by using the function `l_init()` which returns a pointer 
to a variable of type `list_t`.

```c
list_t* l = list_init();
```

## Adding to a list

To add an element to the list use the function `list_push` passing as
parameters the list and the value to add.

```c
list_push(l, 10);
```

## Getting items from a list

To get an element from the list use the function `list_get_by_index` passing as
parameters the list and the position of the element.

```c
int element = list_get_by_index(l, 0);
```

## Print elements in a list

To print the elements of a list use the function `list_print` passing as
parameter the list.

```c
list_print(l);
```

## Destroying a list

To destroy a list use the function `ll_delete` passing as parameter
the list. This function uses `free` to deallocate the memory of the list.

```c
ll_delete(ll);
```

## Build

To build the project use the following command in the root directory
of the project.

    $ make

