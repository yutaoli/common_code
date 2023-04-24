#ifndef LRU_H
#define LRU_H

//#include <bits/stdc++.h>
#include <list>
#include <unordered_map>
using namespace std;

class LRUCache {
public:
    // store keys of cache
    list<int> dq;
 
    // store references of key in cache
    unordered_map<int, list<int>::iterator> ma;
    int csize; // maximum capacity of cache
 
public:
    LRUCache(int);
    void refer(int);
    void display();
};

#endif
