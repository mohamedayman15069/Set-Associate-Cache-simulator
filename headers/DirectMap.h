#ifndef DIRECTMAP_H_
#define DIRECTMAP_H_

#include <includes.h>
#include <CacheLine.h>


class DirectMapProbe;
class DirectMap
{
    private:
        uint16_t cache_line_size;
        uint16_t cache_size;
        uint8_t cs_bits;
        uint8_t ls_bits;
        std::map <uint16_t,CacheLine *> cache;
    public:
        DirectMap (CacheStats * p_cacheStats,uint16_t p_cache_line_size , uint16_t p_cache_size, bool _wb=true);
        void access (uint64_t full_address,char mode);
        void check (uint64_t full_address,DirectMapProbe * p_directMapProbe);
        void updateCounter (uint16_t index,bool access);
        uint64_t getCounter (uint16_t index);
        ~DirectMap();
};

class DirectMapProbe
{
    private:
        mutex lock;
        DirectMap * directMap;
        uint64_t counter;
        unsigned short status;
        uint16_t cache_line_index;
    public:
        DirectMapProbe ();
        void down();
        void up ();
        void setDirectMap(DirectMap * p_directMap);
        void setCounter (uint64_t p_counter);
        void setStatus (unsigned short p_status);
        void setCacheLineIndex(uint16_t p_cache_line_index);
        DirectMap * getDirectMap();
        uint64_t getCounter ();
        unsigned short getStatus();
        uint16_t getCacheLineIndex ();
        ~DirectMapProbe ();
};




#endif // DIRECTMAP_H_