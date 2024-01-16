/* Control and store FIFO customer queue using array */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cq.h"
#include "customers.h"

/* Create a new customer queue using malloc allocation */
c_queue *Init_CustomerQueue(int m)
{

    // Create an buffer called customer queue
    c_queue *bankqueue = (c_queue *)malloc(sizeof(c_queue));
    bankqueue->customer = (Customer *)malloc(m * sizeof(Customer));
    bankqueue->size = m;
    bankqueue->count = 0;

    return bankqueue;
}

/* Check if a buffer is empty */
int isEmpty(c_queue *bankqueue)
{
    return bankqueue->count == 0;
}

/* Check if a buffer is full */
int isFull(c_queue *bankqueue)
{
    return bankqueue->count == bankqueue->size;
}

/* Customer comes and join the end of the queue */
void enqueue(c_queue *bankqueue, Customer *new_customer)
{
    if (isFull(bankqueue))
    {
        printf("Error: Our bank is currently overloaded ˙◠˙ \n");
        return;
    }
    if (isEmpty(bankqueue))
    {
        bankqueue->customer[0] = *new_customer;
        bankqueue->count += 1;
    }

    // Insert the customer from the end of the bank queue
    else
    {
        // Insert the customer from the end of the bank queue
        bankqueue ->customer[bankqueue->count] = *new_customer;
        bankqueue->count += 1;
    }
    
    
    printCustomerQueue(bankqueue);
}

/* Teller takes customers from the begining of the queue */
Customer *dequeue(c_queue *bankqueue)
{

    if (isEmpty(bankqueue))
    {
        printf("Error: No more customers to be served ˙◠˙\n");
        return NULL;
    }

    Customer *person = &(bankqueue->customer[bankqueue->head]);
    printf("Customer ID: %d\n", person->customer_id);
    bankqueue->count -= 1;

    

    /*Update the position*/
    for (int i = 0; i < bankqueue->count; i++)
    {
        bankqueue->customer[i] = bankqueue->customer[i + 1];
    }

    return person;
}

/* Print queue just for testing */
void printCustomerQueue(c_queue *bankqueue)
{
    int i;
    printf("Customer Queue:\n");
    printf("---------------\n");
    if (isEmpty(bankqueue))
    {
        printf("Empty Queue\n");
    }
    else
    {
        for (i = 0; i < bankqueue->count; i++)
        {
            Customer *person = &(bankqueue->customer[i]);
            printf("(%d,%d,%c) ", i, person->customer_id, person->service_type);
        }
        printf("\n");
    }
    // printf("Bank count: %d", bankqueue->count);
}

/* Free memory */
void freeCustomerQueue(c_queue *bankqueue)
{
    free(bankqueue->customer);
    free(bankqueue);
}