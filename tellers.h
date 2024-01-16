#include <time.h>
#include <pthread.h>

#ifndef TELLERS_H
    #define TELLERS_H

    typedef struct teller_args {
        int id;
        int *tw, *td, *ti;
        c_queue *bankqueue;
        pthread_mutex_t *cqueue_mutex;
        pthread_cond_t *cqueue_empty_condition;
    } teller_args;

    /* Tellers struct */
    struct teller
    {
        int teller_id;
        int status;
        int count_customer;
        time_t start_time;
    };

    /* tellers functions */
    void *teller(void *input);
    void sleep_service(Customer *person, int *tw, int *td, int *ti);
    void write_start_to_log(int teller_id, Customer *person);
    void write_finish_to_log(int teller_id, Customer *person);
    void write_last_summary(teller_spec *teller_specs);
#endif