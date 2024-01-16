#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "cq.h"
#include "customers.h"
#include "tellers.h"

pthread_mutex_t cqueue_mutex;
pthread_cond_t cqueue_empty_condition;

/* There are two shared variables that need to be protected when taking customers out of queue:
    *** c_queue bankqueue (store customer information):

    In this assignment both customers() and tellers() thread access c_queue concurrently.
    In order to ensure mutual exclusion (mutex) when accessing c_queue:
        - pthread_mutex_lock(): before accessing the queue, call this func to create a lock
        - pthread_mutex_unlock(): once thread is done, call this func to release the lock

    *** count (keep track num of customers in the queue):
    Both customers() - increase num customers by adding more to queue
    and tellers() - decrease num of customers, use count variable to know there is no more customers to serve
    In order to ensure mutual exclusion (mutex) when accessing count:
        - pthread_mutex_lock(): before accessing count, call this func to create a lock
        - pthread_mutex_unlock(): once thread is done, call this func to release the lock


    In order to implement synchronization between the customers() and teller() threads:
    - pthread_cond_wait(): can be used to block the teller() thread when there are no more customers to serve
    - pthread_cond_signal(): can be used to wake up the teller() thread when a new customer is added to the queue.

    To implement this synchronization, I use a condition variable conjunction with the mutex lock.
    The pthread_cond_wait() function atomically releases the mutex lock and blocks the calling thread
    until the condition variable is signaled by another thread. The pthread_cond_signal() function signals
    the condition variable, waking up any threads that are waiting on it.

    */

int main(int argc, char *argv[])
{
    FILE *log = fopen("r_log.txt", "w");
    fclose(log);

    /* Check the command syntax*/
    if (argc != 6)
    {
        perror("Error, Invalid number of command line arguments");
        pthread_exit(NULL);
    }

    int m = atoi(argv[1]);
    int tc = atoi(argv[2]);
    int tw = atoi(argv[3]);
    int td = atoi(argv[4]);
    int ti = atoi(argv[5]);

    if (m < 1 || tc < 0 || tw < 0 || td < 0 || ti < 0)
    {
        perror("Error, Invalid value!");
        pthread_exit(NULL);
    }

    pthread_t customerThread;
    pthread_t tellersThread[4];                       /* Array to hold 4 tellers */
    c_queue *bankqueue = Init_CustomerQueue(m);       /*Initialize variables for bank queue and mutex*/
    pthread_mutex_init(&cqueue_mutex, NULL);          /* mutex lock to control customer queue*/
    pthread_cond_init(&cqueue_empty_condition, NULL); /* condition variable to control customer queueu -lock - waiting - unlock */

    /* Create customer thread */
    struct cus_args *cus_input = (struct cus_args *)malloc(sizeof(struct cus_args));
    cus_input->tc = &tc;
    cus_input->bankqueue = bankqueue;
    cus_input->cqueue_mutex = &cqueue_mutex;
    cus_input->cqueue_empty_condition = &cqueue_empty_condition;

    if (pthread_create(&customerThread, NULL, customers, (void *)cus_input))
    {
        perror("Error creating customer thread");
        return 1;
    }

    /* Create 4 tellers thread */
    struct teller_args *tel_input;

    tel_input = (struct teller_args *)malloc(sizeof(struct teller_args));
    tel_input->id = 1;
    tel_input->tw = &tw;
    tel_input->td = &td;
    tel_input->ti = &ti;
    tel_input->bankqueue = bankqueue;
    tel_input->cqueue_mutex = &cqueue_mutex;
    tel_input->cqueue_empty_condition = &cqueue_empty_condition;

    if (pthread_create(&tellersThread[1], NULL, teller, (void *)tel_input))
    {
        perror("Error creating teller thread");
        return 1;
    }

    /* Wait for customer thread to finish */
    if (pthread_join(customerThread, NULL))
    {
        perror("Error joining customer thread");
        return 1;
    }

    /* Wait for all tellers threads to finish */

    if (pthread_join(tellersThread[1], NULL))
    {
        perror("Error joining tellers thread");
        return 1;
    }

    /* Clean up resources */
    pthread_mutex_destroy(&cqueue_mutex);
    pthread_cond_destroy(&cqueue_empty_condition);
    freeCustomerQueue(bankqueue);
    free(cus_input);
    free(tel_input);

    return 0;
}