#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

#define maxcat 50
#define show_n 5

typedef struct
{
    book *bk;
    int score;
} cand;

static int had_it(stu *s, int bid)
{
    hnode *h;

    for (h = s->hist; h != NULL; h = h->next)
    {
        if (h->bid == bid)
            return 1;
    }

    return 0;
}


static int taste(stu *s, char cats[][50], int hits[])
{
    int n = 0;
    int i;
    hnode *h;
    book *b;

    for (h = s->hist; h != NULL; h = h->next)
    {
        b = find_book(h->bid);

        if (b == NULL)
            continue;

        for (i = 0; i < n && strcmp(cats[i], b->cat) != 0; i++)
            ;

        if (i == n)
        {
            if (n == maxcat)
                continue;

            strcpy(cats[n], b->cat);
            hits[n] = 0;
            n++;
        }

        hits[i]++;
    }

    return n;
}

static int cat_hits(char cats[][50], int hits[], int n, const char *cat)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (strcmp(cats[i], cat) == 0)
            return hits[i];
    }

    return 0;
}

void recommend(void)
{
    int id, n = 0, nc, i, j, best, shown;
    char cats[maxcat][50];
    int hits[maxcat];
    stu *s;
    book *b;
    cand *list;
    cand tmp;

    printf("\nstudent id: ");
    scanf("%d", &id);

    s = find_stu(id);

    if (s == NULL)
    {
        printf("not found\n");
        return;
    }

    for (b = head; b != NULL; b = b->next)
        n++;

    list = malloc(n * sizeof(cand));

    if (list == NULL)
    {
        printf("no memory\n");
        return;
    }

    nc = taste(s, cats, hits);
    n = 0;

    for (b = head; b != NULL; b = b->next)
    {
        if (had_it(s, b->id))
            continue;

        list[n].bk = b;
        list[n].score = 10 * cat_hits(cats, hits, nc, b->cat) + b->count;
        n++;
    }

    if (n == 0)
    {
        printf("nothing to recommend\n");
        free(list);
        return;
    }

    printf("\n--- picks for %s ---\n", s->name);

    if (nc == 0)
        printf("(no history yet, showing popular books)\n");

    shown = n < show_n ? n : show_n;

    for (i = 0; i < shown; i++)
    {
        best = i;

        for (j = i + 1; j < n; j++)
        {
            if (list[j].score > list[best].score)
                best = j;
        }

        tmp = list[i];
        list[i] = list[best];
        list[best] = tmp;

        printf("%d. %s | by %s | %s | %s\n", i + 1, list[i].bk->title, list[i].bk->author,
               list[i].bk->cat, list[i].bk->avail ? "in" : "out");
    }

    free(list);
}
