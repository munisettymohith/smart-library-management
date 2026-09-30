# Individual contribution statement

**Project:** Smart Library Management and Book Recommendation System (C)
**Name:** Tanuhya
**Roll number:** A25126510211
**Team:** bhashitha, Tanuhya, prakash, mohith

## My role

Member 2: student management and issue / return, including the waiting queue.

## Files I own

| file | lines | what it holds |
|------|-------|---------------|
| `student.c` | 329 | student list, each student's borrowing history, fine totals shown per student |
| `issue_return.c` | 419 | issue and return, transaction log, waiting queue, fine totals |

## What I built

**Student management (`student.c`)**
- add, show, edit and delete students, kept in a singly linked list (newest first)
- a singly linked borrowing history for every student: a new borrow is added at the front, and a return marks the entry as returned
- `show_mine` lists every book a student has taken, with its status and the student's total fines
- loading functions used when the program starts

**Issue and return (`issue_return.c`)**
- `issue_book`: checks the student and the book, then records the loan in the transaction log, updates the book's status and borrow count, and adds it to the student's history
- `return_book`: finds the open log entry, closes it, works out days late and the fine (rs 5 per day after 14 days)
- transaction log as a singly linked list with a tail pointer, so a new entry is added in O(1) and there is no limit on its size
- waiting queue as a circular linked list with a rear pointer: a student asking for a book that is out joins the back of the queue
- when a book is returned it is issued straight to the first student waiting for it; students who have since been deleted are skipped
- the waiting queue is saved to `data/queue.txt` and restored on start
- `fine_of`: adds up a student's fines from the log, so nothing extra has to be stored
- number input checks: typing letters where a number is expected prints "please enter a number" instead of crashing or looping

## Data structures and complexity in my modules

| structure | used for | main cost |
|-----------|----------|-----------|
| singly linked list | students, history, transaction log | find a student O(s), append to log O(1) |
| circular linked list with rear pointer | waiting queue | add O(1), find first waiting for a book O(q) |

The full table is in `complexity-analysis.md`.

## Testing

My test cases are S1 to S5, I1 to I8 and I10 in `test-cases.md`. They cover adding and deleting students, a repeated student id, non-numeric input, issuing, an unavailable book, a missing student, on-time and late returns, a return with no matching issue, the waiting queue passing a book on, the queue surviving a restart, and student fine totals. All of them pass.

## What I can explain in the viva

- why the log is a linked list with a tail pointer, and why the queue is circular with a rear pointer
- how `issue_book` and `return_book` change the book list, the student's history and the log, step by step
- how the fine is calculated and why the total is worked out from the log instead of stored
- how the waiting queue is stored in a file and rebuilt in the same order
- the cost of each of these operations

## Acknowledgement

I used an AI assistant (Claude) while working on the project. It helped me review my code, add the input checks, and prepare drafts of the test cases, complexity analysis and design document. I ran the program and checked the results myself, and I have read and understood the code described above.
