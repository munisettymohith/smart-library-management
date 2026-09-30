#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"



static int list_size(void)
{
    int n = 0;
    book *t;

    for (t = head; t != NULL; t = t->next)
        n++;

    return n;
}

static void print_book(book *b)
{
    printf("\nid     : %d", b->id);
    printf("\ntitle  : %s", b->title);
    printf("\nauthor : %s", b->author);
    printf("\ncat    : %s", b->cat);
    printf("\nstatus : %s", b->avail ? "in" : "out");
    printf("\ncount  : %d\n", b->count);
}


static int first_match(book **arr, int n, const char *x)
{
    int low = 0;
    int high = n - 1;
    int found = -1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        int c = strcmp(arr[mid]->title, x);

        if (c == 0)
        {
            found = mid;
            high = mid - 1;     
        }
        else if (c < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return found;
}

void bin_title(void)
{
    int n = list_size();
    int i = 0;
    int pos;
    char x[100];
    book *t;
    book **arr;

    if (n == 0)
    {
        printf("\nno books\n");
        return;
    }

    arr = malloc(n * sizeof(book *));

    if (arr == NULL)
    {
        printf("no memory\n");
        return;
    }

    printf("\ntitle to search: ");
    scanf(" %99[^\n]", x);

    sort_books(1);      

    for (t = head; t != NULL; t = t->next)
        arr[i++] = t;

    pos = first_match(arr, n, x);

    if (pos == -1)
        printf("not found\n");
    else
    {
        
        while (pos < n && strcmp(arr[pos]->title, x) == 0)
        {
            print_book(arr[pos]);
            pos++;
        }
    }

    free(arr);
}
