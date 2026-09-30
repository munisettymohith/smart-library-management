#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

static stu *shead = NULL;

static void read_text(char *dest, int size)
{
    int c = getchar();
    int i = 0;

    while (c == '\n' || c == ' ')
        c = getchar();

    while (c != '\n' && c != EOF && i < size - 1)
    {
        dest[i] = c;
        i++;
        c = getchar();
    }

    dest[i] = '\0';
}

static int ask_num(const char *msg, int *out)
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

static void free_hist(hnode *h)
{
    hnode *nx;

    while (h != NULL)
    {
        nx = h->next;
        free(h);
        h = nx;
    }
}

stu *find_stu(int id)
{
    stu *t = shead;

    while (t != NULL && t->id != id)
        t = t->next;

    return t;
}

void add_stu(void)
{
    stu *n;
    int id;

    if (!ask_num("\nstudent id: ", &id))
        return;

    if (find_stu(id) != NULL)
    {
        printf("id already used\n");
        return;
    }

    n = malloc(sizeof(stu));

    if (n == NULL)
    {
        printf("no memory\n");
        return;
    }

    n->id = id;

    printf("name: ");
    read_text(n->name, sizeof(n->name));

    n->count = 0;
    n->hist = NULL;
    n->next = shead;
    shead = n;

    printf("student added\n");
}

void show_stu(void)
{
    stu *t;

    if (shead == NULL)
    {
        printf("\nno students\n");
        return;
    }

    printf("\n--- students (newest first) ---\n");

    for (t = shead; t != NULL; t = t->next)
    {
        printf("\nid    : %d", t->id);
        printf("\nname  : %s", t->name);
        printf("\nbooks : %d\n", t->count);
    }
}

void edit_stu(void)
{
    int id;
    stu *s;

    if (!ask_num("\nstudent id to edit: ", &id))
        return;

    s = find_stu(id);

    if (s == NULL)
    {
        printf("not found\n");
        return;
    }

    printf("new name: ");
    read_text(s->name, sizeof(s->name));

    printf("student updated\n");
}

void del_stu(void)
{
    int id;
    stu **pp = &shead;
    stu *gone;

    if (!ask_num("\nstudent id to delete: ", &id))
        return;

    while (*pp != NULL && (*pp)->id != id)
        pp = &(*pp)->next;

    if (*pp == NULL)
    {
        printf("not found\n");
        return;
    }

    gone = *pp;
    *pp = gone->next;

    free_hist(gone->hist);
    free(gone);

    printf("student deleted\n");
}

void hist_add(int sid, int bid)
{
    stu *s = find_stu(sid);
    hnode *h;

    if (s == NULL)
        return;

    h = malloc(sizeof(hnode));

    if (h == NULL)
        return;

    h->bid = bid;
    h->back = 0;
    h->next = s->hist;
    s->hist = h;
    s->count++;
}

void hist_back(int sid, int bid)
{
    stu *s = find_stu(sid);
    hnode *h;

    if (s == NULL)
        return;

    for (h = s->hist; h != NULL; h = h->next)
    {
        if (h->bid == bid && !h->back)
        {
            h->back = 1;
            s->count--;
            return;
        }
    }
}

void show_mine(void)
{
    int id;
    stu *s;
    hnode *h;
    book *b;

    if (!ask_num("\nstudent id: ", &id))
        return;

    s = find_stu(id);

    if (s == NULL)
    {
        printf("not found\n");
        return;
    }

    if (s->hist == NULL)
    {
        printf("no books yet\n");
        return;
    }

    printf("\n--- books of %s ---\n", s->name);

    for (h = s->hist; h != NULL; h = h->next)
    {
        b = find_book(h->bid);
        printf("book %d | %s | %s\n", h->bid, b ? b->title : "(deleted)", h->back ? "returned" : "with student");
    }
}

int count_stu(void)
{
    stu *t;
    int n = 0;

    for (t = shead; t != NULL; t = t->next)
        n++;

    return n;
}

int count_busy(void)
{
    stu *t;
    int n = 0;

    for (t = shead; t != NULL; t = t->next)
    {
        if (t->count > 0)
            n++;
    }

    return n;
}

stu *first_stu(void)
{
    return shead;
}

void load_stu(int id, const char *name)
{
    stu *n = malloc(sizeof(stu));
    stu *t;

    if (n == NULL)
        return;

    n->id = id;
    strncpy(n->name, name, sizeof(n->name) - 1);
    n->name[sizeof(n->name) - 1] = '\0';
    n->count = 0;
    n->hist = NULL;
    n->next = NULL;

    if (shead == NULL)
    {
        shead = n;
        return;
    }

    for (t = shead; t->next != NULL; t = t->next)
        ;

    t->next = n;
}

void load_hist(int sid, int bid, int back)
{
    stu *s = find_stu(sid);
    hnode *h;
    hnode *t;

    if (s == NULL)
        return;

    h = malloc(sizeof(hnode));

    if (h == NULL)
        return;

    h->bid = bid;
    h->back = back;
    h->next = NULL;

    if (s->hist == NULL)
        s->hist = h;
    else
    {
        for (t = s->hist; t->next != NULL; t = t->next)
            ;

        t->next = h;
    }

    if (!back)
        s->count++;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

static stu *shead = NULL;

static void read_text(char *dest, int size)
{
    int c = getchar();
    int i = 0;

    while (c == '\n' || c == ' ')
        c = getchar();

    while (c != '\n' && c != EOF && i < size - 1)
    {
        dest[i] = c;
        i++;
        c = getchar();
    }

    dest[i] = '\0';
}

/* reads one whole number into *out.
   returns 1 on success, 0 if the input was not a number.
   on bad input the rest of the line is thrown away so the next
   read starts clean */
static int ask_num(const char *msg, int *out)
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

static void free_hist(hnode *h)
{
    hnode *nx;

    while (h != NULL)
    {
        nx = h->next;
        free(h);
        h = nx;
    }
}

stu *find_stu(int id)
{
    stu *t = shead;

    while (t != NULL && t->id != id)
        t = t->next;

    return t;
}

void add_stu(void)
{
    stu *n;
    int id;

    if (!ask_num("\nstudent id: ", &id))
        return;

    if (find_stu(id) != NULL)
    {
        printf("id already used\n");
        return;
    }

    n = malloc(sizeof(stu));

    if (n == NULL)
    {
        printf("no memory\n");
        return;
    }

    n->id = id;

    printf("name: ");
    read_text(n->name, sizeof(n->name));

    n->count = 0;
    n->hist = NULL;
    n->next = shead;
    shead = n;

    printf("student added\n");
}

void show_stu(void)
{
    stu *t;

    if (shead == NULL)
    {
        printf("\nno students\n");
        return;
    }

    printf("\n--- students (newest first) ---\n");

    for (t = shead; t != NULL; t = t->next)
    {
        printf("\nid    : %d", t->id);
        printf("\nname  : %s", t->name);
        printf("\nbooks : %d\n", t->count);
    }
}

void edit_stu(void)
{
    int id;
    stu *s;

    if (!ask_num("\nstudent id to edit: ", &id))
        return;

    s = find_stu(id);

    if (s == NULL)
    {
        printf("not found\n");
        return;
    }

    printf("new name: ");
    read_text(s->name, sizeof(s->name));

    printf("student updated\n");
}

void del_stu(void)
{
    int id;
    stu **pp = &shead;
    stu *gone;

    if (!ask_num("\nstudent id to delete: ", &id))
        return;

    while (*pp != NULL && (*pp)->id != id)
        pp = &(*pp)->next;

    if (*pp == NULL)
    {
        printf("not found\n");
        return;
    }

    gone = *pp;
    *pp = gone->next;

    free_hist(gone->hist);
    free(gone);

    printf("student deleted\n");
}

void hist_add(int sid, int bid)
{
    stu *s = find_stu(sid);
    hnode *h;

    if (s == NULL)
        return;

    h = malloc(sizeof(hnode));

    if (h == NULL)
        return;

    h->bid = bid;
    h->back = 0;
    h->next = s->hist;
    s->hist = h;
    s->count++;
}

void hist_back(int sid, int bid)
{
    stu *s = find_stu(sid);
    hnode *h;

    if (s == NULL)
        return;

    for (h = s->hist; h != NULL; h = h->next)
    {
        if (h->bid == bid && !h->back)
        {
            h->back = 1;
            s->count--;
            return;
        }
    }
}

void show_mine(void)
{
    int id;
    stu *s;
    hnode *h;
    book *b;

    if (!ask_num("\nstudent id: ", &id))
        return;

    s = find_stu(id);

    if (s == NULL)
    {
        printf("not found\n");
        return;
    }

    if (s->hist == NULL)
    {
        printf("no books yet\n");
        return;
    }

    printf("\n--- books of %s ---\n", s->name);

    for (h = s->hist; h != NULL; h = h->next)
    {
        b = find_book(h->bid);
        printf("book %d | %s | %s\n", h->bid, b ? b->title : "(deleted)", h->back ? "returned" : "with student");
    }
}

int count_stu(void)
{
    stu *t;
    int n = 0;

    for (t = shead; t != NULL; t = t->next)
        n++;

    return n;
}

int count_busy(void)
{
    stu *t;
    int n = 0;

    for (t = shead; t != NULL; t = t->next)
    {
        if (t->count > 0)
            n++;
    }

    return n;
}

stu *first_stu(void)
{
    return shead;
}

void load_stu(int id, const char *name)
{
    stu *n = malloc(sizeof(stu));
    stu *t;

    if (n == NULL)
        return;

    n->id = id;
    strncpy(n->name, name, sizeof(n->name) - 1);
    n->name[sizeof(n->name) - 1] = '\0';
    n->count = 0;
    n->hist = NULL;
    n->next = NULL;

    if (shead == NULL)
    {
        shead = n;
        return;
    }

    for (t = shead; t->next != NULL; t = t->next)
        ;

    t->next = n;
}

void load_hist(int sid, int bid, int back)
{
    stu *s = find_stu(sid);
    hnode *h;
    hnode *t;

    if (s == NULL)
        return;

    h = malloc(sizeof(hnode));

    if (h == NULL)
        return;

    h->bid = bid;
    h->back = back;
    h->next = NULL;

    if (s->hist == NULL)
        s->hist = h;
    else
    {
        for (t = s->hist; t->next != NULL; t = t->next)
            ;

        t->next = h;
    }

    if (!back)
        s->count++;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

static stu *shead = NULL;

static void read_text(char *dest, int size)
{
    int c = getchar();
    int i = 0;

    while (c == '\n' || c == ' ')
        c = getchar();

    while (c != '\n' && c != EOF && i < size - 1)
    {
        dest[i] = c;
        i++;
        c = getchar();
    }

    dest[i] = '\0';
}

/* reads one whole number into *out.
   returns 1 on success, 0 if the input was not a number.
   on bad input the rest of the line is thrown away so the next
   read starts clean */
static int ask_num(const char *msg, int *out)
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

static void free_hist(hnode *h)
{
    hnode *nx;

    while (h != NULL)
    {
        nx = h->next;
        free(h);
        h = nx;
    }
}

stu *find_stu(int id)
{
    stu *t = shead;

    while (t != NULL && t->id != id)
        t = t->next;

    return t;
}

void add_stu(void)
{
    stu *n;
    int id;

    if (!ask_num("\nstudent id: ", &id))
        return;

    if (find_stu(id) != NULL)
    {
        printf("id already used\n");
        return;
    }

    n = malloc(sizeof(stu));

    if (n == NULL)
    {
        printf("no memory\n");
        return;
    }

    n->id = id;

    printf("name: ");
    read_text(n->name, sizeof(n->name));

    n->count = 0;
    n->hist = NULL;
    n->next = shead;
    shead = n;

    printf("student added\n");
}

void show_stu(void)
{
    stu *t;

    if (shead == NULL)
    {
        printf("\nno students\n");
        return;
    }

    printf("\n--- students (newest first) ---\n");

    for (t = shead; t != NULL; t = t->next)
    {
        printf("\nid    : %d", t->id);
        printf("\nname  : %s", t->name);
        printf("\nbooks : %d\n", t->count);
    }
}

void edit_stu(void)
{
    int id;
    stu *s;

    if (!ask_num("\nstudent id to edit: ", &id))
        return;

    s = find_stu(id);

    if (s == NULL)
    {
        printf("not found\n");
        return;
    }

    printf("new name: ");
    read_text(s->name, sizeof(s->name));

    printf("student updated\n");
}

void del_stu(void)
{
    int id;
    stu **pp = &shead;
    stu *gone;

    if (!ask_num("\nstudent id to delete: ", &id))
        return;

    while (*pp != NULL && (*pp)->id != id)
        pp = &(*pp)->next;

    if (*pp == NULL)
    {
        printf("not found\n");
        return;
    }

    gone = *pp;
    *pp = gone->next;

    free_hist(gone->hist);
    free(gone);

    printf("student deleted\n");
}

void hist_add(int sid, int bid)
{
    stu *s = find_stu(sid);
    hnode *h;

    if (s == NULL)
        return;

    h = malloc(sizeof(hnode));

    if (h == NULL)
        return;

    h->bid = bid;
    h->back = 0;
    h->next = s->hist;
    s->hist = h;
    s->count++;
}

void hist_back(int sid, int bid)
{
    stu *s = find_stu(sid);
    hnode *h;

    if (s == NULL)
        return;

    for (h = s->hist; h != NULL; h = h->next)
    {
        if (h->bid == bid && !h->back)
        {
            h->back = 1;
            s->count--;
            return;
        }
    }
}

void show_mine(void)
{
    int id;
    stu *s;
    hnode *h;
    book *b;

    if (!ask_num("\nstudent id: ", &id))
        return;

    s = find_stu(id);

    if (s == NULL)
    {
        printf("not found\n");
        return;
    }

    if (s->hist == NULL)
    {
        printf("no books yet\n");
        return;
    }

    printf("\n--- books of %s ---\n", s->name);

    for (h = s->hist; h != NULL; h = h->next)
    {
        b = find_book(h->bid);
        printf("book %d | %s | %s\n", h->bid, b ? b->title : "(deleted)", h->back ? "returned" : "with student");
    }
}

int count_stu(void)
{
    stu *t;
    int n = 0;

    for (t = shead; t != NULL; t = t->next)
        n++;

    return n;
}

int count_busy(void)
{
    stu *t;
    int n = 0;

    for (t = shead; t != NULL; t = t->next)
    {
        if (t->count > 0)
            n++;
    }

    return n;
}

stu *first_stu(void)
{
    return shead;
}

void load_stu(int id, const char *name)
{
    stu *n = malloc(sizeof(stu));
    stu *t;

    if (n == NULL)
        return;

    n->id = id;
    strncpy(n->name, name, sizeof(n->name) - 1);
    n->name[sizeof(n->name) - 1] = '\0';
    n->count = 0;
    n->hist = NULL;
    n->next = NULL;

    if (shead == NULL)
    {
        shead = n;
        return;
    }

    for (t = shead; t->next != NULL; t = t->next)
        ;

    t->next = n;
}

void load_hist(int sid, int bid, int back)
{
    stu *s = find_stu(sid);
    hnode *h;
    hnode *t;

    if (s == NULL)
        return;

    h = malloc(sizeof(hnode));

    if (h == NULL)
        return;

    h->bid = bid;
    h->back = back;
    h->next = NULL;

    if (s->hist == NULL)
        s->hist = h;
    else
    {
        for (t = s->hist; t->next != NULL; t = t->next)
            ;

        t->next = h;
    }

    if (!back)
        s->count++;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

static stu *shead = NULL;

static void read_text(char *dest, int size)
{
    int c = getchar();
    int i = 0;

    while (c == '\n' || c == ' ')
        c = getchar();

    while (c != '\n' && c != EOF && i < size - 1)
    {
        dest[i] = c;
        i++;
        c = getchar();
    }

    dest[i] = '\0';
}

/* reads one whole number into *out.
   returns 1 on success, 0 if the input was not a number.
   on bad input the rest of the line is thrown away so the next
   read starts clean */
static int ask_num(const char *msg, int *out)
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

static void free_hist(hnode *h)
{
    hnode *nx;

    while (h != NULL)
    {
        nx = h->next;
        free(h);
        h = nx;
    }
}

stu *find_stu(int id)
{
    stu *t = shead;

    while (t != NULL && t->id != id)
        t = t->next;

    return t;
}

void add_stu(void)
{
    stu *n;
    int id;

    if (!ask_num("\nstudent id: ", &id))
        return;

    if (find_stu(id) != NULL)
    {
        printf("id already used\n");
        return;
    }

    n = malloc(sizeof(stu));

    if (n == NULL)
    {
        printf("no memory\n");
        return;
    }

    n->id = id;

    printf("name: ");
    read_text(n->name, sizeof(n->name));

    n->count = 0;
    n->hist = NULL;
    n->next = shead;
    shead = n;

    printf("student added\n");
}

void show_stu(void)
{
    stu *t;

    if (shead == NULL)
    {
        printf("\nno students\n");
        return;
    }

    printf("\n--- students (newest first) ---\n");

    for (t = shead; t != NULL; t = t->next)
    {
        printf("\nid    : %d", t->id);
        printf("\nname  : %s", t->name);
        printf("\nbooks : %d\n", t->count);
    }
}

void edit_stu(void)
{
    int id;
    stu *s;

    if (!ask_num("\nstudent id to edit: ", &id))
        return;

    s = find_stu(id);

    if (s == NULL)
    {
        printf("not found\n");
        return;
    }

    printf("new name: ");
    read_text(s->name, sizeof(s->name));

    printf("student updated\n");
}

void del_stu(void)
{
    int id;
    stu **pp = &shead;
    stu *gone;

    if (!ask_num("\nstudent id to delete: ", &id))
        return;

    while (*pp != NULL && (*pp)->id != id)
        pp = &(*pp)->next;

    if (*pp == NULL)
    {
        printf("not found\n");
        return;
    }

    gone = *pp;
    *pp = gone->next;

    free_hist(gone->hist);
    free(gone);

    printf("student deleted\n");
}

void hist_add(int sid, int bid)
{
    stu *s = find_stu(sid);
    hnode *h;

    if (s == NULL)
        return;

    h = malloc(sizeof(hnode));

    if (h == NULL)
        return;

    h->bid = bid;
    h->back = 0;
    h->next = s->hist;
    s->hist = h;
    s->count++;
}

void hist_back(int sid, int bid)
{
    stu *s = find_stu(sid);
    hnode *h;

    if (s == NULL)
        return;

    for (h = s->hist; h != NULL; h = h->next)
    {
        if (h->bid == bid && !h->back)
        {
            h->back = 1;
            s->count--;
            return;
        }
    }
}

void show_mine(void)
{
    int id;
    stu *s;
    hnode *h;
    book *b;

    if (!ask_num("\nstudent id: ", &id))
        return;

    s = find_stu(id);

    if (s == NULL)
    {
        printf("not found\n");
        return;
    }

    if (s->hist == NULL)
    {
        printf("no books yet\n");
        return;
    }

    printf("\n--- books of %s ---\n", s->name);

    for (h = s->hist; h != NULL; h = h->next)
    {
        b = find_book(h->bid);
        printf("book %d | %s | %s\n", h->bid, b ? b->title : "(deleted)", h->back ? "returned" : "with student");
    }
}

int count_stu(void)
{
    stu *t;
    int n = 0;

    for (t = shead; t != NULL; t = t->next)
        n++;

    return n;
}

int count_busy(void)
{
    stu *t;
    int n = 0;

    for (t = shead; t != NULL; t = t->next)
    {
        if (t->count > 0)
            n++;
    }

    return n;
}

stu *first_stu(void)
{
    return shead;
}

void load_stu(int id, const char *name)
{
    stu *n = malloc(sizeof(stu));
    stu *t;

    if (n == NULL)
        return;

    n->id = id;
    strncpy(n->name, name, sizeof(n->name) - 1);
    n->name[sizeof(n->name) - 1] = '\0';
    n->count = 0;
    n->hist = NULL;
    n->next = NULL;

    if (shead == NULL)
    {
        shead = n;
        return;
    }

    for (t = shead; t->next != NULL; t = t->next)
        ;

    t->next = n;
}

void load_hist(int sid, int bid, int back)
{
    stu *s = find_stu(sid);
    hnode *h;
    hnode *t;

    if (s == NULL)
        return;

    h = malloc(sizeof(hnode));

    if (h == NULL)
        return;

    h->bid = bid;
    h->back = back;
    h->next = NULL;

    if (s->hist == NULL)
        s->hist = h;
    else
    {
        for (t = s->hist; t->next != NULL; t = t->next)
            ;

        t->next = h;
    }

    if (!back)
        s->count++;
}
