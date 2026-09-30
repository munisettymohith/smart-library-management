# Data structures and complexity analysis

Smart Library Management and Book Recommendation System (C)

## Symbols used

| symbol | meaning |
|--------|---------|
| n | number of books |
| s | number of students |
| h | number of books in one student's history |
| L | number of entries in the transaction log |
| q | number of students in the waiting queue |
| c | number of different categories (at most 50) |
| k | number of recommendations shown (5) |

Title, author and name text is at most 100 characters, so text length is treated as a constant.

## 1. Data structures and why each was chosen

| data structure | used for | why it fits the problem |
|----------------|----------|-------------------------|
| singly linked list with a tail pointer | books | books are added and deleted all the time and the number is not fixed; deleting only re-links a pointer, nothing is shifted; the tail pointer makes adding at the end O(1); merge sort works naturally on a linked list |
| singly linked list | students | same reason as books; new students are added at the front in O(1) |
| singly linked list (one per student) | borrowing history | a student can borrow any number of books; the newest borrow goes on the front in O(1) |
| singly linked list with a tail pointer | transaction log | entries are only ever added at the end, so O(1) append; no fixed limit on how many transactions are kept |
| circular linked list with a rear pointer | waiting queue | the queue must be first come, first served; the rear pointer gives O(1) add at the back and the front is always rear->next |
| array of pointers | binary search on titles | binary search must jump to the middle, which a linked list cannot do, so the book pointers are copied into an array after sorting |
| array | recommendation candidates | a simple list of (book, score) pairs that is scored once and read once |

## 2. Algorithms

| algorithm | time | extra space | in plain words |
|-----------|------|-------------|----------------|
| find a book by id (`find_book`) | O(n) | O(1) | walk the list until the id matches |
| add a book | O(n) | O(1) | the O(1) append is preceded by an O(n) check that the id is not already used |
| delete a book | O(n) | O(1) | walk to the book, then re-link the pointer around it |
| search by title, author or category | O(n) | O(1) | check every book; the text match ignores capitals and can match part of the text |
| merge sort (by title, author, category or borrows) | O(n log n) best, average and worst | O(n) | split the list in half again and again, then merge the sorted halves; the recursive merge uses stack space proportional to the list length |
| binary search on titles | O(log n) for the search step | O(n) | look at the middle title and throw away the half that cannot contain the answer |
| binary search menu option, whole call | O(n log n) | O(n) | each call first sorts the books by title, then copies pointers into an array, then searches |
| find a student by id (`find_stu`) | O(s) | O(1) | walk the student list |
| add a student | O(s) | O(1) | duplicate-id check O(s), then add at the front in O(1) |
| add to history / mark as returned | O(s) and O(s + h) | O(1) | find the student first, then add at the front (O(1)) or find the entry (O(h)) |
| issue a book | O(s + n) | O(1) | find the student, find the book, append to the log (O(1)), add to history |
| return a book | O(L + n + s + h + q) | O(1) | find the open log entry, find the book and student, update history, then look in the queue |
| queue: add a waiting student | O(1) | O(1) | the rear pointer puts the new entry at the back straight away |
| queue: find the first student waiting for a book | O(q) | O(1) | walk the circle from the front until a matching book id is found |
| fine for one late return (`calc_fine`) | O(1) | O(1) | days late x rs 5 |
| total fines of one student (`fine_of`) | O(L) | O(1) | add up the fine of every late returned entry in the log |
| show all students with fines | O(s x L) | O(1) | `fine_of` runs once for each student |
| top books | O(n log n) | O(n) | sort by borrows (merge sort), print the first 5 |
| recommendation | O(n x h) | O(n) | see the steps below |
| statistics | O(n + s) | O(1) | one pass over books and one over students |
| save data | O(n + s + h + L + q) | O(1) | write every list to its file once |
| load data | O(n squared + s squared) | O(1) | every loaded record walks the list once to check for a repeated id or to reach the end |

### Recommendation steps

1. Count how many books the student took from each category: for each of the h history entries find the book (O(n)) and update the category counts (O(c)), so O(h x (n + c)).
2. Score every book the student has not taken: 10 points for each earlier borrow in the same category, plus the number of times the book has been borrowed. This costs O(n x (h + c)).
3. Pick the best k books by repeatedly choosing the highest score (selection): O(k x n).

Together this is O(n x h), because c is at most 50 and k is fixed at 5. The candidate list needs O(n) space.

## 3. Known limits

These are honest points to state in the report or viva rather than surprises to be found:

- **Binary search sorts every time.** The search itself is O(log n), but the menu option sorts by title first, so one call costs O(n log n). It also leaves the book list in title order afterwards.
- **Loading is slow for very large files.** Each record is checked against the list, so loading n books takes O(n squared). This is fine for a college library size and would be the first thing to improve with a hash table.
- **Lookups by id are linear.** Finding a book or a student by id walks the list, which is O(n) or O(s). A hash table or a balanced tree would make this O(1) or O(log n), but a linked list was required by the project brief.
- **Recursive merge.** The merge step calls itself once for each node it places, so a very long list uses a lot of stack space. For the sizes in this project this is not a problem.
