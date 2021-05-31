#include <DirectMap.h>
#include <Utils.h>

DirectMapProbe::DirectMapProbe (){
    directMap = NULL;
    counter = 0;
    status = NO_STATUS;
}
void DirectMapProbe::down(){
    lock.lock();
}
void DirectMapProbe::up ()
{
    lock.unlock();
}
void DirectMapProbe::setDirectMap(DirectMap * p_directMap)
{
    directMap = p_directMap;
}
void DirectMapProbe::setCounter (uint64_t p_counter)
{
    counter = p_counter;
}
void DirectMapProbe::setStatus (unsigned short p_status)
{
    status = p_status;
}

void DirectMapProbe::setCacheLineIndex(uint16_t p_cache_line_index)
{
    cache_line_index = p_cache_line_index;
}


DirectMap * DirectMapProbe::getDirectMap(){
    return directMap;
}
uint64_t DirectMapProbe::getCounter ()
{
    return counter;
}
unsigned short DirectMapProbe::getStatus()
{
    return status;
}

uint16_t DirectMapProbe::getCacheLineIndex ()
{
    return cache_line_index;
}

DirectMapProbe::~DirectMapProbe (){

}


DirectMap::DirectMap (CacheStats * p_cacheStats,uint16_t p_cache_line_size , uint16_t p_cache_size, bool _wb)// Constructor to initilaize everything. 
{ 
    cache_line_size = p_cache_line_size;
    cache_size = p_cache_size;
    for ( uint16_t i = 0 ; i < cache_size ; i++)
        cache[i] = new CacheLine(p_cacheStats,_wb);
    ls_bits = Utils::get_first_one_pos(cache_line_size)-1;
    cs_bits = Utils::get_first_one_pos(cache_size)-1;
}
void DirectMap::access (uint64_t full_address,char mode)// To get the index and tag to access the cache line.
{
    uint16_t index = Utils::extract_bits(full_address,cs_bits,ls_bits);
    uint64_t tag = Utils::extract_bits(full_address,64-(cs_bits+ls_bits),cs_bits+ls_bits);
    cache[index]->access(tag,mode);

}
void DirectMap::check (uint64_t full_address,DirectMapProbe * p_directMapProbe)
{
    uint16_t index = Utils::extract_bits(full_address,cs_bits,ls_bits);
    uint64_t tag = Utils::extract_bits(full_address,64-(cs_bits+ls_bits),cs_bits+ls_bits);
    p_directMapProbe->down();
    unsigned short check = cache[index]->check(tag);
    
   // Write your code here; 

if(check == TAG_MATCH) // If the tag matches, I am done.
{
        p_directMapProbe->setCounter(cache[index]->getCounter());
        p_directMapProbe->setDirectMap(this);
        p_directMapProbe->setCacheLineIndex(index);
        p_directMapProbe->setStatus(check);
}
else if(check == TAG_MISMATCH) // If it is mismatching, I will need to see whether the p_directMapProbe is NULL or not 
{   // Now, what if there is a mismatch? I will need to access p_directMap_probe as well.
    if(p_directMapProbe== NULL || p_directMapProbe->getStatus() == TAG_MISMATCH || p_directMapProbe->getStatus() ==0)
    {
        if(cache[index]->getCounter() < p_directMapProbe->getCounter()) // To check the aging ( if the probe is older, then set its values) 
      { p_directMapProbe->setCounter(cache[index]->getCounter());
        p_directMapProbe->setDirectMap(this);
        p_directMapProbe->setCacheLineIndex(index);
        p_directMapProbe->setStatus(check);
      }
    }
}

else if(check == CL_NOT_VALID) // If not valid, I will have rubbish data. 
{       // If it is mismatch or status = 0 or probe is NULL -> put the values. 
    if(p_directMapProbe== NULL || !(p_directMapProbe->getStatus() == TAG_MATCH))
    {   p_directMapProbe->setCounter(cache[index]->getCounter());
        p_directMapProbe->setDirectMap(this);
        p_directMapProbe->setCacheLineIndex(index);
        p_directMapProbe->setStatus(check);
        
    }
}


    p_directMapProbe->up();
}
void DirectMap::updateCounter (uint16_t index,bool access)
{
    cache[index]->updateCounter(access);
}
uint64_t DirectMap::getCounter (uint16_t index)
{
    return cache[index]->getCounter();

}

DirectMap::~DirectMap()
{
    for ( uint16_t i = 0 ; i < cache_size ; i++)
        delete (cache[i]);
}



