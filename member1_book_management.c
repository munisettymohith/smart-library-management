
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book
{
    int bookId;
    char title[100];
    char author[100];
    char category[50];
    int available;
    int borrowCount;
    struct Book *next;
} Book;

Book *head = NULL;

/* Find a book using its ID */
Book *getBookById(int id)
{
    Book *temp = head;

    while (temp != NULL)
    {
        if (temp->bookId == id)
            return temp;

        temp = temp->next;
    }

    return NULL;
}

/* Add a new book */
void addBook()
{
    Book *newBook, *temp;

    newBook = (Book *)malloc(sizeof(Book));

    if (newBook == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("\nEnter Book ID: ");
    scanf("%d", &newBook->bookId);

    if (getBookById(newBook->bookId) != NULL)
    {
        printf("Book ID already exists.\n");
        free(newBook);
        return;
    }

    getchar();

    printf("Enter Book Title: ");
    fgets(newBook->title, sizeof(newBook->title), stdin);
    newBook->title[strcspn(newBook->title, "\n")] = '\0';

    printf("Enter Author: ");
    fgets(newBook->author, sizeof(newBook->author), stdin);
    newBook->author[strcspn(newBook->author, "\n")] = '\0';

    printf("Enter Category: ");
    fgets(newBook->category, sizeof(newBook->category), stdin);
    newBook->category[strcspn(newBook->category, "\n")] = '\0';

    newBook->available = 1;
    newBook->borrowCount = 0;
    newBook->next = NULL;

    if (head == NULL)
    {
        head = newBook;
    }
    else
    {
        temp = head;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newBook;
    }

    printf("Book added successfully.\n");
}

/* Display all books */
void displayBooks()
{
    Book *temp = head;

    if (head == NULL)
    {
        printf("\nNo books available.\n");
        return;
    }

    printf("\n========== BOOK LIST ==========\n");

    while (temp != NULL)
    {
        printf("\nBook ID      : %d", temp->bookId);
        printf("\nTitle        : %s", temp->title);
        printf("\nAuthor       : %s", temp->author);
        printf("\nCategory     : %s", temp->category);
        printf("\nAvailability : %s",
               temp->available ? "Available" : "Issued");
        printf("\nBorrow Count : %d\n", temp->borrowCount);
        printf("-------------------------------\n");

        temp = temp->next;
    }
}

/* Update book details */
void updateBook()
{
    int id;
    Book *book;

    printf("\nEnter Book ID to update: ");
    scanf("%d", &id);

    book = getBookById(id);

    if (book == NULL)
    {
        printf("Book not found.\n");
        return;
    }

    getchar();

    printf("Enter new title: ");
    fgets(book->title, sizeof(book->title), stdin);
    book->title[strcspn(book->title, "\n")] = '\0';

    printf("Enter new author: ");
    fgets(book->author, sizeof(book->author), stdin);
    book->author[strcspn(book->author, "\n")] = '\0';

    printf("Enter new category: ");
    fgets(book->category, sizeof(book->category), stdin);
    book->category[strcspn(book->category, "\n")] = '\0';

    printf("Book updated successfully.\n");
}

/* Delete a book */
void deleteBook()
{
    int id;
    Book *temp = head;
    Book *prev = NULL;

    printf("\nEnter Book ID to delete: ");
    scanf("%d", &id);

    while (temp != NULL && temp->bookId != id)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("Book not found.\n");
        return;
    }

    if (prev == NULL)
        head = temp->next;
    else
        prev->next = temp->next;

    free(temp);

    printf("Book deleted successfully.\n");
}

/* Linear search by ID */
void searchById()
{
    int id;
    Book *temp = head;

    printf("\nEnter Book ID: ");
    scanf("%d", &id);

    while (temp != NULL)
    {
        if (temp->bookId == id)
        {
            printf("\nBook Found\n");
            printf("ID       : %d\n", temp->bookId);
            printf("Title    : %s\n", temp->title);
            printf("Author   : %s\n", temp->author);
            printf("Category : %s\n", temp->category);
            return;
        }

        temp = temp->next;
    }

    printf("Book not found.\n");
}

/* Linear search by title */
void searchByTitle()
{
    char title[100];
    Book *temp = head;

    getchar();

    printf("\nEnter title: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0';

    while (temp != NULL)
    {
        if (strcmp(temp->title, title) == 0)
        {
            printf("\nBook Found\n");
            printf("ID       : %d\n", temp->bookId);
            printf("Title    : %s\n", temp->title);
            printf("Author   : %s\n", temp->author);
            printf("Category : %s\n", temp->category);
            return;
        }

        temp = temp->next;
    }

    printf("Book not found.\n");
}

/* Linear search by author */
void searchByAuthor()
{
    char author[100];
    Book *temp = head;

    getchar();

    printf("\nEnter author: ");
    fgets(author, sizeof(author), stdin);
    author[strcspn(author, "\n")] = '\0';

    while (temp != NULL)
    {
        if (strcmp(temp->author, author) == 0)
        {
            printf("\nBook Found\n");
            printf("ID       : %d\n", temp->bookId);
            printf("Title    : %s\n", temp->title);
            printf("Author   : %s\n", temp->author);
            printf("Category : %s\n", temp->category);
            return;
        }

        temp = temp->next;
    }

    printf("Book not found.\n");
}

/* Linear search by category */
void searchByCategory()
{
    char category[50];
    Book *temp = head;
    int found = 0;

    getchar();

    printf("\nEnter category: ");
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = '\0';

    while (temp != NULL)
    {
        if (strcmp(temp->category, category) == 0)
        {
            printf("\nID: %d | Title: %s | Author: %s\n",
                   temp->bookId, temp->title, temp->author);
            found = 1;
        }

        temp = temp->next;
    }

    if (!found)
        printf("No books found in this category.\n");
}

/* Sort by title using bubble sort */
void sortByTitle()
{
    Book *i, *j;
    Book *last = NULL;

    if (head == NULL || head->next == NULL)
    {
        printf("Not enough books to sort.\n");
        return;
    }

    while (last != head)
    {
        i = head;

        while (i->next != last)
        {
            j = i->next;

            if (strcmp(i->title, j->title) > 0)
            {
                int id = i->bookId;
                int available = i->available;
                int borrowCount = i->borrowCount;
                char title[100], author[100], category[50];

                strcpy(title, i->title);
                strcpy(author, i->author);
                strcpy(category, i->category);

                i->bookId = j->bookId;
                i->available = j->available;
                i->borrowCount = j->borrowCount;
                strcpy(i->title, j->title);
                strcpy(i->author, j->author);
                strcpy(i->category, j->category);

                j->bookId = id;
                j->available = available;
                j->borrowCount = borrowCount;
                strcpy(j->title, title);
                strcpy(j->author, author);
                strcpy(j->category, category);
            }

            i = i->next;
        }

        last = i;
    }

    printf("Books sorted by title.\n");
}

/* Sort by popularity */
void sortByPopularity()
{
    Book *i, *j;
    Book *last = NULL;

    if (head == NULL || head->next == NULL)
    {
        printf("Not enough books to sort.\n");
        return;
    }

    while (last != head)
    {
        i = head;

        while (i->next != last)
        {
            j = i->next;

            if (i->borrowCount < j->borrowCount)
            {
                int id = i->bookId;
                int available = i->available;
                int borrowCount = i->borrowCount;
                char title[100], author[100], category[50];

                strcpy(title, i->title);
                strcpy(author, i->author);
                strcpy(category, i->category);

                i->bookId = j->bookId;
                i->available = j->available;
                i->borrowCount = j->borrowCount;
                strcpy(i->title, j->title);
                strcpy(i->author, j->author);
                strcpy(i->category, j->category);

                j->bookId = id;
                j->available = available;
                j->borrowCount = borrowCount;
                strcpy(j->title, title);
                strcpy(j->author, author);
                strcpy(j->category, category);
            }

            i = i->next;
        }

        last = i;
    }

    printf("Books sorted by popularity.\n");
}

/* Main function */
int main()
{
    int choice;

    do
    {
        printf("\n===== BOOK MANAGEMENT =====\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Update Book\n");
        printf("4. Delete Book\n");
        printf("5. Search by ID\n");
        printf("6. Search by Title\n");
        printf("7. Search by Author\n");
        printf("8. Search by Category\n");
        printf("9. Sort by Title\n");
        printf("10. Sort by Popularity\n");
        printf("0. Exit\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: updateBook(); break;
            case 4: deleteBook(); break;
            case 5: searchById(); break;
            case 6: searchByTitle(); break;
            case 7: searchByAuthor(); break;
            case 8: searchByCategory(); break;
            case 9: sortByTitle(); break;
            case 10: sortByPopularity(); break;
            case 0: printf("Program ended.\n"); break;
            default: printf("Invalid choice.\n");
        }

    } while (choice != 0);

    return 0;
}
