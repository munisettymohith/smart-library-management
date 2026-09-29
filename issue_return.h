#ifndef ISSUE_RETURN_H
#define ISSUE_RETURN_H

typedef struct Transaction
{
    int student_id;
    int book_id;
    int issue_day;
    int return_day;
    int returned;
} Transaction;

typedef struct QueueNode
{
    int student_id;
    int book_id;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *front;
    QueueNode *rear;
} Queue;

void issue_book(void);
void return_book(void);
void enqueue_waiting(int student_id, int book_id);
void process_queue_for_book(int book_id);
void display_queue(void);

#endif
