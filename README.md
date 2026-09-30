# smart library management
smart library management and book recommendation system - dsa project

## files

| file | what it does |
|------|--------------|
| library.h | shared structs and function declarations |
| bookmanagement.c | book linked list, search by title / author / category, merge sort |
| student.c | student list, borrowing history, fine total shown per student |
| issue_return.c | issue / return, transaction log, waiting queue, fine total per student |
| smartlibrary.c | fine rate, stats, top books, menu, main |
| search.c | binary search on book titles |
| recommendation.c | book suggestions from a student's history |
| filehandling.c | save and load data files |
| data/ | books, students, history, logs, queue files |

## data structures

- singly linked list: books, students, borrowing history, transaction log
- circular linked list: waiting queue
- array of pointers: binary search on titles, recommendation candidates
- merge sort on the book list: by title, borrows, author or category
- binary search on titles

## how it works

- issue and return are written to the transaction log
- fine is rs 5 per day after a 14 day loan; each student's total fines are worked out from the log
- if a book is out, the student joins the waiting queue; when the book is returned it is issued to the first student waiting for it
- top books are the 5 most borrowed
- recommendations score each book the student has not taken: 10 for each earlier borrow in the same category, plus how popular the book is

## build

    gcc smartlibrary.c bookmanagement.c student.c issue_return.c search.c recommendation.c filehandling.c -o library

## run

run from the project folder so the program finds `data/`:

    ./library

data is loaded from `data/` on start and saved when you choose 0 (exit) or 24.
the waiting queue is saved too, in `data/queue.txt`, which is created on the first save.
