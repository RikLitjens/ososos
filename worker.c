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
        mq_receive (mq_fd_jobs, (char *) &job, attr.mq_maxmsg , NULL); //attr.mq_maxmsg
        if (job.f == 1) {
            break;
        }

        
        //strncpy(tryPsw, job.st, sizeof(job.st));
        //printf(tryPsw);
        rsleep(10000000);
        //tryHash = md5s(tryPsw, sizeof(tryPsw));

        printf("starts with '%c, %d'\n", job.st, job.f);


        strncpy(result.m, "afbouw", 6);
        if(tryHash == job.h) {
            strncpy(result.m, tryPsw, sizeof(tryPsw));
            result.h = job.h;
            mq_send (mq_fd_results, (char *) &result, attr.mq_maxmsg, 0);
        }
        
    }
    printf("EINDEEEEEE WHILEEEEEEEEEEE LOOOOOOOOOOOOP\n");

    mq_send (mq_fd_jobs, (char *) &job, attr.mq_maxmsg, 0);

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


