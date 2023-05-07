#include <CppUTest/CommandLineTestRunner.h>
#include "smart_ptr_shared_ptr_weak_ptr.hpp"

struct Node {
    WeakPtr<Node> next;
    int value;

    static int new_node_cnt;

    Node()
    {
        new_node_cnt++;
    }
    virtual ~ Node()
    {
        //printf("%s\n", __FUNCTION__);
        new_node_cnt--;
    }
};
int Node::new_node_cnt = 0;

TEST_GROUP(smart_ptr_shared_ptr_weak_ptr)
{
    void setup()
    {
    }
    void teardown()
    {
    }
};

TEST(smart_ptr_shared_ptr_weak_ptr, test_normal)
{
    {
        SharePtr<Node> node1(new Node{});
        SharePtr<Node> node2(new Node{});
        node1->next = node2;
        node2->next = node1;

        CHECK(Node::new_node_cnt == 2);
        CHECK(Counter::new_counter_cnt == 2);
    }
    CHECK(Node::new_node_cnt == 0);
    CHECK(Counter::new_counter_cnt == 0);
}
