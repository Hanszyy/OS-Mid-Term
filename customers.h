#include <time.h>
#include <pthread.h>
#include "cq.h"

#ifndef CUSTOMERS_H
    #define CUSTOMERS_H

    typedef struct cus_args {
        int *tc;
        c_queue *bankqueue;
        pthread_mutex_t *cqueue_mutex;
        pthread_cond_t *cqueue_empty_condition;
    } cus_args;

    /* customers functions */
    void* customers(void *input);
#endif