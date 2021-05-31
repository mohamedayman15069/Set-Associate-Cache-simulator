#include <CacheLine.h>

#define LM1  0x8000000000000000

CacheStats::CacheStats(){
    hits = 0;
    misses = 0;
    cache_reads = 0;
    cache_writes = 0;
    mem_reads = 0;
    mem_writes = 0;
}
void CacheStats::incHits(){
    hits++;
}
void CacheStats::incMisses(){
    misses++;
}
void CacheStats::incCacheReads(){
    cache_reads++;
}
void CacheStats::incCacheWrites(){
    cache_writes++;
}
void CacheStats::incMemReads(){
    mem_reads++;
}
void CacheStats::incMemWrites(){
    mem_writes++;
}
void CacheStats::printStats (unsigned int crc,unsigned int cwc ,unsigned int mrc ,unsigned int mwc)
{
    printf ("Total Misses: %llu\n",misses);
    printf ("Total Hits: %llu\n",hits);
    printf ("Memory Read Access Attempts: %llu\n",cache_reads);
    printf ("Memory Write Access Attempts: %llu\n",cache_writes);
    printf ("Total Memory Access Attempts: %llu\n",cache_reads+cache_writes);
    printf ("Memory Read Access: %llu\n",mem_reads);
    printf ("Memory Write Access: %llu\n",mem_writes);
    printf ("Total # of cycles for cache read: %llu\n",cache_reads*crc);
    printf ("Total # of cycles for cache writes: %llu\n",cache_writes*cwc);
    printf ("Total # of cycles for cache access: %llu\n",(cache_reads*crc)+(cache_writes*cwc));
    printf ("Total # of cycles for memory read: %llu\n",mem_reads*mrc);
    printf ("Total # of cycles for memory writes: %llu\n",mem_writes*mwc);
    printf ("Total # of cycles for memory access: %llu\n",(mem_reads*mrc)+(mem_writes*mwc));
    printf ("Total # of cycles for memory/cache access: %llu\n",(cache_reads*crc)+(cache_writes*cwc)+(mem_reads*mrc)+(mem_writes*mwc));
}

CacheStats::~CacheStats(){

}


CacheLine::CacheLine (CacheStats * p_cacheStats,bool _wb)
{
    v = false;
    M = false; 
    counter = 0;
    tag =0;
    wb = _wb;
    cacheStats = p_cacheStats;
}
//********************************************The member Function Write Through************************************************//
void CacheLine::write_through()
{
    cacheStats->incMemWrites(); 
    // Then this data is not valid 
   // v = 0; 

}
//********************************************The member Function Write Back************************************************//
void CacheLine::write_back() // If the block has M = 1 , meaning that it is has been updated(will be one when we are dealing with access function), I can inc mem_writes. 
{
    if(M)
    {
    cacheStats->incMemWrites();  // inc the number of memory_writes. 
    }
    // Then this data has become not valid 
    v = 0; 

}
//*************************************************************************************************************************//
void CacheLine::handle_miss (uint64_t p_tag)
{
    //Write your code here

    // First, we need to check whether the block contains valid data or not.
   
        if(wb) // If it is true, then I can write_back; otherwise, it is always writing from the memory each time (write through)
        {
            // Now we can write_back 
            this->write_back(); 
        }
        // Now I need to fill this tag with previous one 
        tag = p_tag; 
        // Second I need to set the valid bit( to be 1 : to say that this data has valid data)
        v= true;
        // I have not modified anything, or I do not need to modify the data now 
        M = 0;  
        counter = 0; // New counter.

}


bool CacheLine::valid()
{
    return v;
}
uint64_t CacheLine::getTag()
{
    return tag;
}

uint64_t CacheLine::getCounter()
{
    return counter;
}
void CacheLine::access (uint64_t p_tag,char mode)
{
    //Write your code here
    // This function will simply inc the all members, see if there is any misses to be handled and so on.
    if(this->valid()&&this->getTag()== p_tag) // I need to know whether the p_tag it is the same this tag and it is valid.
    {
            // If so, I know now there is a hit.
           cacheStats->incHits(); 
        // I need to check Whether I am in 'R' or 'W'
        if(mode == READ_ACCESS)  // READ_ACCESS is a #define in CacheLine.cpp means 'R'
        {
            cacheStats->incCacheReads();
        }
        else if(mode == WRITE_ACCESS)// Otherwise, I am writing. 
        {
           cacheStats->incCacheWrites(); // Inc in the cache. 
            // Now I need to check whether it is a write_through or write_back
            if(wb==0)
                this->write_through(); // Update now to the memory.


            // I modified it so M will be set
            M = 1 ; 

        }

    }
    // If the previous tag != the same tag or is not valid, cache miss occurs
    else 
    {
        // First, handle the misses. 
        this->handle_miss(p_tag);
        cacheStats->incMisses(); 
        cacheStats->incMemReads(); 
        // The same secnario will happens as above but I will read from the memory to the all levels of the cache. 
         if(mode == READ_ACCESS)  // READ_ACCESS is a #define in CacheLine.cpp means 'R'
        {
            cacheStats->incCacheReads();
        }
        else if(mode == WRITE_ACCESS)// Otherwise, I am writing. 
        {
            cacheStats->incCacheWrites(); // Inc in the cache. 
            //cacheStats->incMemWrites(); // Memory as well. 
            // Now I need to check whether it is a write_through or write_back
            if(wb==0)
                this->write_through(); // Update now to the memory.


            // I modified it so M will be set
            M = 1; 

        }

    }
}
unsigned short  CacheLine::check (uint64_t p_tag)
{       // I will just need to check the validity and the matching between the tage with the other. 
    //Write your code here
    if(!this->valid())
        {
            return CL_NOT_VALID;
        }
    else 
        {
            if(this->getTag() == p_tag)
                return TAG_MATCH; 
            
            else if(this->getTag() != p_tag)
                return TAG_MISMATCH; 
        }
    

}
void CacheLine::updateCounter (bool access)
{
    //Write your code here
	//Aging algorithm
// I will need to counter ++; then, then ORing the counter of this block and shift right afterwards.
counter>>1; 
if(access)
{
    counter || LM1 ; 
}
}
CacheLine::~CacheLine()
{

}