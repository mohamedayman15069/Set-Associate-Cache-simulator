#ifndef UTILS_H_
#define UTILS_H_

#include <includes.h>

class Utils{
    public:
        template <typename T>
        static uint8_t count_bits (T n) {
            int counter = 0;
            while(n) {
                counter += n % 2;
                n >>= 1;
            }
            return counter;
        }
        template <typename T>
        static uint8_t get_first_one_pos (T n) {
            uint8_t position = 1;
            T m = 1;
            while (!(n & m)) {
                m = m << 1;
                position++;
            }
            return position;
        }

        template <typename T>
        static T extract_bits(T n , int count, int from)
        {
            T x = 1;
            x <<= count;
            x -=1;
            T y = n >> (from);
            return (x&y);
        }


};



#endif