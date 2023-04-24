// /usr/src/kernels/3.10.0-1127.19.1.el7.x86_64/include/linux/hashtable.h
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <list>
using namespace std;

class Hash
{
    int BUCKET;    // No. of buckets
 
    // Pointer to an array containing buckets
    list<pair<int, int> > *table;
public:
    Hash(int V);  // Constructor
    virtual ~Hash();
 
    // inserts a key into hash table
    void insert(pair<int, int> kv);
 
    bool get(int k, list<pair<int,int> >::iterator &kv);

    // deletes a key from hash table
    void remove(int k);
 
    // hash function to map values to key
    int hashFunction(int x) {
        return (x % BUCKET);
    }
 
    void displayHash();
};

#endif

