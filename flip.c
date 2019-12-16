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


// some useful functions borrowed from threaded_basics.c:

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

/*-------------------------------------------------------------------------*/


void flipBit(int m, int p) {

    div_t index = div(p, 128);
    int arrayIndex = index.quot;
    int bitIndex = index.rem;

    if ( BIT_IS_SET(buffer[arrayIndex], bitIndex) ) {
       BIT_CLEAR(buffer[arrayIndex], bitIndex);
    }
    else{
        BIT_SET(buffer[arrayIndex], bitIndex);
    }
}

static void *
flip_thread (void * m_arg)
{

     int *   m_argi; 
    int     m;      
    int *   rtnval;

    m_argi = (int *) m_arg;     // proper casting before dereferencing (could also be done in one statement)
    m = *m_argi;              // get the integer value of the pointer
    free (m_arg);  

    for (size_t p = 1; p < NROF_PIECES; p++)
        {
            if( (p % m) == 0 ) {
                flipBit(m, p);
            }
    }

    return (0);    
}

int main (void)
{
    printf("%d\n", ((NROF_PIECES/128)));

    //initialize all values buffer to 1 / black
    for (size_t i = 0; i < (NROF_PIECES/128); i++)
    {
        buffer[i] = ~0;
        printf ("v (all 1's) : %lx%016lx\n", HI(buffer[i]), LO(buffer[i]));
    }

    int *       m_parameter;
    pthread_t   thread_id[NROF_PIECES];
    // loop through every possible (m)ultiple from
    // 2 and create a flipping thread for it
    for (size_t m = 2; m < NROF_PIECES; m++) 
    {
        m_parameter =  malloc (sizeof (int));
        *m_parameter = m;
        printf ("%lx: starting thread ...\n", pthread_self());
        pthread_create (&thread_id[m-2], NULL, flip_thread, m_parameter);
        
        // wait for the thread, and we are interested in the return value
        //pthread_join (thread_id, NULL);

    }
    pthread_join (thread_id[0], NULL);

    printf('endloopnowprint\n');
    for (size_t i = 0; i < (NROF_PIECES/128); i++)
    {
        for (size_t j = 0; j < 128; j++)
        {
            if ( BIT_IS_SET(buffer[i], j) ) {
                printf ("%d\n", (128*i + j));    
            }
        }        
    }
    
    
    // TODO: start threads to flip the pieces and output the results
    // (see thread_test() and thread_mutex_test() how to use threads and mutexes,
    //  see bit_test() how to manipulate bits in a large integer)


    return (0);
}

