#include <stdio.h>
#include <stdlib.h>
#include "issue_return.h"
#include "student.h"
#include "book.h"   /* Member 1's module: needs get_book_by_id(),
                       set_book_availability(), increment_borrow_count() */

#define MAX_TRANSACTIONS 100

static Transaction transactions[MAX_TRANSACTIONS];
static int transactionCount = 0;

static Queue waitQueue = { NULL, NULL };

void issue_book(void)
{
    int student_id, book_id, day;
    Book *book;
    StudentNode *student;

    printf("\nEnter Student ID: ");
    scanf("%d", &student_id);

    student = search_student_by_id(student_id);
    if (student == NULL)
    {
        printf("Student not found.\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &book_id);

    book = get_book_by_id(book_id);
    if (book == NULL)
    {
        printf("Book not found.\n");
        return;
    }

    if (!book->is_available)
    {
        printf("Book is currently issued. Adding you to the waiting queue.\n");
        enqueue_waiting(student_id, book_id);
        return;
    }

    printf("Enter issue day (e.g. day number of the term): ");
    scanf("%d", &day);

    if (transactionCount >= MAX_TRANSACTIONS)
    {
        printf("Transaction log is full.\n");
        return;
    }

    transactions[transactionCount].student_id = student_id;
    transactions[transactionCount].book_id = book_id;
    transactions[transactionCount].issue_day = day;
    transactions[transactionCount].return_day = -1;
    transactions[transactionCount].returned = 0;
    transactionCount++;

    set_book_availability(book_id, 0);
    increment_borrow_count(book_id);
    change_borrowed_count(student_id, 1);

    printf("Book issued successfully.\n");
}

void return_book(void)
{
    int student_id, book_id, day;
    int i, found = -1;

    printf("\nEnter Student ID: ");
    scanf("%d", &student_id);

    printf("Enter Book ID: ");
    scanf("%d", &book_id);

    for (i = 0; i < transactionCount; i++)
    {
        if (transactions[i].student_id == student_id &&
            transactions[i].book_id == book_id &&
            transactions[i].returned == 0)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        printf("No matching active transaction found.\n");
        return;
    }

    printf("Enter return day: ");
    scanf("%d", &day);

    transactions[found].return_day = day;
    transactions[found].returned = 1;

    set_book_availability(book_id, 1);
    change_borrowed_count(student_id, -1);

    printf("Book returned successfully.\n");

    /* hand the book straight to the next student waiting, if any */
    process_queue_for_book(book_id);
}

void enqueue_waiting(int student_id, int book_id)
{
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));

    if (node == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    node->student_id = student_id;
    node->book_id = book_id;
    node->next = NULL;

    if (waitQueue.rear == NULL)
    {
        waitQueue.front = node;
        waitQueue.rear = node;
    }
    else
    {
        waitQueue.rear->next = node;
        waitQueue.rear = node;
    }

    printf("Added to waiting queue.\n");
}

void process_queue_for_book(int book_id)
{
    QueueNode *temp = waitQueue.front;
    QueueNode *prev = NULL;

    while (temp != NULL)
    {
        if (temp->book_id == book_id)
        {
            printf("Notify Student ID %d: Book %d is now available.\n",
                   temp->student_id, temp->book_id);

            if (prev == NULL)
                waitQueue.front = temp->next;
            else
                prev->next = temp->next;

            if (temp == waitQueue.rear)
                waitQueue.rear = prev;

            free(temp);
            return; /* only the first matching student in line gets notified */
        }

        prev = temp;
        temp = temp->next;
    }
}

void display_queue(void)
{
    QueueNode *temp = waitQueue.front;

    if (temp == NULL)
    {
        printf("\nWaiting queue is empty.\n");
        return;
    }

    printf("\n===== WAITING QUEUE =====\n");

    while (temp != NULL)
    {
        printf("Student ID %d waiting for Book ID %d\n",
               temp->student_id, temp->book_id);
        temp = temp->next;
    }
}
