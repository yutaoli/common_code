#include "singly_linked_list.h"

singly_linked_list::singly_linked_list()
{
    head = NULL;
}

singly_linked_list::~ singly_linked_list()
{
    if(head == NULL) return;

    struct singly_linked_list_node *temp = head;
    while(temp != NULL)
    {
        head = head->next;

        delete temp;

        temp = head;
    }
}

struct singly_linked_list_node *singly_linked_list::front()
{
    return head;
}

void singly_linked_list::push_front(int val)
{
    struct singly_linked_list_node * new_node = new singly_linked_list_node;
    if(head == NULL)
    {
        new_node->val  = val;
        new_node->next = NULL;
        head = new_node;
        return;
    }

    new_node->val = val;
    new_node->next = head;
    head = new_node;
}

void singly_linked_list::pop_front()
{
    if(head == NULL) return;

    struct singly_linked_list_node * temp = head;
    head = head->next;
    delete temp;
}

struct singly_linked_list_node *singly_linked_list::insert_after(struct singly_linked_list_node *position, int val)
{
    struct singly_linked_list_node * new_node = new singly_linked_list_node;
    if(position == NULL)
    {   
        new_node->val = val;
        new_node->next = NULL;
        head = new_node;
        return head;
    }

    new_node->val = val;
    new_node->next = position->next;
    position->next = new_node;
    return new_node;
}

struct singly_linked_list_node *singly_linked_list::erase_after(struct singly_linked_list_node *position)
{
    if(position == NULL || position->next == NULL) return NULL;

    // delete position->next
    struct singly_linked_list_node * temp = position->next;
    position->next = temp->next;
    delete temp;

    return position->next;
}

void singly_linked_list::reverse()
{
    if(head == NULL) return ;

    struct singly_linked_list_node * pre = NULL;
    struct singly_linked_list_node * cur = head;
    struct singly_linked_list_node * next = NULL;
    while(cur != NULL)
    {
        next = cur->next;
        cur->next = pre;
        pre = cur;
        cur = next;
    }
    head = pre;
}
