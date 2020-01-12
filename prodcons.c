/* 
 * Operating Systems {2INCO} Practical Assignment
 * Condition Variables Application
 *
 * Rik Litjens (1317059)
 * Second student dropped out 
 *
 * Grading:
 * Students who hand in clean code that fully satisfies the minimum requirements will get an 8. 
 * Extra steps can lead to higher marks because we want students to take the initiative. 
 * Extra steps can be, for example, in the form of measurements added to your code, a formal 
 * analysis of deadlock freeness etc.
 */
 
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "prodcons.h"

static ITEM buffer[BUFFER_SIZE];

static void rsleep (int t);			// already implemented (see below)
static ITEM get_next_item (void);	// already implemented (see below)
// declare a mutex and a condition variable, and they are initialized as well
static pthread_mutex_t      mutex            		 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t       conditionWorkToDo        = PTHREAD_COND_INITIALIZER;
static pthread_cond_t       conditionConsToDo        = PTHREAD_COND_INITIALIZER;
static int					elementsInBuffer 	     = 0;
static int 					nextBufferSetPos         = 0;


/* producer thread */
static void * 
producer (void * arg)
{
    while (true)
    {
        // get the new item
		ITEM item = get_next_item();
		
        rsleep (100);	// simulating all kind of activities...
		
		//put item into the buffer
		pthread_mutex_lock(&mutex);
		while ( !(elementsInBuffer < BUFFER_SIZE) )
		{
			pthread_cond_wait(&conditionWorkToDo, &mutex);
		}
		
		buffer[nextBufferSetPos] = item;
		//put item in buffer for consumer and go
		if(item == NROF_ITEMS){break;}
		nextBufferSetPos = (nextBufferSetPos + 1) % BUFFER_SIZE;
		elementsInBuffer +=1;

		//send signal to the consumer that he can resume consuming
		pthread_cond_signal(&conditionConsToDo);
		

		pthread_mutex_unlock(&mutex);
		// TODO:
		// * put the item into buffer[]
		//
        // follow this pseudocode (according to the ConditionSynchronization lecture):
        //      mutex-lock;
        //      while not condition-for-this-producer
        //          wait-cv;
        //      critical-section;
        //      possible-cv-signals;
        //      mutex-unlock;
        //
        // (see condition_test() in condition_basics.c how to use condition variables)
    }
	return (NULL);
}

/* consumer thread */
static void * 
consumer (void * arg)
{
	int nextBufferGetPos = 0;
    while (true /* TODO: not all items retrieved from buffer[] */)
    {
        // TODO: 
		// * get the next item from buffer[]

		pthread_mutex_lock(&mutex);
		while (!(elementsInBuffer > 0)){
			pthread_cond_wait(&conditionConsToDo, &mutex);
		}
		ITEM item = buffer[nextBufferGetPos];
		//break if item indicates that all work has been done
		if(item == NROF_ITEMS){break;}
		printf("%d\n", item);
		nextBufferGetPos = (nextBufferGetPos + 1) % BUFFER_SIZE;
		pthread_cond_signal(&conditionWorkToDo);
		pthread_mutex_unlock(&mutex);
        //
        // follow this pseudocode (according to the ConditionSynchronization lecture):
        //      mutex-lock;
        //      while not condition-for-this-consumer
        //          wait-cv;
        //      critical-section;
        //      possible-cv-signals;
        //      mutex-unlock;
		
        rsleep (100);		// simulating all kind of activities...
    }
	return (NULL);
}

int main (void)
{

	//create producers and a consumer
	pthread_t   my_threads[NROF_PRODUCERS+1];
	for (size_t i = 0; i < NROF_PRODUCERS; i++)
	{
		pthread_create (&my_threads[i], NULL, producer, NULL);
	}
	pthread_create (&my_threads[NROF_PRODUCERS], NULL, consumer, NULL);

	//wait for threads
	for (size_t i = 0; i < NROF_PRODUCERS+1; i++)
	{
    	pthread_join (my_threads[i], NULL);  
	}
	
    return (0);
}

/*
 * rsleep(int t)
 *
 * The calling thread will be suspended for a random amount of time between 0 and t microseconds
 * At the first call, the random generator is seeded with the current time
 */
static void 
rsleep (int t)
{
    static bool first_call = true;
    
    if (first_call == true)
    {
        srandom (time(NULL));
        first_call = false;
    }
    usleep (random () % t);
}


/* 
 * get_next_item()
 *
 * description:
 *		thread-safe function to get a next job to be executed
 *		subsequent calls of get_next_item() yields the values 0..NROF_ITEMS-1 
 *		in arbitrary order 
 *		return value NROF_ITEMS indicates that all jobs have already been given
 * 
 * parameters:
 *		none
 *
 * return value:
 *		0..NROF_ITEMS-1: job number to be executed
 *		NROF_ITEMS:		 ready
 */
static ITEM
get_next_item(void)
{
    static pthread_mutex_t	job_mutex	= PTHREAD_MUTEX_INITIALIZER;
	static bool 			jobs[NROF_ITEMS+1] = { false };	// keep track of issued jobs
	static int              counter = 0;    // seq.nr. of job to be handled
    ITEM 					found;          // item to be returned
	
	/* avoid deadlock: when all producers are busy but none has the next expected item for the consumer 
	 * so requirement for get_next_item: when giving the (i+n)'th item, make sure that item (i) is going to be handled (with n=nrof-producers)
	 */
	pthread_mutex_lock (&job_mutex);

    counter++;
	if (counter > NROF_ITEMS)
	{
	    // we're ready
	    found = NROF_ITEMS;
	}
	else
	{
	    if (counter < NROF_PRODUCERS)
	    {
	        // for the first n-1 items: any job can be given
	        // e.g. "random() % NROF_ITEMS", but here we bias the lower items
	        found = (random() % (2*NROF_PRODUCERS)) % NROF_ITEMS;
	    }
	    else
	    {
	        // deadlock-avoidance: item 'counter - NROF_PRODUCERS' must be given now
	        found = counter - NROF_PRODUCERS;
	        if (jobs[found] == true)
	        {
	            // already handled, find a random one, with a bias for lower items
	            found = (counter + (random() % NROF_PRODUCERS)) % NROF_ITEMS;
	        }    
	    }
	    
	    // check if 'found' is really an unhandled item; 
	    // if not: find another one
	    if (jobs[found] == true)
	    {
	        // already handled, do linear search for the oldest
	        found = 0;
	        while (jobs[found] == true)
            {
                found++;
            }
	    }
	}
    jobs[found] = true;
			
	pthread_mutex_unlock (&job_mutex);
	return (found);
}

