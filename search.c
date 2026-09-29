#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

static int get_book_count(void)
{
    int n = 0;
    book *p = head;

    while (p != NULL)
    {
        n++;
        p = p->next;
    }

    return n;
}

static void print_result(book *p)
{
    printf("\nID       : %d", p->id);
    printf("\nTitle    : %s", p->title);
    printf("\nAuthor   : %s", p->author);
    printf("\nCategory : %s", p->cat);
    printf("\nStatus   : %s", p->avail ? "Available" : "Issued");
    printf("\nBorrows  : %d\n", p->count);
}

void bin_title(void)
{
    int n, left, right, mid, found = -1;
    int i;
    char title[100];
    book *p;
    book **list;

    n = get_book_count();

    if (n == 0)
    {
        printf("No books available.\n");
        return;
    }

    list = malloc(n * sizeof(book *));

    if (list == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter title: ");
    scanf(" %99[^\n]", title);

    /* Binary search needs the titles in sorted order. */
    sort_books(1);

    p = head;
    i = 0;

    while (p != NULL)
    {
        list[i] = p;
        i++;
        p = p->next;
    }

    left = 0;
    right = n - 1;

    while (left <= right)
    {
        int result;

        mid = (left + right) / 2;
        result = strcmp(list[mid]->title, title);

        if (result == 0)
        {
            found = mid;
            break;
        }
        else if (result < 0)
            left = mid + 1;
        else
            right = mid - 1;
    }

    if (found == -1)
    {
        printf("Book not found.\n");
    }
    else
    {
        int first = found;
        int last = found;

        while (first > 0 && strcmp(list[first - 1]->title, title) == 0)
            first--;

        while (last < n - 1 && strcmp(list[last + 1]->title, title) == 0)
            last++;

        for (i = first; i <= last; i++)
            print_result(list[i]);
    }

    free(list);
}
