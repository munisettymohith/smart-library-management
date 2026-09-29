#include <stdio.h>
#include <stdlib.h>
#include "library.h"

#define BOOK_FILE "data/books.txt"
#define STUDENT_FILE "data/students.txt"
#define HISTORY_FILE "data/history.txt"
#define LOG_FILE "data/logs.txt"

static void save_books(void)
{
    FILE *file;
    book *p;

    file = fopen(BOOK_FILE, "w");

    if (file == NULL)
    {
        printf("Unable to save books.\n");
        return;
    }

    for (p = head; p != NULL; p = p->next)
    {
        fprintf(file, "%d|%s|%s|%s|%d|%d\n",
                p->id, p->title, p->author, p->cat,
                p->avail, p->count);
    }

    fclose(file);
}

static void save_students(void)
{
    FILE *students;
    FILE *history;
    stu *s;
    hnode *h;

    students = fopen(STUDENT_FILE, "w");
    history = fopen(HISTORY_FILE, "w");

    if (students == NULL || history == NULL)
    {
        if (students != NULL)
            fclose(students);

        if (history != NULL)
            fclose(history);

        printf("Unable to save student data.\n");
        return;
    }

    for (s = first_stu(); s != NULL; s = s->next)
    {
        fprintf(students, "%d|%s\n", s->id, s->name);

        for (h = s->hist; h != NULL; h = h->next)
            fprintf(history, "%d|%d|%d\n",
                    s->id, h->bid, h->back);
    }

    fclose(students);
    fclose(history);
}

static void load_books(void)
{
    FILE *file;
    book *b;

    file = fopen(BOOK_FILE, "r");

    if (file == NULL)
        return;

    while (1)
    {
        b = malloc(sizeof(book));

        if (b == NULL)
            break;

        if (fscanf(file, " %d|%99[^|]|%99[^|]|%49[^|]|%d|%d",
                   &b->id, b->title, b->author, b->cat,
                   &b->avail, &b->count) != 6)
        {
            free(b);
            break;
        }

        b->next = NULL;

        if (find_book(b->id) == NULL)
            put_book(b);
        else
            free(b);
    }

    fclose(file);
}

static void load_students(void)
{
    FILE *file;
    int id;
    int sid, bid, back;
    char name[100];

    file = fopen(STUDENT_FILE, "r");

    if (file != NULL)
    {
        while (fscanf(file, " %d|%99[^\n]", &id, name) == 2)
        {
            if (find_stu(id) == NULL)
                load_stu(id, name);
        }

        fclose(file);
    }

    file = fopen(HISTORY_FILE, "r");

    if (file != NULL)
    {
        while (fscanf(file, " %d|%d|%d",
                      &sid, &bid, &back) == 3)
        {
            load_hist(sid, bid, back);
        }

        fclose(file);
    }
}

void save_all(void)
{
    save_books();
    save_students();
    logs_save(LOG_FILE);

    printf("Data saved.\n");
}

void load_all(void)
{
    if (head != NULL || first_stu() != NULL)
        return;

    load_books();
    load_students();
    logs_load(LOG_FILE);
}
