/* Control and store FIFO customer queue using array */
#include <time.h>

#ifndef CUSTOMERQUEUE_H
    #define CUSTOMERQUEUE_H
    
    /* Customer Struct */
    typedef struct Customer
    {
        int customer_id;
        char service_type;
        char arrival_time[9];
        int teller_id;
    } Customer;

    /* Customer Queue Struct */
    typedef struct c_queue
    {
        struct Customer* customer;
        int head;
        int tail;
        int size;
        int count;
        int complete;

    } c_queue;

    /* Functifon forward declarations */
    struct c_queue *Init_CustomerQueue(int m);
    int isEmpty(c_queue *bankqueue);
    int isFull(c_queue *bankqueue);
    void enqueue(c_queue *bankqueue, Customer *new_customer);
    Customer *dequeue(c_queue *bankqueue);
    void setComplete(c_queue* bankqueue);
    int isComplete(c_queue *bankqueue);
    void printCustomerQueue(c_queue *bankqueue);
    void freeCustomerQueue(c_queue *bankqueue);

#endif