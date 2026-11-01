/*
 * Student Record Manager
 * A console app in C to add, view, search, and delete student records.
 * Records are saved to students.dat so data persists between runs.
 */
#include <stdio.h>
#include <string.h>

#define MAX 100
#define FILE_NAME "students.dat"

typedef struct {
    int roll;
    char name[50];
    float marks;
} Student;

Student students[MAX];
int count = 0;

void loadFromFile(void) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) return;
    count = (int)fread(students, sizeof(Student), MAX, fp);
    fclose(fp);
}

void saveToFile(void) {
    FILE *fp = fopen(FILE_NAME, "wb");
    if (fp == NULL) {
        printf("Error: could not save data.\n");
        return;
    }
    fwrite(students, sizeof(Student), count, fp);
    fclose(fp);
}

int findByRoll(int roll) {
    for (int i = 0; i < count; i++)
        if (students[i].roll == roll) return i;
    return -1;
}

void addStudent(void) {
    if (count >= MAX) {
        printf("Database full.\n");
        return;
    }
    Student s;
    printf("Enter roll number: ");
    scanf("%d", &s.roll);
    if (findByRoll(s.roll) != -1) {
        printf("Roll number already exists.\n");
        return;
    }
    printf("Enter name: ");
    scanf(" %49[^\n]", s.name);
    printf("Enter marks (0-100): ");
    scanf("%f", &s.marks);
    if (s.marks < 0 || s.marks > 100) {
        printf("Invalid marks.\n");
        return;
    }
    students[count++] = s;
    saveToFile();
    printf("Student added.\n");
}

void viewAll(void) {
    if (count == 0) {
        printf("No records found.\n");
        return;
    }
    printf("\n%-8s %-25s %-6s\n", "Roll", "Name", "Marks");
    printf("----------------------------------------\n");
    for (int i = 0; i < count; i++)
        printf("%-8d %-25s %-6.2f\n", students[i].roll, students[i].name, students[i].marks);
}

void searchStudent(void) {
    int roll;
    printf("Enter roll number to search: ");
    scanf("%d", &roll);
    int i = findByRoll(roll);
    if (i == -1)
        printf("Student not found.\n");
    else
        printf("Found: %d | %s | %.2f\n", students[i].roll, students[i].name, students[i].marks);
}

void deleteStudent(void) {
    int roll;
    printf("Enter roll number to delete: ");
    scanf("%d", &roll);
    int i = findByRoll(roll);
    if (i == -1) {
        printf("Student not found.\n");
        return;
    }
    for (int j = i; j < count - 1; j++)
        students[j] = students[j + 1];
    count--;
    saveToFile();
    printf("Record deleted.\n");
}

int main(void) {
    int choice;
    loadFromFile();
    do {
        printf("\n=== Student Record Manager ===\n");
        printf("1. Add student\n2. View all\n3. Search by roll\n4. Delete\n5. Exit\n");
        printf("Choose: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = 0;
        }
        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewAll(); break;
            case 3: searchStudent(); break;
            case 4: deleteStudent(); break;
            case 5: printf("Goodbye!\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
    return 0;
}
