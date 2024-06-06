#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_PHONE_LENGTH 15
#define FILE_NAME "student_records.txt"

// Define structure for student record
struct Student {
    int id;
    char name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
};

// Function prototypes
void addRecord();
void displayRecords();
void searchRecord();
void modifyRecord();
void deleteRecord();

int main() {
    int choice;
    do {
        printf("\nStudent Record System\n");
        printf("1. Add Record\n");
        printf("2. Display Records\n");
        printf("3. Search Record\n");
        printf("4. Modify Record\n");
        printf("5. Delete Record\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addRecord();
                break;
            case 2:
                displayRecords();
                break;
            case 3:
                searchRecord();
                break;
            case 4:
                modifyRecord();
                break;
            case 5:
                deleteRecord();
                break;
            case 6:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}

void addRecord() {
    struct Student student;
    FILE *file = fopen(FILE_NAME, "ab+");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter student ID: ");
    scanf("%d", &student.id);
    printf("Enter student name: ");
    scanf("%s", student.name);
    printf("Enter student phone: ");
    scanf("%s", student.phone);

    fwrite(&student, sizeof(struct Student), 1, file);
    printf("Record added successfully.\n");

    fclose(file);
}

void displayRecords() {
    struct Student student;
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nStudent Records\n");
    printf("ID\tName\tPhone\n");
    printf("----------------------\n");

    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        printf("%d\t%s\t%s\n", student.id, student.name, student.phone);
    }

    fclose(file);
}

void searchRecord() {
    int searchId;
    struct Student student;
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter student ID to search: ");
    scanf("%d", &searchId);

    int found = 0;
    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        if (student.id == searchId) {
            printf("Record found:\n");
            printf("ID: %d\nName: %s\nPhone: %s\n", student.id, student.name, student.phone);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Record not found.\n");
    }

    fclose(file);
}

void modifyRecord() {
    int modifyId;
    struct Student student;
    FILE *file = fopen(FILE_NAME, "rb+");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter student ID to modify: ");
    scanf("%d", &modifyId);

    int found = 0;
    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        if (student.id == modifyId) {
            printf("Enter new name: ");
            scanf("%s", student.name);
            printf("Enter new phone: ");
            scanf("%s", student.phone);

            fseek(file, -sizeof(struct Student), SEEK_CUR);
            fwrite(&student, sizeof(struct Student), 1, file);
            printf("Record modified successfully.\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Record not found.\n");
    }

    fclose(file);
}

void deleteRecord() {
    int deleteId;
    struct Student student;
    FILE *tempFile = fopen("temp.txt", "wb");
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL || tempFile == NULL) {
        printf("Error opening file(s).\n");
        return;
    }

    printf("Enter student ID to delete: ");
    scanf("%d", &deleteId);

    int found = 0;
    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        if (student.id != deleteId) {
            fwrite(&student, sizeof(struct Student), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);

    if (found) {
        printf("Record deleted successfully.\n");
    } else {
        printf("Record not found.\n");
    }
}
