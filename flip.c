/* 
 * Operating Systems {2INCO} Practical Assignment
 * Threaded Application
 *
 * Rik Litjens 1317059
 * Last minute drop out unfortunately
 *
 * Grading:
 * Students who hand in clean code that fully satisfies the minimum requirements will get an 8. 
 * Extra steps can lead to higher marks because we want students to take the initiative. 
 * Extra steps can be, for example, in the form of measurements added to your code, a formal 
 * analysis of deadlock freeness etc.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>          // for perror()
#include <pthread.h>

#include "uint128.h"
#include "flip.h"


// some useful functions borrowed from threaded_basics.c
// create a bitmask where bit at position n is set
#define BITMASK(n)          (((uint128_t) 1) << (n))

// check if bit n in v is set
#define BIT_IS_SET(v,n)     (((v) & BITMASK(n)) == BITMASK(n))

// set bit n in v
#define BIT_SET(v,n)        ((v) =  (v) |  BITMASK(n))

// clear bit n in v
#define BIT_CLEAR(v,n)      ((v) =  (v) & ~BITMASK(n))

// declare a mutex, and it is initialized as well
static pthread_mutex_t      mutex          = PTHREAD_MUTEX_INITIALIZER;

//length of the buffer involved
static int bufferLength = sizeof(buffer)/sizeof(buffer[0]);


/*-------------------------------------------------------------------------*/


void flipBit(m, p) {
    div_t index = div(p, 128);
    arrayIndex = index.quot;
    bitIndex = index.rem;
    if ( BIT_IS_SET ) {

    }
}

int main (void)
{
    printf("%d\n", bufferLength);
    printf("%d\n", ((NROF_PIECES/128) + 1));

    for (size_t i = 0; i < bufferLength; i++)
    {
        buffer[i] = ~0;
        printf ("v (all 1's) : %lx%016lx\n", HI(v), LO(v));
    }

    // loop through every possible (m)ultiple from
    // 2 and create a flipping thread for it
    for (size_t m = 2; m < NROF_PIECES; m++) 
    {
        for (size_t p = 1; p < NROF_PIECES; p++)
        {
            if( (p % m) == 0 ) {
                flipBit(m, p);
            }
        }
        
    }
    
    
    // TODO: start threads to flip the pieces and output the results
    // (see thread_test() and thread_mutex_test() how to use threads and mutexes,
    //  see bit_test() how to manipulate bits in a large integer)


    return (0);
}

