#include <stdio.h>
#include "lru.h"

lru::lru(int size_){size = size_;}
lru::~lru(){}

// 如果没有，返回-1
int lru::get(int key)
{
    std::map<int, std::list<std::pair<int, int> >::iterator>::iterator m_it = key2it.find(key);
    if(m_it == key2it.end())
    {
        return -1;
    }

    queue.push_front(*(m_it->second));
    queue.erase(m_it->second);
    key2it[key] = queue.begin();
    return queue.begin()->second;
}

void lru::set(int key, int value)
{
    std::map<int, std::list<std::pair<int, int> >::iterator>::iterator m_it = key2it.find(key);
    if(m_it == key2it.end())
    {
        if(queue.size() == (size_t)size)
        {
            std::pair<int,int> last = queue.back();
            queue.pop_back();
            key2it.erase(last.first);
            //return;
        }
    }
    else 
    {
        queue.erase(m_it->second);
    }

    std::pair<int, int> new_pair = std::make_pair(key,value);
    queue.push_front(new_pair);
    key2it[key] = queue.begin();
}

void lru::display()
{
    for(auto it = queue.begin(); it  != queue.end(); it++)
    {
        printf("%d ", it->first);
    }
    printf("\n");
}

