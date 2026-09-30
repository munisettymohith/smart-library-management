#include <stdio.h>
#include <stdlib.h>
#include "library.h"

#define maxlog 100
#define loan 14

typedef struct
{
    int sid;
    int bid;
    int iday;
    int rday;
    int done;
} trans;

typedef struct wait
{
    int sid;
    int bid;
    struct wait *next;
} wait;

static trans logs[maxlog];
static int nlog = 0;

static wait *rear = NULL;
static int ask(const char *msg, int *out)
{
    int c;

    printf("%s", msg);

    if (scanf("%d", out) == 1)
        return 1;

    while ((c = getchar()) != '\n' && c != EOF)
        ;

    printf("please enter a number\n");
    return 0;
}

static void wait_add(int sid, int bid)
{
    wait *n = malloc(sizeof(wait));

    if (n == NULL)
    {
        printf("no memory\n");
        return;
    }

    n->sid = sid;
    n->bid = bid;

    if (rear == NULL)
        n->next = n;
    else
    {
        n->next = rear->next;
        rear->next = n;
    }

    rear = n;

    printf("added to queue\n");
}

static void wait_go(int bid)
{
    wait *prev;
    wait *cur;

    if (rear == NULL)
        return;

    prev = rear;
    cur = rear->next;

    do
    {
        if (cur->bid == bid)
        {
            printf("tell student %d: book %d is free\n", cur->sid, bid);

            if (cur == prev)
                rear = NULL;
            else
            {
                prev->next = cur->next;

                if (cur == rear)
                    rear = prev;
            }

            free(cur);
            return;
        }

        prev = cur;
        cur = cur->next;
    } while (prev != rear);
}

void issue_book(void)
{
    int sid, bid, day;
    book *b;

    if (!ask("\nstudent id: ", &sid))
        return;

    if (find_stu(sid) == NULL)
    {
        printf("no such student\n");
        return;
    }

    if (!ask("book id: ", &bid))
        return;

    b = find_book(bid);

    if (b == NULL)
    {
        printf("no such book\n");
        return;
    }

    if (!b->avail)
    {
        printf("book is out\n");
        wait_add(sid, bid);
        return;
    }

    if (nlog >= maxlog)
    {
        printf("log full\n");
        return;
    }

    if (!ask("issue day: ", &day))
        return;

    logs[nlog].sid = sid;
    logs[nlog].bid = bid;
    logs[nlog].iday = day;
    logs[nlog].rday = -1;
    logs[nlog].done = 0;
    nlog++;

    b->avail = 0;
    b->count++;
    hist_add(sid, bid);

    printf("book issued, due in %d days\n", loan);
}

void return_book(void)
{
    int sid, bid, day, late;
    int at = -1;
    int i;
    book *b;

    if (!ask("\nstudent id: ", &sid))
        return;

    if (!ask("book id: ", &bid))
        return;

    for (i = 0; i < nlog && at == -1; i++)
    {
        if (logs[i].sid == sid && logs[i].bid == bid && !logs[i].done)
            at = i;
    }

    if (at == -1)
    {
        printf("no such issue\n");
        return;
    }

    if (!ask("return day: ", &day))
        return;

    logs[at].rday = day;
    logs[at].done = 1;

    b = find_book(bid);

    if (b != NULL)
        b->avail = 1;

    hist_back(sid, bid);

    printf("book returned\n");

    late = day - logs[at].iday - loan;

    if (late > 0)
        printf("late by %d days, fine rs %.2f\n", late, calc_fine(late));

    wait_go(bid);
}

void show_queue(void)
{
    wait *t;
    int n = 0;

    if (rear == NULL)
    {
        printf("\nqueue empty\n");
        return;
    }

    printf("\n--- queue ---\n");

    t = rear->next;

    do
    {
        n++;
        printf("%d. student %d wants book %d\n", n, t->sid, t->bid);
        t = t->next;
    } while (t != rear->next);
}

void logs_save(const char *path)
{
    FILE *f = fopen(path, "w");
    int i;

    if (f == NULL)
    {
        printf("cannot write %s\n", path);
        return;
    }

    for (i = 0; i < nlog; i++)
        fprintf(f, "%d|%d|%d|%d|%d\n", logs[i].sid, logs[i].bid, logs[i].iday, logs[i].rday, logs[i].done);

    fclose(f);
}

void logs_load(const char *path)
{
    FILE *f = fopen(path, "r");

    if (f == NULL)
        return;

    nlog = 0;

    while (nlog < maxlog &&
           fscanf(f, " %d|%d|%d|%d|%d", &logs[nlog].sid, &logs[nlog].bid,
                  &logs[nlog].iday, &logs[nlog].rday, &logs[nlog].done) == 5)
        nlog++;

    fclose(f);
}
