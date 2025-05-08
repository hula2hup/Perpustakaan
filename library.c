#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

// Fungsi untuk buku
BookNode* createBookNode(const char* title, int stock) {
    BookNode* newNode = (BookNode*)malloc(sizeof(BookNode));
    strcpy(newNode->title, title);
    newNode->stock = stock;
    newNode->queue = NULL;
    strcpy(newNode->currentBorrower, "");
    newNode->next = NULL;
    return newNode;
}

void insertBook(BookNode** head, const char* title, int stock) {
    BookNode* newNode = createBookNode(title, stock);
    newNode->next = *head;
    *head = newNode;
}

BookNode* findBook(BookNode* head, const char* title) {
    while (head != NULL) {
        if (strcmp(head->title, title) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

void displayBooks(BookNode* head) {
    if (head == NULL) {
        printf("Tidak ada buku.\n");
        return;
    }
    while (head != NULL) {
        printf("Judul: %s, Stok: %d, Peminjam: %s\n", head->title, head->stock, 
               strlen(head->currentBorrower) > 0 ? head->currentBorrower : "Tidak ada");
        printf("Antrian peminjaman:\n");
        displayQueue(head->queue);
        head = head->next;
    }
}

// Fungsi untuk antrian peminjaman (queue)
MemberNode* createMemberNode(const char* name, PriorityLevel priority) {
    MemberNode* newNode = (MemberNode*)malloc(sizeof(MemberNode));
    strcpy(newNode->name, name);
    newNode->priority = priority;
    newNode->next = NULL;
    return newNode;
}

void enqueueMember(MemberNode** head, const char* name, PriorityLevel priority) {
    MemberNode* newNode = createMemberNode(name, priority);
    if (*head == NULL || (*head)->priority < priority) {
        newNode->next = *head;
        *head = newNode;
    } else {
        MemberNode* current = *head;
        MemberNode* prev = NULL;
        while (current != NULL && current->priority >= priority) {
            prev = current;
            current = current->next;
        }
        newNode->next = current;
        if (prev != NULL) {
            prev->next = newNode;
        } else {
            *head = newNode;
        }
    }
}

MemberNode* dequeueHighestPriority(MemberNode** head) {
    if (*head == NULL) return NULL;
    MemberNode* highest = *head;
    MemberNode* prev = NULL;
    MemberNode* current = *head;
    MemberNode* prevHighest = NULL;

    // Cari node dengan prioritas tertinggi
    while (current->next != NULL) {
        if (current->next->priority > highest->priority) {
            highest = current->next;
            prevHighest = current;
        }
        current = current->next;
    }

    // Hapus node dengan prioritas tertinggi
    if (prevHighest != NULL) {
        prevHighest->next = highest->next;
    } else {
        *head = highest->next;
    }
    return highest;
}

void displayQueue(MemberNode* head) {
    if (head == NULL) {
        printf("  Tidak ada antrian.\n");
        return;
    }
    while (head != NULL) {
        printf("  - %s (Prioritas: %d)\n", head->name, head->priority);
        head = head->next;
    }
}

int isQueueEmpty(MemberNode* head) {
    return head == NULL;
}

// Fungsi untuk stack (riwayat aktivitas)
void pushActivity(ActivityNode** stack, const char* activity) {
    ActivityNode* newNode = (ActivityNode*)malloc(sizeof(ActivityNode));
    strcpy(newNode->activity, activity);
    newNode->next = *stack;
    *stack = newNode;
}

void popActivity(ActivityNode** stack, char* activity) {
    if (*stack == NULL) {
        strcpy(activity, "");
        return;
    }
    ActivityNode* temp = *stack;
    strcpy(activity, temp->activity);
    *stack = temp->next;
    free(temp);
}

void displayActivities(ActivityNode* stack) {
    if (stack == NULL) {
        printf("Tidak ada aktivitas.\n");
        return;
    }
    while (stack != NULL) {
        printf("- %s\n", stack->activity);
        stack = stack->next;
    }
}

// Fungsi untuk sistem perpustakaan
void borrowBook(BookNode* book, const char* memberName, ActivityNode** activityStack) {
    if (book->stock > 0 && strlen(book->currentBorrower) == 0) {
        book->stock--;
        strcpy(book->currentBorrower, memberName);
        char activity[MAX_NAME_LENGTH];
        snprintf(activity, MAX_NAME_LENGTH, "Pinjam %s oleh %s", book->title, memberName);
        pushActivity(activityStack, activity);
        printf("%s berhasil meminjam %s.\n", memberName, book->title);
    } else {
        printf("Buku sedang dipinjam atau stok habis.\n");
    }
}

void returnBook(BookNode* book, ActivityNode** activityStack) {
    if (strlen(book->currentBorrower) == 0) {
        printf("Buku tidak sedang dipinjam.\n");
        return;
    }
    char activity[MAX_NAME_LENGTH];
    snprintf(activity, MAX_NAME_LENGTH, "Kembalikan %s oleh %s", book->title, book->currentBorrower);
    pushActivity(activityStack, activity);
    printf("%s mengembalikan %s.\n", book->currentBorrower, book->title);
    book->stock++;
    strcpy(book->currentBorrower, "");

    // Proses antrian berikutnya
    if (!isQueueEmpty(book->queue)) {
        MemberNode* nextMember = dequeueHighestPriority(&(book->queue));
        if (nextMember != NULL) {
            borrowBook(book, nextMember->name, activityStack);
            free(nextMember);
        }
    }
}

void cancelLastActivity(BookNode* bookList, ActivityNode** activityStack) {
    char lastActivity[MAX_NAME_LENGTH];
    popActivity(activityStack, lastActivity);
    if (strlen(lastActivity) == 0) {
        printf("Tidak ada aktivitas untuk dibatalkan.\n");
        return;
    }
    printf("Membatalkan aktivitas: %s\n", lastActivity);

    // Parsing aktivitas untuk menentukan tindakan
    char action[MAX_NAME_LENGTH], bookTitle[MAX_NAME_LENGTH], memberName[MAX_NAME_LENGTH];
    sscanf(lastActivity, "%s %s oleh %s", action, bookTitle, memberName);

    BookNode* book = findBook(bookList, bookTitle);
    if (book == NULL) return;

    if (strcmp(action, "Pinjam") == 0) {
        if (strcmp(book->currentBorrower, memberName) == 0) {
            book->stock++;
            strcpy(book->currentBorrower, "");
            printf("Peminjaman %s oleh %s dibatalkan.\n", bookTitle, memberName);
        }
    } else if (strcmp(action, "Kembalikan") == 0) {
        book->stock--;
        strcpy(book->currentBorrower, memberName);
        printf("Pengembalian %s oleh %s dibatalkan.\n", bookTitle, memberName);
    }
}