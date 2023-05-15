#include <vector>
#include <stdio.h>
#include <string>
#include <thread>
#include <pthread.h>
#include <mutex>
#include <shared_mutex>
#include <numeric>
//#include <stdint.h>
#include <atomic>
//#include <threads.h>

class DistributedCounter {
public:
typedef long long value_type;
static std::string name() { return "locked counter"; };
private:
value_type count;
std::shared_mutex mutable mtx;
public:
DistributedCounter() : count(0) {}
void operator++() {
std::unique_lock lock(mtx);
++count;
}
value_type get() const {
std::shared_lock lock(mtx);
return count;
}
};
/*
Running the counter in a program with one hundred threads each counting to ten
million takes 113 seconds
• Even though the computer has 128 cores
• We probably aren't getting much parallelism because they are all taking turns to get the
lock each time they increment the counter
• We also are probably getting bad memory performance because the different
processors need to update main memory after each increment
• Can we do better?
*/

class DistributedCounterLockedArray {
typedef size_t value_type;
struct bucket {
std::shared_mutex sm;
value_type count;
};
static size_t const buckets{ 128 };
std::vector<bucket> counts{ buckets };
public:
static std::string name() { return "locked array"; };
void operator++() {
size_t index = std::hash<std::thread::id>()(std::this_thread::get_id()) % buckets;
std::unique_lock ul(counts[index].sm);
counts[index].count++;
}
value_type get() {
return std::accumulate(counts.begin(), counts.end(), (value_type)0,
[](auto acc, auto &x) { std::shared_lock sl(x.sm); return acc + x.count; });
}
};
/*
• Running the same test now takes 13.86 seconds
• Ten times faster!
• Are we set?
• Let's think more about the cache
*/

/*
Third attempt: Locked padded array
• Remember when I mentioned that the cache system caches an entire cache line to take
advantage of locality?
• This means that when a processor caches a counter from the array, it will actually cache
several adjacent counters that belong to the same cache line
• As soon as it modifies its own counter, the entire cache line enters a modified state,
which means that any other counters in the cache line that are being used by other
threads need to be resync'ed with main memory in order to avoid cache coherency
problems
• This is called a "false sharing" problem where objects that aren't shared in the code still
are connected in the cache
• To address this, let's pad the individual counters in the array to 256 bytes, which will
certainly be longer than a cache line

*/
class DistributedCounterLockedPaddedArray {
typedef size_t value_type;
struct bucket {
std::shared_mutex sm;
value_type count;
char padding[256];
};
static size_t const buckets{ 128 };
std::vector<bucket> counts{ buckets };
public:
static std::string name() { return "locked padded array"; };
void operator++() {
size_t index = std::hash<std::thread::id>()(std::this_thread::get_id()) % buckets;
std::unique_lock ul(counts[index].sm);
counts[index].count++;
}
value_type get() {
return std::accumulate(counts.begin(), counts.end(), (value_type)0,
[](auto acc, auto &x) { std::shared_lock sl(x.sm); return acc + x.count; });
}
};
/*
• 6.31s
• Eliminating the false sharing doubled the performance from the last version!
• Can we do better?
*/

/*
Fourth attempt: Atomic padded array
• Locks are not the only way to avoid race conditions
• C++ also includes atomics
• The idea is that computer hardware often has built in operations like increment that can
be done "atomically" (i.e., without a race condition) without need for acquiring a lock in
the program code
• Let's modify the previous code to use atomics
*/
class DistributedCounterAtomicPaddedArray {
typedef size_t value_type;
struct bucket {
std::atomic<value_type> count;
char padding[256];
};
static size_t const buckets{ 128 };
std::vector<bucket> counts{ buckets };
public:
static std::string name() { return "locked padded array"; };
void operator++() {
size_t index = std::hash<std::thread::id>()(std::this_thread::get_id()) % buckets;
counts[index].count++;
}
value_type get() {
return std::accumulate(counts.begin(), counts.end(), (value_type)0,
[](auto acc, auto &x) {return acc + x.count.load(); });
}
};
/*
Atomic padded array performance
• Now our benchmark takes only 1.15s
• About one hundred times faster than our original version!
• This is a good counter implementation
*/

/*
Cache-conscious programming
(Adapted from Herlihy&Shavit p. 477)
• Objects or fields that are accessed independently should be aligned and padded so they end
up on different cache lines.
• Keep read-only data separate from data that is modified frequently.
• When possible, split an object into thread-local pieces. For example, a counter used for
statistics could be split into an array of counters, one per thread, each one residing on a
different cache line. While a shared counter would cause invalidation traffic, the split counter
allows each thread to update its own replica without causing cache coherence traffic.
• If a lock protects data that is frequently modified, then keep the lock and the data on distinct
cache lines, so that threads trying to acquire the lock do not interfere with the lock-holder’s
access to the data.
• If a lock protects data that is frequently uncontended, then try to keep the lock and the data
on the same cache lines, so that acquiring the lock will also load some of the data into the
cache.
• If a class or struct contains a large chunk of data whose size is divisible by a high power of
two, consider separating it out of the class and holding it with an unique_ptr to avoid the
Ghostscript problem from above
• Use a profiling tool like VTune to identify where your cache bottlenecks are

Portable cache-conscious programming
The rules for cache-conscious programming, had a lot of phrases like“put on the
same/different cache line"
But how do we know whether two objects are on the same or different cache lines?
How do we do that in C++?
We could make an educated guess, like we did with adding 256 bytes to our padded
counter
• However, such “magic numbers” are always brittle
C++17 introduced a portable approach
• If you keep objects further than std::hardware_destructive_interference_size, they
“should” (i.e., implementation-defined) end up on different cache lines
• If you keep objects closer than std::hardware_constructive_interference_size, they
“should” end up on the same cache line
*/

DistributedCounter counter;
DistributedCounterLockedArray locked_array;
DistributedCounterLockedPaddedArray locked_padded_array;
DistributedCounterAtomicPaddedArray atomic_padded_array;
	
const int N = 1000000;

typedef struct timediff
{
    timediff(std::string prompt_)
    {   
        prompt = prompt_;
        begin.tv_sec = 0;
        begin.tv_nsec = 0;
        clock_gettime(CLOCK_REALTIME, &begin);
    }   

    ~ timediff()
    {   
        end.tv_sec = 0;
        end.tv_nsec = 0;
        clock_gettime(CLOCK_REALTIME, &end);

        if(end.tv_nsec > begin.tv_nsec)
        {   
            printf("%s,timediff:sec:%lu,nsec:%lu\n",prompt.c_str(),end.tv_sec-begin.tv_sec, end.tv_nsec- begin.tv_nsec);
        }   
        else 
        {   
            printf("%s,timediff:sec:%lu,nsec:%lu\n",prompt.c_str(),end.tv_sec-begin.tv_sec-1, 1000000000UL + end.tv_nsec- begin.tv_nsec);
        }   
        printf("\n");
    } 
	struct timespec begin;
    struct timespec end;
    std::string prompt;
    /*
    uint64_t getnowtime64()
    {
        struct timespec now={0,0};
        clock_gettime(CLOCK_REALTIME, &now);
        return now.tv_sec << 32 | (now.tv_nsec & 0xffffffff);
    }
    */
}timediff_t;


void* test_func_counter(void* input)
{
	for(int i = 0; i < N; i++)
	{
		++counter;
	}

    return NULL;
}

void test_counter()
{
	timediff_t timdiff(__FUNCTION__);
	
	pthread_t threads[10];
    for(int i = 0; i < 10; i++)
        pthread_create(&threads[i], NULL, test_func_counter, NULL);
    
    for(int i = 0; i < 10; i++)
        pthread_join(threads[i], NULL);
	
	printf("counter is: %lu\n", counter.get());
}

void* test_func_locked_array(void* input)
{
	for(int i = 0; i < N; i++)
	{
		++locked_array;
	}

    return NULL;
}

void test_locked_array()
{
	timediff_t timdiff(__FUNCTION__);
	
	pthread_t threads[10];
    for(int i = 0; i < 10; i++)
        pthread_create(&threads[i], NULL, test_func_locked_array, NULL);
    
    for(int i = 0; i < 10; i++)
        pthread_join(threads[i], NULL);
	
	printf("locked_array counter is: %lu\n", locked_array.get());
}

void* test_func_locked_padded_array(void* input)
{
	for(int i = 0; i < N; i++)
	{
		++locked_padded_array;
	}

    return NULL;
}

void test_locked_padded_array()
{
	timediff_t timdiff(__FUNCTION__);
	
	pthread_t threads[10];
    for(int i = 0; i < 10; i++)
        pthread_create(&threads[i], NULL, test_func_locked_padded_array, NULL);
    
    for(int i = 0; i < 10; i++)
        pthread_join(threads[i], NULL);
	
	printf("locked_padded_array counter is: %lu\n", locked_padded_array.get());
}

void* test_func_atomic_padded_array(void* input)
{
	for(int i = 0; i < N; i++)
	{
		++atomic_padded_array;
	}

    return NULL;
}

void test_atomic_padded_array()
{
	timediff_t timdiff(__FUNCTION__);
	
    pthread_t threads[10];
    for(int i = 0; i < 10; i++)
        pthread_create(&threads[i], NULL, test_func_atomic_padded_array, NULL);
    
    for(int i = 0; i < 10; i++)
        pthread_join(threads[i], NULL);
	
	printf("atomic_padded_array counter is: %lu\n", atomic_padded_array.get());
}

int main()
{
	test_counter();
	test_locked_array();
	test_locked_padded_array();
	test_atomic_padded_array();
}
/*
 [root@ecs-KwUzX demo]# ./cache_line 
 counter is: 10000000
 test_counter,timediff:sec:3,nsec:442594063

 locked_array counter is: 10000000
 test_locked_array,timediff:sec:2,nsec:291894377

 locked_padded_array counter is: 10000000
 test_locked_padded_array,timediff:sec:2,nsec:293857149

 atomic_padded_array counter is: 10000000
 test_atomic_padded_array,timediff:sec:0,nsec:805975962
 */
