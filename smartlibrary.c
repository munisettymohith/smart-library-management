#include <stdio.h>
#include "library.h"

#define rate 5

float calc_fine(int days)
{
    if (days <= 0)
        return 0;

    return days * rate;
}

void show_pop(void)
{
    book *t;
    int n = 0;

    if (head == NULL)
    {
        printf("no books\n");
        return;
    }

    sort_books(2);

    printf("\n--- top books ---\n");

    for (t = head; t != NULL && n < 5; t = t->next)
    {
        n++;
        printf("%d. %s | by %s | borrows: %d\n", n, t->title, t->author, t->count);
    }
}

void stats(void)
{
    book *t;
    int all = 0;
    int out = 0;
    int borrows = 0;

    for (t = head; t != NULL; t = t->next)
    {
        all++;
        borrows += t->count;

        if (!t->avail)
            out++;
    }

    printf("\n--- stats ---\n");
    printf("books       : %d\n", all);
    printf("out         : %d\n", out);
    printf("in          : %d\n", all - out);
    printf("students    : %d\n", count_stu());
    printf("borrowing   : %d\n", count_busy());
    printf("all borrows : %d\n", borrows);
}

void test_fine(void)
{
    int t[] = {0, 1, 3, 5, 10};
    int i;

    printf("\n--- fine test ---\n");

    for (i = 0; i < 5; i++)
        printf("late %d days -> rs %.2f\n", t[i], calc_fine(t[i]));
}

int main(void)
{
    int ch = -1;

    load_all();

    while (ch != 0)
    {
        printf("\n===== smart library =====\n");
        printf("1. add book         11. add student\n");
        printf("2. show books       12. show students\n");
        printf("3. edit book        13. edit student\n");
        printf("4. delete book      14. delete student\n");
        printf("5. find by id       15. issue book\n");
        printf("6. find by title    16. return book\n");
        printf("7. find by author   17. show queue\n");
        printf("8. find by category 18. top books\n");
        printf("9. sort by title    19. stats\n");
        printf("10. sort by borrows 20. test fine\n");
        printf("21. student books   22. recommend\n");
        printf("23. binary search   24. save data\n");
        printf("0. exit\n");

        printf("\nchoice: ");

        if (scanf("%d", &ch) != 1)
            break;

        switch (ch)
        {
            case 1: add_book(); break;
            case 2: show_books(); break;
            case 3: edit_book(); break;
            case 4: del_book(); break;
            case 5: by_id(); break;
            case 6: find_text(1); break;
            case 7: find_text(2); break;
            case 8: find_text(3); break;
            case 9: sort_books(1); printf("sorted by title\n"); break;
            case 10: sort_books(2); printf("sorted by borrows\n"); break;
            case 11: add_stu(); break;
            case 12: show_stu(); break;
            case 13: edit_stu(); break;
            case 14: del_stu(); break;
            case 15: issue_book(); break;
            case 16: return_book(); break;
            case 17: show_queue(); break;
            case 18: show_pop(); break;
            case 19: stats(); break;
            case 20: test_fine(); break;
            case 21: show_mine(); break;
            case 22: recommend(); break;
            case 23: bin_title(); break;
            case 24: save_all(); break;
            case 0: save_all(); printf("bye\n"); break;
            default: printf("wrong choice\n");
        }
    }

    return 0;
}
