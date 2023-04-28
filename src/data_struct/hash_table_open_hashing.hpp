#pragma once   //防止头文件重复包含

#include <iostream>
#include <string>

template<typename KEY, typename VALUE>
class hash_table_open_hashing
{
    public:
        hash_table_open_hashing(double factor_ = 0.7, int size_ = 1 << 10):
            factor(factor_),
            used(0),
            size(size_)
    {
        kvs = new KV[size];       
        for(int i = 0; i < size;i++)
        {
            kvs[i].used = false;
        }
    }
        virtual ~ hash_table_open_hashing()
        {
            if(kvs != NULL)
            {
                delete [] kvs;
                kvs = NULL;
            }
        }

        int get_size()
        {
            return used;
        }
        int get_capacity()
        {
            return size;
        }

        VALUE &operator[](const KEY &k)
        {
            if (used > factor * size)
            {
                printf("%s,used:%d,factor:%lf,size:%d\n",__FUNCTION__,used,factor,size);
                rehash();
            }

            int hash_code = hashcode(k);

            while(kvs[hash_code].used == true
                    && kvs[hash_code].k != k)
            {
                hash_code++;
                if(hash_code == size) 
                {
                    hash_code = 0;
                }

            }

            if(kvs[hash_code].used == false)
            {
                kvs[hash_code].used = true;
                kvs[hash_code].k = k;
                used++;
                return kvs[hash_code].v;
            }
            return kvs[hash_code].v;
        }

    private:
        int hashcode(int k)
        {
            return k % size;
        }

        int hashcode(const std::string &k)
        {
            unsigned long hash = 5381;
            unsigned char c = 0;
            const char *str = k.c_str();
            while((c = *str++))
            {   
                hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
            }   

            return hash % size;
        }


        void rehash()
        {
           printf("before rehash,used:%d,size:%d\n",used,size);
           int temp_kvs_size = size;
           KV *temp_kvs = new KV[temp_kvs_size];
           for(int i = 0; i < size; i++)
           {
               temp_kvs[i] = kvs[i];
           }

           // resize
           delete []kvs;
           used = 0;
           size = 2 * size;
           kvs = new KV[size];
           for(int i = 0; i < size;i++)
           {
               kvs[i].used = false;
           }

           // 构建新的kvs
           for(int i =0; i < temp_kvs_size; i++)
           {
               if(temp_kvs[i].used == true)
               {
                   (*this)[temp_kvs[i].k] = temp_kvs[i].v;
               }
           }

           delete []temp_kvs;
           printf("after rehash,used:%d,size:%d\n",used,size);
        }

        struct KV
        {
            bool used = false;
            KEY k;
            VALUE v;
        };


    private:
        double factor;
        int used;
        int size;
        KV *kvs;
};
