#include <iostream>
#include "lru.h"

// Declare the size
LRUCache::LRUCache(int n) { csize = n; }
 
// Refers key x with in the LRU cache
void LRUCache::refer(int x)
{
    // not present in cache
    if (ma.find(x) == ma.end()) {
        // cache is full
        if (dq.size() == (size_t)csize) {
            // delete least recently used element
            int last = dq.back();
 
            // Pops the last element
            dq.pop_back();
 
            // Erase the last
            ma.erase(last);
        }
    }
 
    // present in cache
    else
        dq.erase(ma[x]);
 
    // update reference
    dq.push_front(x);
    ma[x] = dq.begin();
}
 
// Function to display contents of cache
void LRUCache::display()
{
 
    // Iterate in the deque and print
    // all the elements in it
    for (auto it = dq.begin(); it != dq.end(); it++)
        cout << (*it) << " ";
 
    cout << endl;
}
