#ifndef LIBRARY_H
#define LIBRARY_H

#define MAX_NAME_LENGTH 50

// Enum untuk tingkat prioritas
typedef enum {
    UMUM = 1,      // Prioritas terendah
    MAHASISWA = 2, // Prioritas menengah
    DOSEN = 3      // Prioritas tertinggi
} PriorityLevel;

// Struktur untuk node anggota (digunakan dalam antrian peminjaman)
typedef struct MemberNode {
    char name[MAX_NAME_LENGTH];
    PriorityLevel priority;
    struct MemberNode* next;
} MemberNode;

// Struktur untuk node buku
typedef struct BookNode {
    char title[MAX_NAME_LENGTH];
    int stock;              // Stok buku
    MemberNode* queue;      // Antrian peminjaman (queue)
    char currentBorrower[MAX_NAME_LENGTH]; // Peminjam saat ini
    struct BookNode* next;
} BookNode;

// Struktur untuk node stack (untuk riwayat aktivitas)
typedef struct ActivityNode {
    char activity[MAX_NAME_LENGTH]; // Deskripsi aktivitas (misal: "Pinjam Buku1 oleh Dosen1")
    struct ActivityNode* next;
} ActivityNode;

// Function prototypes untuk buku
BookNode* createBookNode(const char* title, int stock);
void insertBook(BookNode** head, const char* title, int stock);
BookNode* findBook(BookNode* head, const char* title);
void displayBooks(BookNode* head);

// Function prototypes untuk antrian peminjaman (queue)
MemberNode* createMemberNode(const char* name, PriorityLevel priority);
void enqueueMember(MemberNode** head, const char* name, PriorityLevel priority);
MemberNode* dequeueHighestPriority(MemberNode** head);
void displayQueue(MemberNode* head);
int isQueueEmpty(MemberNode* head);

// Function prototypes untuk stack (riwayat aktivitas)
void pushActivity(ActivityNode** stack, const char* activity);
void popActivity(ActivityNode** stack, char* activity);
void displayActivities(ActivityNode* stack);

// Function prototypes untuk sistem perpustakaan
void borrowBook(BookNode* book, const char* memberName, ActivityNode** activityStack);
void returnBook(BookNode* book, ActivityNode** activityStack);
void cancelLastActivity(BookNode* bookList, ActivityNode** activityStack);

#endif // LIBRARY_H