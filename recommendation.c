#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

#define MAX_RECOMMEND 5

static int already_borrowed(stu *s, int book_id)
{
    hnode *h = s->hist;

    while (h != NULL)
    {
        if (h->bid == book_id)
            return 1;

        h = h->next;
    }

    return 0;
}

static int category_history(stu *s, const char *category)
{
    int total = 0;
    hnode *h = s->hist;

    while (h != NULL)
    {
        book *b = find_book(h->bid);

        if (b != NULL && strcmp(b->cat, category) == 0)
            total++;

        h = h->next;
    }

    return total;
}

static int book_score(stu *s, book *b)
{
    int history_score = category_history(s, b->cat);

    /*
       Ten points are given for every previous book
       from the same category. Popular books also get
       their normal borrow count as extra points.
    */
    return (history_score * 10) + b->count;
}

void recommend(void)
{
    int id;
    int total = 0;
    int i, j;
    int limit;
    stu *s;
    book *p;
    book **books;
    int *score;

    printf("Enter student id: ");
    scanf("%d", &id);

    s = find_stu(id);

    if (s == NULL)
    {
        printf("Student not found.\n");
        return;
    }

    for (p = head; p != NULL; p = p->next)
    {
        if (!already_borrowed(s, p->id))
            total++;
    }

    if (total == 0)
    {
        printf("No new books to recommend.\n");
        return;
    }

    books = malloc(total * sizeof(book *));
    score = malloc(total * sizeof(int));

    if (books == NULL || score == NULL)
    {
        free(books);
        free(score);
        printf("Memory allocation failed.\n");
        return;
    }

    i = 0;

    for (p = head; p != NULL; p = p->next)
    {
        if (!already_borrowed(s, p->id))
        {
            books[i] = p;
            score[i] = book_score(s, p);
            i++;
        }
    }

    /* Sort the recommendations by score. */
    for (i = 0; i < total - 1; i++)
    {
        int best = i;

        for (j = i + 1; j < total; j++)
        {
            if (score[j] > score[best])
                best = j;
        }

        if (best != i)
        {
            book *temp_book = books[i];
            int temp_score = score[i];

            books[i] = books[best];
            score[i] = score[best];

            books[best] = temp_book;
            score[best] = temp_score;
        }
    }

    printf("\n--- Recommendations for %s ---\n", s->name);

    if (s->hist == NULL)
        printf("No borrowing history. Showing popular choices.\n");

    limit = total < MAX_RECOMMEND ? total : MAX_RECOMMEND;

    for (i = 0; i < limit; i++)
    {
        printf("%d. %s | %s | %s | %s\n",
               i + 1,
               books[i]->title,
               books[i]->author,
               books[i]->cat,
               books[i]->avail ? "Available" : "Issued");
    }

    free(books);
    free(score);
}
