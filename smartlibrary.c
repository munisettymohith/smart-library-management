#include <stdio.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_STUDENTS 100
#define FINE_RATE 5

typedef struct
{
    int id;
    char title[100];
    char author[100];
    char category[50];
    int available;
    int borrowCount;
} Book;

typedef struct
{
    int id;
    char name[100];
    int borrowing;
} Student;

Book books[MAX_BOOKS];
Student students[MAX_STUDENTS];

int bookCount = 0;
int studentCount = 0;

/* Calculate Fine */

float calculateFine(int lateDays)
{
    if(lateDays <= 0)
        return 0;

    return lateDays * FINE_RATE;
}

/* Find Book */

int findBook(int id)
{
    int i;

    for(i = 0; i < bookCount; i++)
    {
        if(books[i].id == id)
            return i;
    }

    return -1;
}

/* Add Book */

void addBook()
{
    if(bookCount >= MAX_BOOKS)
    {
        printf("Book limit reached.\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d",&books[bookCount].id);

    printf("Enter Book Title: ");
    scanf(" %[^\n]",books[bookCount].title);

    printf("Enter Author: ");
    scanf(" %[^\n]",books[bookCount].author);

    printf("Enter Category: ");
    scanf(" %[^\n]",books[bookCount].category);

    books[bookCount].available = 1;
    books[bookCount].borrowCount = 0;

    bookCount++;

    printf("Book added successfully.\n");
}

/* Add Student */

void addStudent()
{
    if(studentCount >= MAX_STUDENTS)
    {
        printf("Student limit reached.\n");
        return;
    }

    printf("Enter Student ID: ");
    scanf("%d",&students[studentCount].id);

    printf("Enter Student Name: ");
    scanf(" %[^\n]",students[studentCount].name);

    students[studentCount].borrowing = 0;

    studentCount++;

    printf("Student added successfully.\n");
}

/* Issue Book */

void issueBook()
{
    int bookId;
    int index;

    printf("Enter Book ID to issue: ");
    scanf("%d",&bookId);

    index = findBook(bookId);

    if(index == -1)
    {
        printf("Book not found.\n");
        return;
    }

    if(books[index].available == 0)
    {
        printf("Book is already issued.\n");
        return;
    }

    books[index].available = 0;

    books[index].borrowCount++;

    printf("Book issued successfully.\n");
    printf("Borrow count of %s = %d\n",
           books[index].title,
           books[index].borrowCount);
}

/* Return Book and Fine */

void returnBook()
{
    int bookId;
    int lateDays;
    int index;
    float fine;

    printf("Enter Book ID to return: ");
    scanf("%d",&bookId);

    index = findBook(bookId);

    if(index == -1)
    {
        printf("Book not found.\n");
        return;
    }

    if(books[index].available == 1)
    {
        printf("Book is not currently issued.\n");
        return;
    }

    printf("Enter number of late days: ");
    scanf("%d",&lateDays);

    fine = calculateFine(lateDays);

    books[index].available = 1;

    printf("\nBook returned successfully.\n");
    printf("Late Days : %d\n",lateDays);
    printf("Fine Rate : Rs.%d/day\n",FINE_RATE);
    printf("Fine      : Rs.%.2f\n",fine);
}

/* Swap Books */

void swapBooks(Book *a, Book *b)
{
    Book temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

/* Sort Books by Popularity */

void sortPopularBooks()
{
    int i,j;

    for(i = 0; i < bookCount - 1; i++)
    {
        for(j = 0; j < bookCount - i - 1; j++)
        {
            if(books[j].borrowCount < books[j+1].borrowCount)
            {
                swapBooks(&books[j],&books[j+1]);
            }
        }
    }
}

/* Display Popular Books */

void displayPopularBooks()
{
    int i;

    if(bookCount == 0)
    {
        printf("No books available.\n");
        return;
    }

    sortPopularBooks();

    printf("\n========== POPULAR BOOKS ==========\n");

    for(i = 0; i < bookCount; i++)
    {
        printf("%d. %s | Author: %s | Borrows: %d\n",
               i + 1,
               books[i].title,
               books[i].author,
               books[i].borrowCount);
    }
}

/* Library Statistics */

void displayStatistics()
{
    int i;
    int issuedBooks = 0;
    int availableBooks = 0;
    int totalBorrowings = 0;
    int activeStudents = 0;

    for(i = 0; i < bookCount; i++)
    {
        if(books[i].available == 0)
            issuedBooks++;
        else
            availableBooks++;

        totalBorrowings += books[i].borrowCount;
    }

    for(i = 0; i < studentCount; i++)
    {
        if(students[i].borrowing == 1)
            activeStudents++;
    }

    printf("\n========== LIBRARY STATISTICS ==========\n");

    printf("Total Books        : %d\n",bookCount);
    printf("Issued Books       : %d\n",issuedBooks);
    printf("Available Books    : %d\n",availableBooks);
    printf("Total Students     : %d\n",studentCount);
    printf("Borrowing Students : %d\n",activeStudents);
    printf("Total Borrowings   : %d\n",totalBorrowings);
}

/* Display Books */

void displayBooks()
{
    int i;

    if(bookCount == 0)
    {
        printf("No books available.\n");
        return;
    }

    printf("\n========== BOOK LIST ==========\n");

    for(i = 0; i < bookCount; i++)
    {
        printf("\nBook ID      : %d",books[i].id);
        printf("\nTitle        : %s",books[i].title);
        printf("\nAuthor       : %s",books[i].author);
        printf("\nCategory     : %s",books[i].category);
        printf("\nStatus       : %s",
               books[i].available ? "Available" : "Issued");
        printf("\nBorrow Count : %d\n",books[i].borrowCount);
    }
}

/* Fine Testing */

void testFineCalculation()
{
    int testCases[] = {0,1,3,5,10};
    int i;

    printf("\n========== FINE TESTING ==========\n");

    for(i = 0; i < 5; i++)
    {
        printf("Late Days: %d -> Fine: Rs.%.2f\n",
               testCases[i],
               calculateFine(testCases[i]));
    }
}

/* Main */

int main()
{
    int choice;

    do
    {
        printf("\n\n====================================\n");
        printf(" SMART LIBRARY - MEMBER 3 MODULE\n");
        printf("====================================\n");

        printf("1. Add Book\n");
        printf("2. Add Student\n");
        printf("3. Issue Book\n");
        printf("4. Return Book & Calculate Fine\n");
        printf("5. Display Books\n");
        printf("6. Popular Books\n");
        printf("7. Library Statistics\n");
        printf("8. Test Fine Calculation\n");
        printf("9. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                addBook();
                break;

            case 2:
                addStudent();
                break;

            case 3:
                issueBook();
                break;

            case 4:
                returnBook();
                break;

            case 5:
                displayBooks();
                break;

            case 6:
                displayPopularBooks();
                break;

            case 7:
                displayStatistics();
                break;

            case 8:
                testFineCalculation();
                break;

            case 9:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    }while(choice != 9);

    return 0;
}
