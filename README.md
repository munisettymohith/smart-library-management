# smart library management
smart library management and book recommendation system - dsa project

## files

| file | what it does |
|------|--------------|
| library.h | shared structs and function declarations |
| bookmanagement.c | book linked list, search, merge sort |
| student.c | student list and borrowing history |
| issue_return.c | issue / return, transaction log, waiting queue |
| smartlibrary.c | fine, stats, top books, menu, main |
| search.c | binary search on book titles |
| recommendation.c | book suggestions from a student's history |
| filehandling.c | save and load data files |
| data/ | sample data (books, students, history, logs) |

## data structures

- singly linked list: books, students, borrowing history
- circular linked list: waiting queue
- array: transaction log, binary search
- merge sort on the book list
- binary search on titles

## build

    gcc smartlibrary.c bookmanagement.c student.c issue_return.c search.c recommendation.c filehandling.c -o library

## run

run from the project folder so the program finds `data/`:

    ./library

data is loaded from `data/` on start and saved when you choose 0 (exit) or 24.
the waiting queue is not saved.
