/* 
 * Operating Systems {2INCO} Practical Assignment
 * Interprocess Communication
 *
 * Contains definitions which are commonly used by the farmer and the workers
 *
 */

#ifndef COMMON_H
#define COMMON_H


// maximum size for any message in the tests
#define MAX_MESSAGE_LENGTH	6
 



//TODO :
#define STUDENT_NAME        "Rik_Litjens"
static char                 mq_name_jobs[80];
static char                 mq_name_results[80];
extern uint128_t;
/**
 * Datastructures for the jobs and results that are 
 * to be interchanged between the farmer and its workers 
 */
typedef struct
{
    // a data structure with 2 members
    // start symbol s and hash h
    char                    s;
    uint128_t               h;
} MQ_JOB;

typedef struct
{
    // a data structure with 2 members
    // result --only if succesfull 
    // msg m corresponding to hash h 
    char                    m[6];
    uint128_t               h;
} MQ_RESULT;

#endif

