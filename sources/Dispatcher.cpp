#include <Dispatcher.h>

Dispatcher::Dispatcher(vector <DirectMap *> * p_directMaps,char * filename)
{
    directMaps = p_directMaps;
    f = fopen (filename,"r");
    if ( f == NULL )
    {
        printf ("Invalid file name: %s\n",filename);
        exit(1);
    }
    hits = 0;
    misses = 0;
}
bool Dispatcher::dispatch ()
{
    if ( f != NULL)
    {
        uint64_t address;
        char skip[100];
        char mode;
        int r = fscanf(f, "%s %c %llx\n", skip, &mode,&address);
        while (r != EOF)
        {
            DirectMapProbe directMapProbe;
            vector <thread *> threads;
            for ( int i = 0 ; i < directMaps->size() ; i ++)
			{//Write your code here                
                threads.push_back(new std::thread(&DirectMap::check,(*directMaps)[i],address,&directMapProbe)); // check the syntax 
			}
            
            for ( int i = 0 ; i < threads.size() ; i ++) 
			{                 // Since we are dealing with direct maps in parallel, I need to join them now.
                threads[i]->join(); 
				 //Write your code here
			}             //Write your code here

                if(directMapProbe.getDirectMap()== NULL)
                {
                    // access this probe.
                    (*directMaps)[0]->access(address,mode);
                }

            else directMapProbe.getDirectMap()->access(address,mode);
             for ( int i = 0 ; i < directMaps->size() ; i ++)
			{
               if((*directMaps)[i]==directMapProbe.getDirectMap())
               {
                   (*directMaps)[i]->updateCounter(directMapProbe.getCacheLineIndex(),true); 
               } 
              else (*directMaps)[i]->updateCounter(directMapProbe.getCacheLineIndex(),false); 
 
                

            }
 
			 
            r = fscanf(f, "%s %c %llx\n", skip, &mode,&address);
        }
        return true;
    }
    else return false;
}
Dispatcher::~Dispatcher()
{
    if ( f != NULL) fclose (f);
}
