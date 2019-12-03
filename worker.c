/* 
 * Operating Systems {2INCO} Practical Assignment
 * Interprocess Communication
 *
 * Rik Litjens (1317059)
 * Roel (STUDENT_NR_2)
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
#include <string.h>
#include <errno.h>          // for perror()
#include <unistd.h>         // for getpid()
#include <mqueue.h>         // for mq-stuff
#include <time.h>           // for time()
#include <complex.h>

#include "common.h"
#include "md5s.h"

static void rsleep (int t);

int main (int argc, char * argv[])
{
    // TODO:
    // (see message_queue_test() in interprocess_basic.c)
    //  * open the two message queues (whose names are provided in the arguments)
    //  * repeatingly:
    //      - read from a message queue the new job to do
    //      - wait a random amount of time (e.g. rsleep(10000);)
    //      - do that job 
    //      - write the results to a message queue
    //    until there are no more tasks to do
    //  * close the message queues
    /**
     * Open message queues 
     */
    mqd_t               mq_fd_jobs;
    mqd_t               mq_fd_results;
    MQ_JOB              job;
    MQ_RESULT           result;
    uint128_t           tryHash;
    char                tryPsw[MAX_MESSAGE_LENGTH+2];


    if (argc > 1 ) {
        mq_fd_jobs    = mq_open (argv[1], O_RDWR);
        mq_fd_results = mq_open (argv[2], O_WRONLY);
    } else {
        printf("error");
        return (1);
    }
    
    
    while (1) {
        mq_receive (mq_fd_jobs, (char *) &job, sizeof(job) , NULL); 
        if (job.f == 1) {
            break;
        }
        
        /**
         * Try different passwords beginning with st
         */
        memset(tryPsw, 0, sizeof(tryPsw));
        snprintf(tryPsw, sizeof(tryPsw), "%c", job.st);
        
        rsleep(10000);
        
        /**
         * Hash the password we are trying and compare it
         */
        
        //start character plus all final characters 
        //(ie the final to be checked psw)
        char finalCheck[MAX_MESSAGE_LENGTH+2];
        finalCheck[0] = job.st;
        for (size_t i = 1; i < MAX_MESSAGE_LENGTH; i++)
            {
                finalCheck[i] = job.af;
            }

        /**
         * Loop over all possible messages and check hash
         */ 
        tryHash = md5s(tryPsw, strlen(tryPsw));
        if(tryHash == job.h) {
            strncpy(result.m, tryPsw, sizeof(tryPsw));
            mq_send (mq_fd_results, (char *) &result, sizeof(result), 0);
            continue;
        } 
        tryPsw[1] = job.ast;

        while (1)
            {
                
                tryHash = md5s(tryPsw, strlen(tryPsw));
                if(tryHash == job.h) {
                    strncpy(result.m, tryPsw, sizeof(tryPsw));
                    mq_send (mq_fd_results, (char *) &result, sizeof(result), 0);
                    break;
                } 
                if (strcmp(tryPsw, finalCheck) == 0) {
                    break;
                }
                /**
                 * Start at last element of possible password and loop forward
                 */ 
                for (size_t i = MAX_MESSAGE_LENGTH-1; i > 0; i--)
                {
                    //if element is not initialized ----------
                    if (tryPsw[i] < job.ast || tryPsw[i] > job.af)
                    {
                        /* continue until we find a letter */
                        continue;   
                    }
                    //if element is a letter -----------------
                    else
                    {
                        // if element is not the final letter
                        if (tryPsw[i] != job.af)
                        {
                          /* we increment */
                          tryPsw[i]++;
                          break;
                        } 
                        //if element is the final letter
                        else
                        {   
                            /* it overflows to the first char */
                            tryPsw[i] = job.ast;

                            /* remainder should be added to the previous */
                            for (size_t j = i-1; j < -1; j--)
                            {
                                //if all letters are final except first  
                                if(j == 0){
                                    /* remainder goes into adding new letter */
                                    tryPsw[i + 1] = job.ast;
                                    break;
                                }
                                
                                //if element before is also final letter
                                if (tryPsw[j] == job.af)
                                {   
                                    /* more overflow */
                                    tryPsw[j] = job.ast;
                                }

                                //if we find a non final letter thats not the first
                                else
                                {
                                    /* we find a place to drop the remainder */

                                    tryPsw[j]++;
                                    break;
                                    
                                }
                                
                            }
                        break;                            
                        }  
                    }       
                }
                //printf("%s\n", tryPsw);
                //sleep(0.60);
            }          
    }

    /**
     * All jobs are finished, so round up
     */ 
    mq_send (mq_fd_jobs, (char *) &job, sizeof(job), 0);

    mq_close (mq_fd_results);
    mq_close (mq_fd_jobs);
    return (0);
}

/*
 * rsleep(int t)
 *
 * The calling thread will be suspended for a random amount of time
 * between 0 and t microseconds
 * At the first call, the random generator is seeded with the current time
 */
static void rsleep (int t)
{
    static bool first_call = true;
    
    if (first_call == true)
    {
        srandom (time (NULL) % getpid ());
        first_call = false;
    }
    usleep (random() % t);
}


