// 单链表：/usr/include/c++/4.8.5/forward_list

#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include<stddef.h>

struct singly_linked_list_node
{
    int val;
    struct singly_linked_list_node *next;
};

class singly_linked_list
{
    public:
        singly_linked_list();
        virtual ~ singly_linked_list();

    public:
        struct singly_linked_list_node *front();
        void push_front(int val);
        void pop_front();

        // insert after position, return inserted last element position
        struct singly_linked_list_node *insert_after(struct singly_linked_list_node *position, int val);
        // erase element after position, return next element of position, if no next element of position, return NULL
        struct singly_linked_list_node *erase_after(struct singly_linked_list_node *position);

        void reverse();

    private:
        struct singly_linked_list_node *head;
};

#endif
