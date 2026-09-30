# Test cases and results

Smart Library Management and Book Recommendation System (C)

**Result: 34 of 34 test cases passed.**

## How the tests were run

- compiler: gcc, built with `-Wall -Wextra` (no warnings)
- build: `gcc smartlibrary.c bookmanagement.c student.c issue_return.c search.c recommendation.c filehandling.c -o library`
- every test starts from the same sample data in `data/` (12 books, 4 students, 9 log entries), so results can be repeated
- the program was run with the typed input shown in the Input column and its real output was recorded
- a test passes when every expected message appears in the output

## Book

| ID | Test | Input | Expected | Actual output | Result |
|----|------|-------|----------|---------------|--------|
| B1 | Add a new book | menu 1, id 113, title Operating Systems, author Tanenbaum, category Systems | book added | book added | Pass |
| B2 | Add a book with an id that already exists | menu 1, id 101 | id exists | id exists | Pass |
| B3 | Find a book by id | menu 5, id 101 | C Programming | C Programming | Pass |
| B4 | Find a book by an id that does not exist | menu 5, id 999 | not found | not found | Pass |
| B5 | Delete a book, then look for it | menu 4, id 111; menu 5, id 111 | book deleted; not found | book deleted; not found | Pass |

## Student

| ID | Test | Input | Expected | Actual output | Result |
|----|------|-------|----------|---------------|--------|
| S1 | Add a new student | menu 11, id 5, name Ravi Kumar | student added | student added | Pass |
| S2 | Add a student with an id already used | menu 11, id 1 | id already used | id already used | Pass |
| S3 | Type letters instead of a student id | menu 11, then abc | please enter a number | please enter a number | Pass |
| S4 | Delete a student | menu 14, id 4 | student deleted | student deleted | Pass |
| S5 | Show a student's borrowing history | menu 21, id 2 | book 107; with student; returned | total fines: rs 5.00; book 107 / Introduction to Algorithms / with student; book 103 / Algorithms Unlocked / returned | Pass |

## Issue/Return

| ID | Test | Input | Expected | Actual output | Result |
|----|------|-------|----------|---------------|--------|
| I1 | Issue an available book | menu 15, student 1, book 111, day 5 | book issued, due in 14 days | book issued, due in 14 days | Pass |
| I2 | Issue a book that is already out (joins queue) | menu 15, student 2, book 102 | book is out; added to queue | book is out; added to queue | Pass |
| I3 | Issue to a student that does not exist | menu 15, student 99 | no such student | no such student | Pass |
| I4 | Return a book on time | menu 16, student 3, book 104, day 30 (issued day 25) | book returned | book returned | Pass |
| I5 | Return a book late (21 days late) | menu 16, student 3, book 104, day 60 (issued day 25) | late by 21 days; rs 105.00 | book returned; late by 21 days, fine rs 105.00 | Pass |
| I6 | Return a book that was never issued to that student | menu 16, student 4, book 101 | no such issue | no such issue | Pass |

## Queue

| ID | Test | Input | Expected | Actual output | Result |
|----|------|-------|----------|---------------|--------|
| I7 | Returned book goes to the first waiting student | student 2 asks for 102 (out); student 1 returns 102 on day 20 | book issued to waiting student 2; queue empty | book issued to waiting student 2, due in 14 days; queue empty | Pass |
| I8 | Waiting queue is kept after restart | run 1: students 2 and 3 ask for 102, exit; run 2: menu 17 | student 2 wants book 102; student 3 wants book 102 | 1. student 2 wants book 102; 2. student 3 wants book 102 | Pass |

## Fine

| ID | Test | Input | Expected | Actual output | Result |
|----|------|-------|----------|---------------|--------|
| I9 | Fine table (0, 1, 3, 5, 10 days late) | menu 20 | late 10 days -> rs 50.00 | late 0 days -> rs 0.00; late 10 days -> rs 50.00 | Pass |
| I10 | Total fines for a student | menu 12 (student 2 returned one book 1 day late) | fines : rs 5.00 | name  : Kasun Silva; fines : rs 5.00 | Pass |

## Search

| ID | Test | Input | Expected | Actual output | Result |
|----|------|-------|----------|---------------|--------|
| Q1 | Partial, any-case title search | menu 6, text data | Data Structures in C; Database Systems | Data Structures in C; Database Systems | Pass |
| Q2 | Author search ignoring capitals | menu 7, text cormen | Algorithms Unlocked; Introduction to Algorithms | Algorithms Unlocked; Introduction to Algorithms | Pass |
| Q3 | Category search | menu 8, text PROGRAMMING | C Programming; Data Structures in C; Let Us C | C Programming; Data Structures in C; Let Us C | Pass |
| Q4 | Search with no match | menu 6, text zzz | not found | not found | Pass |
| Q5 | Binary search finds a title | menu 23, title C Programming | C Programming | C Programming | Pass |
| Q6 | Binary search, title not present | menu 23, title Nope | not found | not found | Pass |

## Sort

| ID | Test | Input | Expected | Actual output | Result |
|----|------|-------|----------|---------------|--------|
| Q7 | Sort by title, first book | menu 9, then menu 2 | Algorithms Unlocked | Algorithms Unlocked; Artificial Intelligence; C Programming | Pass |
| Q8 | Sort by borrows, most borrowed first | menu 10, then menu 2 | C Programming | C Programming; Algorithms Unlocked; Computer Networks | Pass |
| Q9 | Sort by author, first book | menu 25, then menu 2 | Operating System Concepts | Operating System Concepts; Computer Networks; C Programming | Pass |
| Q10 | Sort by category, first book | menu 26, then menu 2 | Artificial Intelligence | Artificial Intelligence; Algorithms Unlocked; Introduction to Algorithms | Pass |

## Popular

| ID | Test | Input | Expected | Actual output | Result |
|----|------|-------|----------|---------------|--------|
| R1 | Top books list | menu 18 | 1. C Programming | 1. C Programming / by Dennis Ritchie / borrows: 3; 2. Algorithms Unlocked / by Thomas Cormen / borrows: 1; 3. Computer Networks / by Andrew Tanenbaum / borrows: 1 | Pass |

## Recommend

| ID | Test | Input | Expected | Actual output | Result |
|----|------|-------|----------|---------------|--------|
| R2 | Recommendations for student 1 (has read C Programming, Data Structures in C) | menu 22, student 1 | 1. Let Us C | --- picks for Nimal Perera ---; 1. Let Us C / by Yashavant Kanetkar / Programming / in; 2. Computer Networks / by Andrew Tanenbaum / Networking / in | Pass |

## Stats

| ID | Test | Input | Expected | Actual output | Result |
|----|------|-------|----------|---------------|--------|
| R3 | Library statistics | menu 19 | books       : 12; students    : 4 | books       : 12; students    : 4; all borrows : 9 | Pass |

## File

| ID | Test | Input | Expected | Actual output | Result |
|----|------|-------|----------|---------------|--------|
| P1 | Data is kept after restart | run 1: add student 5 Ravi Kumar, exit; run 2: menu 12 | Ravi Kumar | name  : Ravi Kumar | Pass |

## Notes

- fine is rs 5 per day late, after a 14 day loan; I5: returned 60 - 25 - 14 = 21 days late, 21 x 5 = rs 105
- I10 shows rs 5.00 because the sample data has one book returned 1 day late for student 2
- R2: student 1 has taken C Programming and Data Structures in C, so Programming books score highest and Let Us C is first
- tests P1 and I8 use two program runs to prove data and the waiting queue are kept after exit