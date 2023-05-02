#include "list.h"
#include <stdlib.h>
#include <stdio.h>

list::list()
{
    head = NULL;
}

list::~ list()
{
    if(head == NULL) return;

    struct list_node *cur = head;
    do
    {
        struct list_node *temp = cur;
        if(temp != NULL)
        {
            cur = cur->next;

            delete temp;
        }
    }
    while(cur != head);
}

void list::push_back(int val)
{
    if(head == NULL)
    {
        struct list_node *new_node = new struct list_node;
        new_node->val = val;
        new_node->prev = new_node->next = new_node;
        head = new_node;
        return;
    }

    // find last node
    struct list_node *last = head->prev;
    struct list_node *new_node = new struct list_node;
    new_node->val = val;
    new_node->prev = last;
    new_node->next = head;

    last->next = new_node;
    head->prev = new_node;
}

void list::push_front(int val)
{
    if(head == NULL)
    {
        struct list_node *new_node = new struct list_node;
        new_node->val = val;
        new_node->prev = new_node->next = new_node;
        head = new_node;
        return;
    }

    // find last node;
    struct list_node *last = head->prev;
    struct list_node *new_node = new struct list_node;
    new_node->val = val;
    new_node->prev = last;
    new_node->next = head;

    last->next = new_node;
    head->prev = new_node;

    // update head pointer
    head = new_node;
}

void list::pop_back()
{
    if(head == NULL)
    {
        return;
    }

    if(head == head->prev) 
    {
        delete head;
        head = NULL;
        return;
    }

    struct list_node *temp = head->prev;
    temp->prev->next = head;
    temp->next->prev = temp->prev;

    delete temp;
}

void list::pop_front()
{
    if(head == NULL) 
    {
        return;
    }

    if(head == head->prev)
    {
        delete head;
        head = NULL;
        return;
    }

    struct list_node *temp = head;
    temp->prev->next = head->next;
    temp->next->prev = temp->prev;
    head = head->next;

    delete temp;
}

struct list_node * list::front()
{
    return head;
}

struct list_node * list::back()
{
    if(head == NULL) return NULL;
    return head->prev;
}

