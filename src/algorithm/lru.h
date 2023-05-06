#ifndef LRU_H
#define LRU_H

//#include <bits/stdc++.h>
#include <list>
#include <map>

class lru
{
    public:
        lru(int size_);
        virtual ~ lru();

        // 如果没有，返回-1
        int get(int key);
        void set(int key, int value);
        void display();

        std::list<std::pair<int, int> > queue;
        std::map<int, std::list<std::pair<int, int> >::iterator> key2it;
        int size;
};

#endif
