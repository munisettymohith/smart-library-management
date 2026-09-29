#ifndef STUDENT_H
#define STUDENT_H

typedef struct Student
{
    int student_id;
    char name[100];
    int borrowed_count;
} Student;

typedef struct StudentNode
{
    Student data;
    struct StudentNode *next;
} StudentNode;


extern StudentNode *studentHead;

void add_student(void);
void display_students(void);
StudentNode *search_student_by_id(int student_id);
void update_student(void);
void delete_student(void);


void change_borrowed_count(int student_id, int delta);

#endif
