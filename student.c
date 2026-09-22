#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

StudentNode *studentHead = NULL;
static StudentNode *studentTail = NULL;

StudentNode *search_student_by_id(int student_id)
{
    StudentNode *temp = studentHead;

    while (temp != NULL)
    {
        if (temp->data.student_id == student_id)
            return temp;

        temp = temp->next;
    }

    return NULL;
}

void add_student(void)
{
    StudentNode *newNode = (StudentNode *)malloc(sizeof(StudentNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("\nEnter Student ID: ");
    scanf("%d", &newNode->data.student_id);

    if (search_student_by_id(newNode->data.student_id) != NULL)
    {
        printf("Student ID already exists.\n");
        free(newNode);
        return;
    }

    getchar();

    printf("Enter Student Name: ");
    fgets(newNode->data.name, sizeof(newNode->data.name), stdin);
    newNode->data.name[strcspn(newNode->data.name, "\n")] = '\0';

    newNode->data.borrowed_count = 0;
    newNode->next = NULL;

    if (studentHead == NULL)
    {
        studentHead = newNode;
        studentTail = newNode;
    }
    else
    {
        studentTail->next = newNode;
        studentTail = newNode;
    }

    printf("Student added successfully.\n");
}

void display_students(void)
{
    StudentNode *temp = studentHead;

    if (studentHead == NULL)
    {
        printf("\nNo students registered.\n");
        return;
    }

    printf("\n========== STUDENT LIST ==========\n");

    while (temp != NULL)
    {
        printf("\nStudent ID     : %d", temp->data.student_id);
        printf("\nName           : %s", temp->data.name);
        printf("\nBorrowed Count : %d\n", temp->data.borrowed_count);
        printf("-----------------------------------\n");

        temp = temp->next;
    }
}

void update_student(void)
{
    int id;
    StudentNode *node;

    printf("\nEnter Student ID to update: ");
    scanf("%d", &id);

    node = search_student_by_id(id);

    if (node == NULL)
    {
        printf("Student not found.\n");
        return;
    }

    getchar();

    printf("Enter new name: ");
    fgets(node->data.name, sizeof(node->data.name), stdin);
    node->data.name[strcspn(node->data.name, "\n")] = '\0';

    printf("Student updated successfully.\n");
}

void delete_student(void)
{
    int id;
    StudentNode *temp = studentHead;
    StudentNode *prev = NULL;

    printf("\nEnter Student ID to delete: ");
    scanf("%d", &id);

    while (temp != NULL && temp->data.student_id != id)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("Student not found.\n");
        return;
    }

    if (prev == NULL)
        studentHead = temp->next;
    else
        prev->next = temp->next;

    if (temp == studentTail)
        studentTail = prev;

    free(temp);

    printf("Student deleted successfully.\n");
}

/* delta is +1 when a book is issued, -1 when returned */
void change_borrowed_count(int student_id, int delta)
{
    StudentNode *node = search_student_by_id(student_id);

    if (node != NULL)
        node->data.borrowed_count += delta;
}
