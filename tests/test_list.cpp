#include <CppUTest/CommandLineTestRunner.h>
#include <stdio.h>
#include "list.h"

TEST_GROUP(list)
{
    void setup()
    {
    }
    void teardown()
    {
    }

    void display(list &l)
    {
        struct list_node* start = l.front();
        struct list_node* temp = l.front();

        printf("\nTraversal in forward direction \n");
        while (temp->next != start) 
        {
            printf("%d ", temp->val);
            temp = temp->next;
        }
        printf("%d ", temp->val);

        printf("\nTraversal in reverse direction \n");
        struct list_node* last = start->prev;
        temp = last;
        while (temp->prev != last) 
        {
            printf("%d ", temp->val);
            temp = temp->prev;
        }
        printf("%d ", temp->val);
    }
};

TEST(list, test_NULL)
{
    list l;
    struct list_node *start = l.front();
    CHECK(start == NULL);
}

TEST(list, test_normal)
{
    list l;
    l.push_back(5);

    l.push_front(4);

    l.push_back(7);

    l.push_back(8);

    struct list_node *start = l.front();
    CHECK(start->val == 4);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 5);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 7);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 8);
    l.pop_front();

    start = l.front();
    CHECK(start == NULL);

    //display(l);
}

TEST(list, test_pop_back)
{
    list l;
    l.push_back(5);

    l.push_front(4);

    l.push_back(7);

    l.push_back(8);

    struct list_node *end = l.back();
    CHECK(end->val == 8);
    l.pop_back();

    end = l.back();
    CHECK(end->val == 7);
    l.pop_back();

    end = l.back();
    CHECK(end->val == 5);
    l.pop_back();

    end = l.back();
    CHECK(end->val == 4);
    l.pop_back();

    end = l.back();
    CHECK(end == NULL);
    //display(l);
}
