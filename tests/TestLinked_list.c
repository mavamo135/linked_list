#include "unity.h"
#include "Linked_list.h"

static list_t* l;

void
setUp(void)
{
    list_init(&l);
}

void
tearDown(void)
{
    list_destroy(l);
}

void
test_LinkedList_should_PushAtBack(void)
{
    list_push(l, 10);
    list_push(l, 20);
    list_push(l, 30);
    list_push(l, 40);
    list_push(l, 50);

    TEST_ASSERT_EQUAL_INT16(50, list_pop(l));
    TEST_ASSERT_EQUAL_INT16(40, list_pop(l));
    TEST_ASSERT_EQUAL_INT16(30, list_pop(l));
    TEST_ASSERT_EQUAL_INT16(20, list_pop(l));
    TEST_ASSERT_EQUAL_INT16(10, list_pop(l));
}

void
test_LinkedList_should_PushAtFront(void)
{
    list_push_front(l, 50);
    list_push_front(l, 40);
    list_push_front(l, 30);
    list_push_front(l, 20);
    list_push_front(l, 10);

    TEST_ASSERT_EQUAL_INT16(10, list_pop_front(l));
    TEST_ASSERT_EQUAL_INT16(20, list_pop_front(l));
    TEST_ASSERT_EQUAL_INT16(30, list_pop_front(l));
    TEST_ASSERT_EQUAL_INT16(40, list_pop_front(l));
    TEST_ASSERT_EQUAL_INT16(50, list_pop_front(l));
}

void
test_LinkedList_should_GetElements(void)
{
    list_push(l, 10);
    list_push(l, 20);
    list_push(l, 30);

    TEST_ASSERT_EQUAL_INT16(10, list_get_by_index(l, 0));
    TEST_ASSERT_EQUAL_INT16(20, list_get_by_index(l, 1));
    TEST_ASSERT_EQUAL_INT16(30, list_get_by_index(l, 2));
    TEST_ASSERT_EQUAL_INT16(-1, list_get_by_index(l, 3));
}

void
test_LinkedList_should_ReturnErrorIfListIsEmptyUsingFront(void)
{
    list_push(l, 10);
    list_push(l, 20);
    list_push(l, 30);

    TEST_ASSERT_EQUAL_INT16(10, list_pop_front(l));
    TEST_ASSERT_EQUAL_INT16(20, list_pop_front(l));
    TEST_ASSERT_EQUAL_INT16(30, list_pop_front(l));
    TEST_ASSERT_EQUAL_INT16(-1, list_pop_front(l));
}

void
test_LinkedList_should_ReturnErrorIfListIsEmptyUsingBack(void)
{
    list_push(l, 10);
    list_push(l, 20);
    list_push(l, 30);

    TEST_ASSERT_EQUAL_INT16(30, list_pop(l));
    TEST_ASSERT_EQUAL_INT16(20, list_pop(l));
    TEST_ASSERT_EQUAL_INT16(10, list_pop(l));
    TEST_ASSERT_EQUAL_INT16(-1, list_pop(l));
}

void
test_LinkedList_should_BehaveAsFIFO(void)
{
    list_push(l, 10);
    list_push(l, 20);
    list_push(l, 30);

    TEST_ASSERT_EQUAL_INT16(10, list_pop_front(l));
    TEST_ASSERT_EQUAL_INT16(20, list_pop_front(l));
    TEST_ASSERT_EQUAL_INT16(30, list_pop_front(l));
}

void
test_LinkedList_should_BehaveAsLIFO(void)
{
    list_push(l, 10);
    list_push(l, 20);
    list_push(l, 30);

    TEST_ASSERT_EQUAL_INT16(30, list_pop(l));
    TEST_ASSERT_EQUAL_INT16(20, list_pop(l));
    TEST_ASSERT_EQUAL_INT16(10, list_pop(l));
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
    return UNITY_END();
}