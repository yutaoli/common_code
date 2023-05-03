#include <CppUTest/CommandLineTestRunner.h>
#include <stdio.h>
#include "singly_linked_list.h"

TEST_GROUP(singly_linked_list)
{
    void setup()
    {
    }
    void teardown()
    {
    }

    void display(singly_linked_list &l)
    {
        struct singly_linked_list_node* temp = l.front();

        printf("\nTraversal in forward direction \n");
        while (temp != NULL) 
        {
            printf("%d ", temp->val);
            temp = temp->next;
        }
    }
};

TEST(singly_linked_list, test_NULL)
{
    singly_linked_list l;
    struct singly_linked_list_node* start = l.front();
    CHECK(start == NULL);
}

TEST(singly_linked_list, test_normal)
{
    singly_linked_list l;
    l.push_front(8);

    l.push_front(7);

    l.push_front(6);

    l.push_front(5);

    //display(l);

    struct singly_linked_list_node *start = l.front();
    CHECK(start->val == 5);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 6);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 7);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 8);
    l.pop_front();

    start = l.front();
    CHECK(start == NULL);
}

TEST(singly_linked_list, test_insert_after_begin)
{
    singly_linked_list l;
    struct singly_linked_list_node *after = l.insert_after(l.front(), 5);// 5
    //display(l);
    CHECK(after->val == 5);
}

TEST(singly_linked_list, test_insert_after_middle)
{
    singly_linked_list l;
    l.push_front(8);

    l.push_front(7);

    l.push_front(6);

    l.push_front(5);

    //display(l);

    struct singly_linked_list_node *start = l.front();
    start = start->next;// 5 6 7 8
                        //   ^
    struct singly_linked_list_node *after = NULL;
    after = l.insert_after(start, 10);// 5 6 10 7 8 
                                      //      ^
    CHECK(after->val == 10);
    //display(l);

    start = l.front();
    CHECK(start->val == 5);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 6);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 10);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 7);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 8);
    l.pop_front();

    start = l.front();
    CHECK(start == NULL);
}

TEST(singly_linked_list, test_insert_after_end)
{
    singly_linked_list l;
    l.push_front(8);

    l.push_front(7);

    l.push_front(6);

    l.push_front(5);

    //display(l);

    struct singly_linked_list_node *cur = l.front();
    struct singly_linked_list_node *next = cur->next;
    while(next != NULL)
    {
        cur = next;
        next = next->next;
    }
    CHECK(cur->val == 8);// 5 6 7 8
                         //       ^

    struct singly_linked_list_node *after = NULL;
    after = l.insert_after(cur, 10);// 5 6 7 8 10
                                    //          ^
    CHECK(after->val == 10);
    //display(l);

    struct singly_linked_list_node *start = NULL;
    start = l.front();
    CHECK(start->val == 5);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 6);
    l.pop_front();


    start = l.front();
    CHECK(start->val == 7);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 8);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 10);
    l.pop_front();

    start = l.front();
    CHECK(start == NULL);
}

TEST(singly_linked_list, test_erase_after_empty)
{
    singly_linked_list l;
    struct singly_linked_list_node *after = l.erase_after(l.front());
    //display(l);
    CHECK(after == NULL);
}

TEST(singly_linked_list, test_erase_after_begin)
{
    singly_linked_list l;
    l.push_front(8);

    l.push_front(7);

    l.push_front(6);

    l.push_front(5);

    //display(l);
    // 5 6 7 8

    struct singly_linked_list_node *after = l.erase_after(l.front());// 5 6 7 8
                                                                     //   ^
                                                                     // 5 7 8
                                                                     //   ^
    //display(l);
    CHECK(after->val == 7);
}

TEST(singly_linked_list, test_erase_after_middle)
{
    singly_linked_list l;
    l.push_front(8);

    l.push_front(7);

    l.push_front(6);

    l.push_front(5);
    //display(l);

    struct singly_linked_list_node *after = l.erase_after(l.front()->next);// 5 6 7 8
                                                                           //     ^
                                                                           // 5 6 8
                                                                           //     ^
    //display(l);
    CHECK(after->val == 8);
}

TEST(singly_linked_list, test_erase_after_end)
{
    singly_linked_list l;
    l.push_front(8);

    l.push_front(7);

    l.push_front(6);

    l.push_front(5);

    //display(l);

    struct singly_linked_list_node *cur = l.front();
    struct singly_linked_list_node *next = cur->next;
    while(next != NULL)
    {
        cur = next;
        next = next->next;
    }
    CHECK(cur->val == 8);// 5 6 7 8
                         //       ^

    struct singly_linked_list_node *after = NULL;
    after = l.erase_after(cur);// 5 6 7 8
                               //       ^
    CHECK(after == NULL);
    //display(l);
}

TEST(singly_linked_list, test_reverse)
{
    singly_linked_list l;
    l.push_front(8);

    l.push_front(7);

    l.push_front(6);

    l.push_front(5);

    //display(l);
    l.reverse();
    //display(l);

    struct singly_linked_list_node *start = l.front();
    CHECK(start->val == 8);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 7);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 6);
    l.pop_front();

    start = l.front();
    CHECK(start->val == 5);
    l.pop_front();

    start = l.front();
    CHECK(start == NULL);
}
