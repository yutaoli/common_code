// 双向链表: /usr/src/kernels/3.10.0-1127.19.1.el7.x86_64/include/linux/list.h
// stl: /usr/include/c++/4.8.5/list

#ifndef LIST_H
#define LIST_H

struct list_node
{
    int val;
    struct list_node *prev;
    struct list_node *next;
};

class list
{
public:
    list();
    virtual ~ list();
public:
    void push_back(int val);
    void push_front(int val);

    void pop_back();
    void pop_front();

    struct list_node * front();
    struct list_node * back();

private:
    struct list_node *head;
};
#endif
