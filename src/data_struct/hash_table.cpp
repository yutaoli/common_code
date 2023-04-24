#include<bits/stdc++.h>
#include "hash_table.h"
using namespace std;
 
Hash::Hash(int b)
{
    this->BUCKET = b;
    table = new list<pair<int,int> >[BUCKET];
}

Hash::~Hash()
{
    if(table != NULL)
    {
        delete []table;
    }
}
 
void Hash::insert(pair<int,int> kv)
{
    list<pair<int,int> >::iterator kv_it;
    bool find = get(kv.first, kv_it);
    if(find == true)
    {
        kv_it->second = kv.second;
        return;
    }

    int index = hashFunction(kv.first);
    table[index].push_back(kv);
}
 
bool Hash::get(int k, list<pair<int,int> >::iterator &kv)
{
    int index = hashFunction(k);
    list<pair<int,int> >::iterator it;
    for(it = table[index].begin();
            it != table[index].end();it++)
    {
        if(it->first == k)
        {
            kv = it;
            return true;
        }
    }
    return false;
}

void Hash::remove(int k)
{
  // get the hash index of key
  int index = hashFunction(k);
 
  // find the key in (index)th list
  list <pair<int,int> > :: iterator it;
  for (it = table[index].begin();
           it != table[index].end(); it++) 
  {
    if (it->first == k)
      break;
  }
 
  // if key is found in hash table, remove it
  if (it != table[index].end())
    table[index].erase(it);
}
 
// function to display hash table
void Hash::displayHash() {
  for (int i = 0; i < BUCKET; i++) {
    cout << i;
    for (auto x : table[i])
      cout << " --> (" << x.first << "," << x.second << ")";
    cout << endl;
  }
}
