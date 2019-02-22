#include "unity.h"
#include "Linked_list.h"

static list_t l;

void
setUp(void)
{
    
}

void
tearDown(void)
{
    list_destroy(&l);
}

void
test_LinkedList_should_PushAtBack(void)
{
    const int16_t data[] = {10, 20, 30, 40, 50};
    int16_t retval;
    uint8_t error;

    list_init(&l, sizeof(int16_t));

    list_push(&l, (void *) &data[0]);
    list_push(&l, (void *) &data[1]);
    list_push(&l, (void *) &data[2]);
    list_push(&l, (void *) &data[3]);
    list_push(&l, (void *) &data[4]);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(50, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(40, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(30, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(20, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(10, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);
}

void
test_LinkedList_should_PushAtFront(void)
{
    const int16_t data[] = {50, 40, 30, 20, 10};
    int16_t retval;
    uint8_t error;

    list_init(&l, sizeof(int16_t));

    list_push_front(&l, (void *) &data[0]);
    list_push_front(&l, (void *) &data[1]);
    list_push_front(&l, (void *) &data[2]);
    list_push_front(&l, (void *) &data[3]);
    list_push_front(&l, (void *) &data[4]);

    error = list_pop_front(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(10, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop_front(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(20, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop_front(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(30, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop_front(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(40, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop_front(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(50, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);
}

void
test_LinkedList_should_GetElements(void)
{
    const int16_t data[] = {10, 20, 30};
    int16_t retval;
    uint8_t error;

    list_init(&l, sizeof(int16_t));

    list_push(&l, (void *) &data[0]);
    list_push(&l, (void *) &data[1]);
    list_push(&l, (void *) &data[2]);

    error = list_get_by_index(&l, 0, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(10, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_get_by_index(&l, 1, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(20, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_get_by_index(&l, 2, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(30, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);
    
    error = list_get_by_index(&l, 3, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(30, retval);
    TEST_ASSERT_EQUAL_UINT8(1, error);
}

void
test_LinkedList_should_ReturnErrorIfListIsEmptyUsingFront(void)
{
    const int16_t data[] = {10, 20, 30};
    int16_t retval;
    uint8_t error;

    list_init(&l, sizeof(int16_t));

    list_push(&l, (void *) &data[0]);
    list_push(&l, (void *) &data[1]);
    list_push(&l, (void *) &data[2]);

    error = list_pop_front(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(10, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop_front(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(20, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop_front(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(30, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop_front(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(30, retval);
    TEST_ASSERT_EQUAL_UINT8(1, error);
}

void
test_LinkedList_should_ReturnErrorIfListIsEmptyUsingBack(void)
{
    const int16_t data[] = {10, 20, 30};
    int16_t retval;
    uint8_t error;

    list_init(&l, sizeof(int16_t));

    list_push(&l, (void *) &data[0]);
    list_push(&l, (void *) &data[1]);
    list_push(&l, (void *) &data[2]);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(30, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(20, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(10, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(10, retval);
    TEST_ASSERT_EQUAL_UINT8(1, error);
}

void
test_LinkedList_should_BehaveAsFIFO(void)
{
    const int16_t data[] = {10, 20, 30};
    int16_t retval;
    uint8_t error;

    list_init(&l, sizeof(int16_t));

    list_push(&l, (void *) &data[0]);
    list_push(&l, (void *) &data[1]);
    list_push(&l, (void *) &data[2]);

    error = list_pop_front(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(10, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop_front(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(20, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop_front(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(30, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);
}

void
test_LinkedList_should_BehaveAsLIFO(void)
{
    const int16_t data[] = {10, 20, 30};
    int16_t retval;
    uint8_t error;

    list_init(&l, sizeof(int16_t));

    list_push(&l, (void *) &data[0]);
    list_push(&l, (void *) &data[1]);
    list_push(&l, (void *) &data[2]);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(30, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(20, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_INT16(10, retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);
}

void
test_LinkedList_should_WorkWithStrings(void)
{
    const char *str[] = {
        "0x0001",
        "0x0002",
        "0x0003",
    };
    char retval[7];
    uint8_t error;

    list_init(&l, strlen(str[0]));

    list_push(&l, (void *) str[0]);
    list_push(&l, (void *) str[1]);
    list_push(&l, (void *) str[2]);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_STRING("0x0003", retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_STRING("0x0002", retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);

    error = list_pop(&l, (void *) &retval);
    TEST_ASSERT_EQUAL_STRING("0x0001", retval);
    TEST_ASSERT_EQUAL_UINT8(0, error);
}

void sum(const void* data, void* arg)
{
    *(int16_t *) arg += *(int16_t *) data;
}

void
test_LinkedList_should_IterateElements(void)
{
    const int16_t data[] = {10, 20, 30};
    int16_t retval = 0;
    uint8_t error;

    list_init(&l, sizeof(int16_t));

    list_push(&l, (void *) &data[0]);
    list_push(&l, (void *) &data[1]);
    list_push(&l, (void *) &data[2]);

    list_for_each(&l, sum, (void*) &retval);
    TEST_ASSERT_EQUAL_INT16(60, retval);
}

int
main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_LinkedList_should_PushAtBack);
    RUN_TEST(test_LinkedList_should_PushAtFront);
    RUN_TEST(test_LinkedList_should_GetElements);
    RUN_TEST(test_LinkedList_should_ReturnErrorIfListIsEmptyUsingFront);
    RUN_TEST(test_LinkedList_should_ReturnErrorIfListIsEmptyUsingBack);
    RUN_TEST(test_LinkedList_should_BehaveAsFIFO);
    RUN_TEST(test_LinkedList_should_BehaveAsLIFO);
    RUN_TEST(test_LinkedList_should_WorkWithStrings);
    RUN_TEST(test_LinkedList_should_IterateElements);
    return UNITY_END();
}