#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

#define f_books "data/books.txt"
#define f_stu   "data/students.txt"
#define f_hist  "data/history.txt"
#define f_logs  "data/logs.txt"

/* every record is one line, fields split with '|' so titles and
   names can keep their spaces */

static void save_books(void)
{
    FILE *f = fopen(f_books, "w");
    book *b;

    if (f == NULL)
    {
        printf("cannot write %s\n", f_books);
        return;
    }

    for (b = head; b != NULL; b = b->next)
        fprintf(f, "%d|%s|%s|%s|%d|%d\n", b->id, b->title, b->author, b->cat, b->avail, b->count);

    fclose(f);
}

static void save_students(void)
{
    FILE *fs = fopen(f_stu, "w");
    FILE *fh = fopen(f_hist, "w");
    stu *s;
    hnode *h;

    if (fs == NULL || fh == NULL)
    {
        printf("cannot write student files\n");

        if (fs != NULL)
            fclose(fs);

        if (fh != NULL)
            fclose(fh);

        return;
    }

    for (s = first_stu(); s != NULL; s = s->next)
    {
        fprintf(fs, "%d|%s\n", s->id, s->name);

        for (h = s->hist; h != NULL; h = h->next)
            fprintf(fh, "%d|%d|%d\n", s->id, h->bid, h->back);
    }

    fclose(fs);
    fclose(fh);
}

static void load_books(void)
{
    FILE *f = fopen(f_books, "r");
    book *b;

    if (f == NULL)
        return;

    for (;;)
    {
        b = malloc(sizeof(book));

        if (b == NULL)
            break;

        if (fscanf(f, " %d|%99[^|]|%99[^|]|%49[^|]|%d|%d",
                   &b->id, b->title, b->author, b->cat, &b->avail, &b->count) != 6)
        {
            free(b);
            break;
        }

        if (find_book(b->id) != NULL)
        {
            free(b);
            continue;
        }

        put_book(b);
    }

    fclose(f);
}

static void load_students(void)
{
    FILE *f = fopen(f_stu, "r");
    int id, sid, bid, back;
    char name[100];

    if (f != NULL)
    {
        while (fscanf(f, " %d|%99[^\n]", &id, name) == 2)
        {
            if (find_stu(id) == NULL)
                load_stu(id, name);
        }

        fclose(f);
    }

    f = fopen(f_hist, "r");

    if (f != NULL)
    {
        while (fscanf(f, " %d|%d|%d", &sid, &bid, &back) == 3)
            load_hist(sid, bid, back);

        fclose(f);
    }
}

void save_all(void)
{
    save_books();
    save_students();
    logs_save(f_logs);

    printf("data saved\n");
}

void load_all(void)
{
    /* only load into an empty library, otherwise ids would clash */
    if (head != NULL || first_stu() != NULL)
        return;

    load_books();
    load_students();
    logs_load(f_logs);
}
