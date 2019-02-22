#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Linked_list.h"

void printInt16(const void *data)
{
  printf("%d :: ", *(const int16_t *)data);
}

void *writeThread(void* arg)
{
    int16_t i;
    list_t* list = (list_t *) arg;

    for (i = 0; i <= 10; i++)
      {
          list_push(list, (void *) &i);
          sleep(1);
      }

    pthread_exit(NULL);
}

void *readThread(void* arg)
{
    uint8_t i;
    int16_t retval;
    list_t* list = (list_t *) arg;

    while (list_size(list) == 0)
      {
        sleep(1);
      }

    for (i = 0; i <= 10; i++)
      {
          list_pop_front(list, (void *) &retval);
          printf("%d <- ", retval);
          list_print(list, printInt16);
          sleep(2);
      }

    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    pthread_t writer;
    pthread_t reader;
    list_t list;

    list_init(&list, sizeof(int16_t));

    if (pthread_create(&writer, NULL, writeThread, (void *) &list))
      {
          printf("Error creating write thread.\n");
          exit(EXIT_FAILURE);
      }

    if (pthread_create(&reader, NULL, readThread, (void *) &list))
      {
          printf("Error creating read thread.\n");
          exit(EXIT_FAILURE);
      }

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    list_destroy(&list);

    exit(EXIT_SUCCESS);
}

