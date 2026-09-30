#include <stdio.h>
#include <stdlib.h>
#include "library.h"

#define loan 14
#define f_queue "data/queue.txt"

typedef struct trans
{
    int sid;
    int bid;
    int iday;
    int rday;
    int done;
    struct trans *next;
} trans;

typedef struct wait
{
    int sid;
    int bid;
    struct wait *next;
} wait;

static trans *lhead = NULL;
static trans *ltail = NULL;

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

static trans *log_add(int sid, int bid, int iday, int rday, int done)
{
    trans *n = malloc(sizeof(trans));

    if (n == NULL)
        return NULL;

    n->sid = sid;
    n->bid = bid;
    n->iday = iday;
    n->rday = rday;
    n->done = done;
    n->next = NULL;

    if (lhead == NULL)
        lhead = n;
    else
        ltail->next = n;

    ltail = n;

    return n;
}

static void log_free(void)
{
    trans *nx;

    while (lhead != NULL)
    {
        nx = lhead->next;
        free(lhead);
        lhead = nx;
    }

    ltail = NULL;
}

static int wait_push(int sid, int bid)
{
    wait *n = malloc(sizeof(wait));

    if (n == NULL)
        return 0;

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

    return 1;
}

static void wait_add(int sid, int bid)
{
    if (wait_push(sid, bid))
        printf("added to queue\n");
    else
        printf("no memory\n");
}

static void wait_free(void)
{
    wait *t;

    while (rear != NULL)
    {
        t = rear->next;

        if (t == rear)
            rear = NULL;
        else
            rear->next = t->next;

        free(t);
    }
}

static int wait_go(int bid)
{
    wait *prev;
    wait *cur;
    int sid;

    if (rear == NULL)
        return -1;

    prev = rear;
    cur = rear->next;

    do
    {
        if (cur->bid == bid)
        {
            sid = cur->sid;

            if (cur == prev)
                rear = NULL;
            else
            {
                prev->next = cur->next;

                if (cur == rear)
                    rear = prev;
            }

            free(cur);
            return sid;
        }

        prev = cur;
        cur = cur->next;
    } while (prev != rear);

    return -1;
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

    if (!ask("issue day: ", &day))
        return;

    if (log_add(sid, bid, day, -1, 0) == NULL)
    {
        printf("no memory\n");
        return;
    }

    b->avail = 0;
    b->count++;
    hist_add(sid, bid);

    printf("book issued, due in %d days\n", loan);
}

void return_book(void)
{
    int sid, bid, day, late, next;
    trans *at = NULL;
    trans *t;
    book *b;

    if (!ask("\nstudent id: ", &sid))
        return;

    if (!ask("book id: ", &bid))
        return;

    for (t = lhead; t != NULL && at == NULL; t = t->next)
    {
        if (t->sid == sid && t->bid == bid && !t->done)
            at = t;
    }

    if (at == NULL)
    {
        printf("no such issue\n");
        return;
    }

    if (!ask("return day: ", &day))
        return;

    at->rday = day;
    at->done = 1;

    b = find_book(bid);

    if (b != NULL)
        b->avail = 1;

    hist_back(sid, bid);

    printf("book returned\n");

    late = day - at->iday - loan;

    if (late > 0)
        printf("late by %d days, fine rs %.2f\n", late, calc_fine(late));

    if (b == NULL)
        return;

    while ((next = wait_go(bid)) != -1)
    {
        if (find_stu(next) == NULL)
        {
            printf("student %d no longer exists, skipped\n", next);
            continue;
        }

        if (log_add(next, bid, day, -1, 0) == NULL)
        {
            printf("no memory\n");
            return;
        }

        b->avail = 0;
        b->count++;
        hist_add(next, bid);

        printf("book issued to waiting student %d, due in %d days\n", next, loan);
        break;
    }
}

float fine_of(int sid)
{
    trans *t;
    float sum = 0;
    int late;

    for (t = lhead; t != NULL; t = t->next)
    {
        if (t->sid == sid && t->done)
        {
            late = t->rday - t->iday - loan;

            if (late > 0)
                sum += calc_fine(late);
        }
    }

    return sum;
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

static void queue_save(void)
{
    FILE *f = fopen(f_queue, "w");
    wait *t;

    if (f == NULL)
    {
        printf("cannot write %s\n", f_queue);
        return;
    }

    if (rear != NULL)
    {
        t = rear->next;

        do
        {
            fprintf(f, "%d|%d\n", t->sid, t->bid);
            t = t->next;
        } while (t != rear->next);
    }

    fclose(f);
}

static void queue_load(void)
{
    FILE *f = fopen(f_queue, "r");
    int sid, bid;

    if (f == NULL)
        return;

    wait_free();

    while (fscanf(f, " %d|%d", &sid, &bid) == 2)
    {
        if (!wait_push(sid, bid))
            break;
    }

    fclose(f);
}

void logs_save(const char *path)
{
    FILE *f;
    trans *t;

    queue_save();

    f = fopen(path, "w");

    if (f == NULL)
    {
        printf("cannot write %s\n", path);
        return;
    }

    for (t = lhead; t != NULL; t = t->next)
        fprintf(f, "%d|%d|%d|%d|%d\n", t->sid, t->bid, t->iday, t->rday, t->done);

    fclose(f);
}

void logs_load(const char *path)
{
    FILE *f;
    int sid, bid, iday, rday, done;

    queue_load();

    f = fopen(path, "r");

    if (f == NULL)
        return;

    log_free();

    while (fscanf(f, " %d|%d|%d|%d|%d", &sid, &bid, &iday, &rday, &done) == 5)
    {
        if (log_add(sid, bid, iday, rday, done) == NULL)
            break;
    }

    fclose(f);
}
