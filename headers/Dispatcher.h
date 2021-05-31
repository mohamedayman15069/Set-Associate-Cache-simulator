#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include <includes.h>
#include <DirectMap.h>

class Dispatcher{

    private:
        FILE * f;
        vector <DirectMap *> * directMaps;
        uint64_t hits;
        uint64_t misses;
    public:
        Dispatcher(vector <DirectMap *> * p_directMaps,char * filename);
        bool dispatch ();
        ~Dispatcher();
};

#endif
