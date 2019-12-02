#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "uint128.h"

// settings for interprocess communications
// (note: be sure that MQ_MAX_MESSAGES <= /proc/sys/fs/mqueue/msg_max (= 10 on most systems))
#define NROF_WORKERS    	1
#define MQ_MAX_MESSAGES 	10


// settings for the alphabet
#define ALPHABET_START_CHAR	'a'
#define ALPHABET_END_CHAR	'd'
	
	
// settings for the jobs to be done (generated by md5s_main)
static uint128_t md5_list[] =
{
    UINT128(0x0cc175b9c0f1b6a8,0x31c399e269772661),    /* 'a' */
    UINT128(0x900150983cd24fb0,0xd6963f7d28e17f72),    /* 'abc' */
    UINT128(0x77963b7a931377ad,0x4ab5ad6a9cd718aa),    /* 'ddd' */
    UINT128(0x4e510be093d34651,0x2011c3f4fe36e4af),    /* 'bababa' */
    UINT128(0xd4b7c284882ca9e2,0x08bb65e8abd5f4c8),    /* 'bcd' */
    UINT128(0x0b4e7a0e5fe84ad3,0x5fb5f95b9ceeac79),    /* 'aaaaaa' */
    UINT128(0x8277e0910d750195,0xb448797616e091ad),    /* 'd' */
    UINT128(0x5ca2aa845c8cd5ac,0xe6b016841f100d82),    /* 'da' */
    UINT128(0xd77d5e503ad1439f,0x585ac494268b351b),    /* 'db' */
    UINT128(0xcfcbe5bdf31116aa,0x3dcffb7e7470333e),    /* 'bdada' */
    // UINT128(0x65ba841e01d6db77,0x33e90a5b7f9e6f80),    /* 'bbbb' */
    // UINT128(0x4124bc0a9335c27f,0x086f24ba207a4912),    /* 'aa' */
    // UINT128(0x523af537946b79c4,0xf8369ed39ba78605),    /* 'ad' */
    // UINT128(0x9134669f44c1af05,0x32f613b7508283c4),    /* 'dcba' */
    // UINT128(0xe2fc714c4727ee93,0x95f324cd2e7f331f),    /* 'abcd' */
    // UINT128(0x980ac217c6b51e7d,0xc41040bec1edfec8),    /* 'dddddd' */
};


// the defines below do not need to be modified:

// calculate the number of elements in the alphabet
#define ALPHABET_NROF_CHAR	(ALPHABET_END_CHAR - ALPHABET_START_CHAR + 1)

// calculate the number of elements in the array md5_list[]
#define MD5_LIST_NROF		(sizeof(md5_list) / sizeof(md5_list[0]))

// the total number of jobs that have to be done
#define JOBS_NROF			(ALPHABET_NROF_CHAR * MD5_LIST_NROF)

#endif

