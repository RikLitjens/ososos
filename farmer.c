/* 
 * Operating Systems {2INCO} Practical Assignment
 * Interprocess Communication
 *
 * STUDENT_NAME_1 (STUDENT_NR_1)
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
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>    
#include <unistd.h>         // for execlp
#include <mqueue.h>         // for mq

#include "settings.h"
#include "common.h"

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
    if (argc != 1)
    {
        fprintf (stderr, "%s: invalid arguments\n", argv[0]);
    }
        
    // TODO:
    //  * create the message queues (see message_queue_test() in interprocess_basic.c)
    /**
     * Creating the message queues for sender jobs to workers
     * and for receiving information from workers
     */
    pid_t               processID;      /* Process ID from fork() */
    mqd_t               mq_fd_jobs;
    mqd_t               mq_fd_results;
    MQ_JOB  job;
    MQ_RESULT result;
    struct mq_attr      attr;

    attr.mq_maxmsg  = MQ_MAX_MESSAGES;
    attr.mq_msgsize = MAX_MESSAGE_LENGTH;
    
    sprintf (mq_name_jobs, "/mq_request_%s_%d", STUDENT_NAME, getpid());
    sprintf (mq_name_results, "/mq_response_%s_%d", STUDENT_NAME, getpid());
    
    mq_fd_jobs = mq_open (mq_name_jobs, O_WRONLY | O_CREAT | O_EXCL, 0600, &attr);
    mq_fd_results = mq_open (mq_name_results, O_RDONLY | O_CREAT | O_EXCL, 0600, &attr);

    getattr(mq_fd_jobs);
    getattr(mq_fd_results);

    //  * create the child processes (see process_test() and message_queue_test())
    processID = fork();
    if (processID < 0)
    {
        perror("fork() failed");
        exit (1);
    }
    else
    {
        if (processID == 0)
        {
            // child-stuff
            //message_queue_child ();
            printf("kind");
            exit (0);
        }
        else
        {
            // remaining of the parent stuff
            printf("pater");
            // fill job
            // job.s = 73;
            // job.h =   ;

            // sleep (3);
            // // send the request
            // printf ("parent: sending...\n");
            // mq_send (mq_fd_request, (char *) &job, sizeof (job), 0);

            // sleep (3);
            // // read the result and store it in the response message
            // printf ("parent: receiving...\n");
            // mq_receive (mq_fd_response, (char *) &rsp, sizeof (rsp), NULL);

            // printf ("parent: received: %d, '", rsp.e);
            // // printing characters of f[] separately:
            // for (int i = 0; i < rsp.e; i++)
            // {
            //     printf ("%c", rsp.f[i]);
            // }
            // // printing g[] in one step (because it has the \0-terminator):
            // printf ("', '%s'\n", rsp.g);
    
            // sleep (1);
    
            waitpid (processID, NULL, 0);   // wait for the child
            
            mq_close (mq_fd_results);
            mq_close (mq_fd_jobs);
            mq_unlink (mq_name_jobs);
            mq_unlink (mq_name_results);   
        }
    }


    //  * do the farming
    //  * wait until the chilren have been stopped (see process_test())
    //  * clean up the message queues (see message_queue_test())

    // Important notice: make sure that the names of the message queues contain your
    // student name and the process id (to ensure uniqueness during testing)
    
    return (0);
}

