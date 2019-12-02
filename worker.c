/* 
 * Operating Systems {2INCO} Practical Assignment
 * Interprocess Communication
 *
 * Rik Litjens (1317059)
 * STUDENT_NAME_2 (STUDENT_NR_2)
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

static void 
getattr (mqd_t mq_fd)
{
    struct mq_attr      attr;
    int                 rtnval;
    
    rtnval = mq_getattr (mq_fd, &attr);
    if (rtnval == -1)
    {
        perror ("mq_getattr() failed");
        exit (1);
    }
    fprintf (stderr, "%d: mqdes=%d max=%ld size=%ld nrof=%ld\n",
                getpid(), 
                mq_fd, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
}

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
    struct mq_attr      attr;
    int                 rtn = mq_getattr (mq_fd_jobs, &attr); 
    uint128_t           tryHash;
    char                tryPsw[6];


    printf("starting while loop\n");
    if (argc > 1 ) {
        mq_fd_jobs    = mq_open (argv[1], O_RDONLY);
        mq_fd_results = mq_open (argv[2], O_WRONLY);
    } else {
        printf("error");
        return (1);
    }
    
    
    while (1) {
        getattr(mq_fd_jobs);
        mq_receive (mq_fd_jobs, (char *) &job, sizeof(job) , NULL); 
        if (job.f == 1) {
            break;
        }
        
        /**
         * Try different passwords beginning with st
         */
        snprintf(tryPsw, sizeof(tryPsw), "%c", job.st);
        printf("Try: %s\n", tryPsw);
        
        rsleep(100000);
        
        /**
         * Hash the password we are trying and compare it
         */
        
        printf("starts with '%c', %d %d\n", job.st, job.f, sizeof(job));

        
        //start character plus all final characters 
        //(ie the final to be checked psw)
        char finalCheck[MAX_MESSAGE_LENGTH+1];
        finalCheck[0] = job.st;
        for (size_t i = 1; i < MAX_MESSAGE_LENGTH; i++)
            {
                finalCheck[i] = job.af;
            }
        printf("%s\n", finalCheck);

        /**
         * Loop over all possible messages and check hash
         */ 
        tryHash = md5s(tryPsw, strlen(tryPsw));
        if(tryHash == job.h) {
            printf("Jawel hoor ze zijn gelijk tis wat\n");
            strncpy(result.m, tryPsw, sizeof(tryPsw));
            result.h = job.h;
            mq_send (mq_fd_results, (char *) &result, sizeof(result), 0);
            continue;
        } 
        tryPsw[1] = job.ast;

        printf("SIngelton does not work so we try %s\n", tryPsw);
        printf("This is the checkerdie check '%c'", tryPsw[2]);
        while (1)
            {
                if (strcmp(tryPsw, finalCheck) == 0) {
                    break;
                }
                tryHash = md5s(tryPsw, strlen(tryPsw));
                if(tryHash == job.h) {
                    printf("Jawel hoor ze zijn gelijk tis wat\n");
                    strncpy(result.m, tryPsw, sizeof(tryPsw));
                    result.h = job.h;
                    mq_send (mq_fd_results, (char *) &result, sizeof(result), 0);
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
                        print("EMTPY SO SKIP\n");
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

                            printf("final d to a, i=%d\n", i);
                            /* remainder should be added to the previous */
                            for (size_t j = i-1; i < -1; i++)
                            {
                                printf("START LOOP %d\n", j);
                                //if all letters are final except first  
                                if(j == 0){
                                    printf("ADD NEW LETTER\n");
                                    /* remainder goes into adding new letter */
                                    tryPsw[i + 1] = job.ast;
                                    break;
                                }
                                
                                //if element before is also final letter
                                if (tryPsw[j] == job.af)
                                {   
                                    printf("FINAL BECOMES A\n");
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
                printf("%s\n", tryPsw);
                sleep(1);
            }          
    }

    /**
     * All jobs are finished, so round up
     */ 
    printf("EINDEEEEEE WHILEEEEEEEEEEE LOOOOOOOOOOOOP\n");

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


