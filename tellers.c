#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "cq.h"
#include "customers.h"
#include "tellers.h"

/* Function to simulate a teller serving customers */
void *teller(void *input)
{
    int id = ((struct teller_args *)input)->id;
    int *tw = ((struct teller_args *)input)->tw;
    int *td = ((struct teller_args *)input)->td;
    int *ti = ((struct teller_args *)input)->ti;
    c_queue *bankqueue = ((struct teller_args *)input)->bankqueue;
    pthread_mutex_t *cqueue_mutex = ((struct teller_args *)input)->cqueue_mutex;
    pthread_cond_t *cqueue_empty_condition = ((struct teller_args *)input)->cqueue_empty_condition;
    int total_customer_been_served = 0;

    pthread_mutex_lock(cqueue_mutex);

    // Get the next customer from the queue
    Customer *person = (Customer *)dequeue(bankqueue);

    total_customer_been_served += 1;

    while(bankqueue->count < 0 && total_customer_been_served < 20){
        printf("No customers. Waiting for more");
        pthread_cond_wait(cqueue_empty_condition, cqueue_mutex);
    }

    /* Unlock mutex before serving the customer */
    pthread_mutex_unlock(cqueue_mutex);

    write_start_to_log(id, person);
    sleep_service(person, tw, td, ti);
    write_finish_to_log(id, person);

    return NULL;
}

void sleep_service(Customer *person, int *tw, int *td, int *ti)
{
    /* Serving the customer based on services they choose */
    if (person == NULL)
    {
        perror("Not a valid customer!");
    }
    else
    {
        switch (person->service_type)
        {
        case 'W':
        {
            sleep(*tw);
            break;
        }

        case 'D':
        {
            sleep(*td);
            break;
        }

        case 'I':
        {
            sleep(*ti);
            break;
        }

        default:
            perror("Not a valid service!");
            break;
        }
    }
}

void write_start_to_log(int teller_id, Customer *person)
{
    FILE *log = fopen("r_log.txt", "a");
    /* Calculate response time - the correct response time is the time that customer spent waiting in the queue after comming*/
    time_t response_time = time(NULL);
    char response[9]; // space for "HH:MM:SS\0"
    struct tm *response_time_tm = localtime(&response_time);
    strftime(response, sizeof(response), "%H:%M:%S", response_time_tm);

    if (log == NULL)
    {
        perror("Failed to open file for writing");
        return;
    }

    fprintf(log, "\n");
    fprintf(log, "Teller: %d\n", teller_id);
    fprintf(log, "Customer: %d\n", person->customer_id);
    fprintf(log, "Arrival time: %s\n", person->arrival_time);
    fprintf(log, "Response time: %s\n", response);

    fclose(log);
}

void write_finish_to_log(int teller_id, Customer *person)
{
    FILE *log = fopen("r_log.txt", "a");
    /* Calculate the completion time */
    time_t completion_time = time(NULL);
    char completion[9]; // space for "HH:MM:SS\0"
    struct tm *completion_time_tm = localtime(&completion_time);
    strftime(completion, sizeof(completion), "%H:%M:%S", completion_time_tm);

    if (log == NULL)
    {
        perror("Failed to open file for writing");
        return;
    }

    fprintf(log, "\n");
    fprintf(log, "Teller: %d\n", teller_id);
    fprintf(log, "Customer: %d\n", person->customer_id);
    fprintf(log, "Arrival time: %s\n", person->arrival_time);
    fprintf(log, "Completion time: %s\n", completion);

    fclose(log);
}
void write_last_summary(teller_spec *teller_specs)
{
    FILE *log = fopen("r_log.txt", "a");
    if (log == NULL)
    {
        perror("Failed to open file for writing");
        return;
    }
    for (int i = 0; i < NUMBER_OF_TELLER; i++)
    {
        teller_spec *spec = teller_specs + i;
        fprintf(log, "-----------------------------------------------------------------------\n");
        fprintf(log, "Teller - %d serves %d customers\n", spec->id, spec->count);
    }
    fclose(log);
}