#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "customers.h"

void *customers(void *input)
{
    int *tc = ((struct cus_args *)input)->tc;
    c_queue *bankqueue = ((struct cus_args *)input)->bankqueue;
    pthread_mutex_t *cqueue_mutex = ((struct cus_args *)input)->cqueue_mutex;
    pthread_cond_t *cqueue_empty_condition = ((struct cus_args *)input)->cqueue_empty_condition;

    char read_file[] = "c_file.txt";
    char write_file[] = "r_log.txt";
    int count = 0;           // keeps track of the number of customers be added to the queue

    // Open the input file for reading
    FILE *file = fopen(read_file, "r");

    if (file == NULL)
    {
        perror("Error, input file could not be opened");
        pthread_exit(NULL);
    }

    // Read customer information from the input file and add it to the bank queue after tc seconds
    Customer customer;

    while (count < 20 && fscanf(file, "%d %c", &customer.customer_id, &customer.service_type) != EOF)
    {
        pthread_mutex_lock(cqueue_mutex);

        /* Add customer to the queue */
        Customer *new_customer = (Customer *)malloc(sizeof(Customer));

        // Get the current time in format HH:MM:SS
        time_t current_time = time(NULL);
        struct tm *time_info;
        char arrival[9]; //// space for "HH:MM:SS\0"
        time(&current_time);
        time_info = localtime(&current_time);
        strftime(arrival, sizeof(arrival), "%H:%M:%S", time_info);

        /* Store the arrival time in the customer struct */
        new_customer->customer_id = customer.customer_id;
        new_customer->service_type = customer.service_type;
        strncpy(new_customer->arrival_time, arrival, sizeof(new_customer->arrival_time) - 1);

        enqueue(bankqueue, new_customer);
        count++;

        // Write the customer's activity to r_log by using appending mode
        FILE *log = fopen(write_file, "a");

        if (log == NULL)
        {
            printf("Failed to open file for writing\n");
        }

        // Write the customer's activity to the file in the specified format
        fprintf(log, "-----------------------------------------------------------------------\n");
        fprintf(log, "%d: %c\n", customer.customer_id, customer.service_type);
        fprintf(log, "Arrival time: %s\n", arrival);

        // Close the file
        fclose(log);

        pthread_mutex_unlock(cqueue_mutex);

        // Signal the condition variable to wake up the tellers
        pthread_cond_signal(cqueue_empty_condition);

        // Sleep for tc seconds before reading from the input file again
        sleep(*tc);
    }

    // Close the file
    fclose(file);
    return NULL;
}