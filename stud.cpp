#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct student {
    char name[100];
    int idn;
    int degree[6];
};

void add();
void show();
void searchByName();
void searchByID();
void menu();
void clearBuffer();
void syncTxtToBin();

int main() {
    syncTxtToBin();
    menu();
    return 0;
}

void menu() {
    int choose;
    do {
        printf("\n==============================");
        printf("\n   Student Management (TXT & BIN)   ");
        printf("\n==============================");
        printf("\n1. Add Student Info");
        printf("\n2. Show All Info (from BIN)");
        printf("\n3. Search by Name (in BIN)");
        printf("\n4. Search by ID (in BIN)");
        printf("\n5. Exit");
        printf("\nChoice: ");

        if (scanf("%d", &choose) != 1) {
            printf("Invalid input!\n");
            clearBuffer();
            continue;
        }

        switch (choose) {
            case 1: add(); break;
            case 2: show(); break;
            case 3: searchByName(); break;
            case 4: searchByID(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Wrong choice.\n");
        }
    } while (choose != 5);
}

void add() {
    struct student s;
    FILE *txtFile = fopen("info.txt", "a");
    FILE *binFile = fopen("info.bin", "ab");

    if (txtFile == NULL || binFile == NULL) {
        printf("Error: Could not open files!\n");
        return;
    }

    clearBuffer();
    printf("\nEnter full name: ");
    fgets(s.name, 100, stdin);
    s.name[strcspn(s.name, "\n")] = 0;

    printf("Enter ID: ");
    scanf("%d", &s.idn);


    fprintf(txtFile, "%s,%d", s.name, s.idn);

    printf("Enter 6 degrees:\n");
    for (int i = 0; i < 6; i++) {
        printf("Subject %d: ", i + 1);
        scanf("%d", &s.degree[i]);
        fprintf(txtFile, ",%d", s.degree[i]);
    }
    fprintf(txtFile, ","); 
    
    fwrite(&s, sizeof(struct student), 1, binFile);

    fclose(txtFile);
    fclose(binFile);
    printf("\nSaved to info.txt and info.bin successfully!\n");
}

void show() {
    
    FILE *binFile = fopen("info.bin", "rb");
    if (binFile == NULL) {
        printf("\nNo binary data found. Add students first.\n");
        return;
    }

    struct student s;
    printf("\n%-20s | %-10s | %-20s\n", "Name", "ID", "Degrees");
    printf("------------------------------------------------------------\n");

    while (fread(&s, sizeof(struct student), 1, binFile)) {
        printf("%-20s | %-10d | ", s.name, s.idn);
        for (int i = 0; i < 6; i++) printf("%d ", s.degree[i]);
        printf("\n");
    }
    fclose(binFile);
}

void searchByName() {
    char target[100];
    struct student s;
    int found = 0;

    clearBuffer();
    printf("\nEnter student name to search (in .bin file): ");
    fgets(target, 100, stdin);
    target[strcspn(target, "\n")] = 0;

    FILE *binFile = fopen("info.bin", "rb");
    if (binFile == NULL) {
        printf("\nError: Could not open .bin file.\n");
        return;
    }

    while (fread(&s, sizeof(struct student), 1, binFile)) {
        if (strcmp(target, s.name) == 0) {
            printf("\n--- Student Found (in BIN) ---\n");
            printf("Name: %s\nID: %d\nDegrees: ", s.name, s.idn);
            for (int i = 0; i < 6; i++) printf("%d ", s.degree[i]);
            printf("\n----------------------\n");
            found = 1;
        }
    }

    if (!found) printf("\nStudent '%s' not found in binary file.\n", target);
    fclose(binFile);
}

void searchByID() {
    int targetID;
    struct student s;
    int found = 0;

    printf("\nEnter student ID to search (in .bin file): ");
    scanf("%d", &targetID);

    FILE *binFile = fopen("info.bin", "rb");
    if (binFile == NULL) {
        printf("\nError: Could not open .bin file.\n");
        return;
    }

    while (fread(&s, sizeof(struct student), 1, binFile)) {
        if (s.idn == targetID) {
            printf("\n--- Student Found (in BIN) ---\n");
            printf("Name: %s\nID: %d\nDegrees: ", s.name, s.idn);
            for (int i = 0; i < 6; i++) printf("%d ", s.degree[i]);
            printf("\n----------------------\n");
            found = 1;
            break;
        }
    }

    if (!found) printf("\nID %d not found in binary file.\n", targetID);
    fclose(binFile);
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void syncTxtToBin(); 
void syncTxtToBin() {
    FILE *txtFile = fopen("info.txt", "r");
    if (txtFile == NULL) {
        
        return;
    }

    FILE *binFile = fopen("info.bin", "wb");
    if (binFile == NULL) {
        printf("Error creating binary file during sync!\n");
        fclose(txtFile);
        return;
    }

    struct student s;
    int count = 0;

    while (fscanf(txtFile, " %99[^,],%d,%d,%d,%d,%d,%d,%d,",
           s.name, &s.idn, &s.degree[0], &s.degree[1],
           &s.degree[2], &s.degree[3], &s.degree[4], &s.degree[5]) == 8) {

        fwrite(&s, sizeof(struct student), 1, binFile);
        count++;
    }

    fclose(txtFile);
    fclose(binFile);

    if (count > 0) {
        printf("\n[Sync] Successfully updated info.bin from info.txt (%d records).\n", count);
    }
}
