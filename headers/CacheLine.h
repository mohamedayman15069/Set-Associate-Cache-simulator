#ifndef CACHELINE_H_
#define CACHELINE_H_

#include <includes.h>
#define READ_ACCESS 'R'
#define WRITE_ACCESS 'W'


#define NO_STATUS       0
#define CL_NOT_VALID    1
#define TAG_MATCH       2
#define TAG_MISMATCH    3

class CacheStats
{
    private:
        uint64_t hits;
        uint64_t misses;
        uint64_t cache_reads;
        uint64_t cache_writes;
        uint64_t mem_reads;
        uint64_t mem_writes;
    public:
        CacheStats();
        void incHits();
        void incMisses();
        void incCacheReads();
        void incCacheWrites();
        void incMemReads();
        void incMemWrites();
        void printStats (unsigned int crc,unsigned int cwc ,unsigned int mrc ,unsigned int mwc);
        ~CacheStats();
};

class CacheLine
{

    private:
        bool wb;
        bool v;
        bool M; 
        uint64_t counter;
        uint64_t tag;
        CacheStats * cacheStats;
        void write_through(); // Also this will increment the mem_writes. ( After modifying write in cache then memory one go)
        void write_back(); // This is to increment the mem_writes (modify on the cache what you need; then go to the memory and modify it (as a bulk). 
        void handle_miss (uint64_t p_tag);
    public:
        CacheLine (CacheStats * p_cacheStats,bool _wb=true);
        bool valid();
        uint64_t getTag();
        uint64_t getCounter();
        void access (uint64_t p_tag,char mode);
        unsigned short check (uint64_t p_tag);
        void updateCounter (bool access);
        ~CacheLine();
};

#endif // CACHELINE_H_
