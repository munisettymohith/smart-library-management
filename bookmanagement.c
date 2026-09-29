#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

book *head = NULL;
static book *tail = NULL;

book *find_book(int id)
{
    book *t = head;

    while (t != NULL)
    {
        if (t->id == id)
            return t;

        t = t->next;
    }

    return NULL;
}

void put_book(book *b)
{
    b->next = NULL;

    if (head == NULL)
        head = b;
    else
        tail->next = b;

    tail = b;
}

static void show_one(book *b)
{
    printf("\nid     : %d", b->id);
    printf("\ntitle  : %s", b->title);
    printf("\nauthor : %s", b->author);
    printf("\ncat    : %s", b->cat);
    printf("\nstatus : %s", b->avail ? "in" : "out");
    printf("\ncount  : %d\n", b->count);
}

void add_book(void)
{
    book *b = malloc(sizeof(book));

    if (b == NULL)
    {
        printf("no memory\n");
        return;
    }

    printf("\nbook id: ");
    scanf("%d", &b->id);

    if (find_book(b->id) != NULL)
    {
        printf("id exists\n");
        free(b);
        return;
    }

    printf("title: ");
    scanf(" %99[^\n]", b->title);

    printf("author: ");
    scanf(" %99[^\n]", b->author);

    printf("category: ");
    scanf(" %49[^\n]", b->cat);

    b->avail = 1;
    b->count = 0;
    b->next = NULL;

    if (head == NULL)
        head = b;
    else
        tail->next = b;

    tail = b;

    printf("book added\n");
}

void show_books(void)
{
    book *t = head;

    if (head == NULL)
    {
        printf("\nno books\n");
        return;
    }

    printf("\n--- all books ---\n");

    while (t != NULL)
    {
        show_one(t);
        t = t->next;
    }
}

void edit_book(void)
{
    int id;
    book *b;

    printf("\nbook id to edit: ");
    scanf("%d", &id);

    b = find_book(id);

    if (b == NULL)
    {
        printf("not found\n");
        return;
    }

    printf("new title: ");
    scanf(" %99[^\n]", b->title);

    printf("new author: ");
    scanf(" %99[^\n]", b->author);

    printf("new category: ");
    scanf(" %49[^\n]", b->cat);

    printf("book updated\n");
}

void del_book(void)
{
    int id;
    book *t = head;
    book *prev = NULL;

    printf("\nbook id to delete: ");
    scanf("%d", &id);

    while (t != NULL && t->id != id)
    {
        prev = t;
        t = t->next;
    }

    if (t == NULL)
    {
        printf("not found\n");
        return;
    }

    if (prev == NULL)
        head = t->next;
    else
        prev->next = t->next;

    if (t == tail)
        tail = prev;

    free(t);

    printf("book deleted\n");
}

void by_id(void)
{
    int id;
    book *b;

    printf("\nbook id: ");
    scanf("%d", &id);

    b = find_book(id);

    if (b == NULL)
        printf("not found\n");
    else
        show_one(b);
}

void find_text(int field)
{
    char x[100];
    char *what;
    book *t = head;
    int found = 0;

    printf("\nsearch text: ");
    scanf(" %99[^\n]", x);

    while (t != NULL)
    {
        if (field == 1)
            what = t->title;
        else if (field == 2)
            what = t->author;
        else
            what = t->cat;

        if (strcmp(what, x) == 0)
        {
            show_one(t);
            found = 1;
        }

        t = t->next;
    }

    if (!found)
        printf("not found\n");
}

static book *merge(book *a, book *b, int type)
{
    int pick_a;

    if (a == NULL)
        return b;

    if (b == NULL)
        return a;

    if (type == 1)
        pick_a = strcmp(a->title, b->title) <= 0;
    else
        pick_a = a->count >= b->count;

    if (pick_a)
    {
        a->next = merge(a->next, b, type);
        return a;
    }

    b->next = merge(a, b->next, type);
    return b;
}

static void split(book *src, book **front, book **back)
{
    book *slow = src;
    book *fast = src->next;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    *front = src;
    *back = slow->next;
    slow->next = NULL;
}

static void msort(book **list, int type)
{
    book *front;
    book *back;

    if (*list == NULL || (*list)->next == NULL)
        return;

    split(*list, &front, &back);

    msort(&front, type);
    msort(&back, type);

    *list = merge(front, back, type);
}

void sort_books(int type)
{
    if (head == NULL || head->next == NULL)
        return;

    msort(&head, type);

    tail = head;

    while (tail->next != NULL)
        tail = tail->next;
}
