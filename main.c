#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

int main() {
    BookNode* bookList = NULL;
    ActivityNode* activityStack = NULL;

    printf("=== Sistem Perpustakaan ===\n");

    // a. Insert 2 buku: "Buku1" dan "Buku2", masing-masing stok 1
    printf("\nLangkah a: Insert Buku1 dan Buku2\n");
    insertBook(&bookList, "Buku1", 1);
    insertBook(&bookList, "Buku2", 1);
    displayBooks(bookList);

    // b. Insert Anggota1 (Mahasiswa) ke antrian peminjaman Buku1
    printf("\nLangkah b: Anggota1 (Mahasiswa) ingin meminjam Buku1\n");
    BookNode* book1 = findBook(bookList, "Buku1");
    enqueueMember(&(book1->queue), "Anggota1", MAHASISWA);
    displayBooks(bookList);

    // c. Insert Dosen1 (Dosen) ke antrian peminjaman Buku1
    printf("\nLangkah c: Dosen1 (Dosen) ingin meminjam Buku1\n");
    enqueueMember(&(book1->queue), "Dosen1", DOSEN);
    displayBooks(bookList);

    // d. Insert Umum1 (Masyarakat Umum) ke antrian peminjaman Buku1
    printf("\nLangkah d: Umum1 (Masyarakat Umum) ingin meminjam Buku1\n");
    enqueueMember(&(book1->queue), "Umum1", UMUM);
    displayBooks(bookList);

    // e. Insert Umum1 (Masyarakat Umum) ke antrian peminjaman Buku2
    printf("\nLangkah e: Umum1 (Masyarakat Umum) ingin meminjam Buku2\n");
    BookNode* book2 = findBook(bookList, "Buku2");
    enqueueMember(&(book2->queue), "Umum1", UMUM);
    displayBooks(bookList);

    // f. Proses peminjaman Buku1 (harusnya Dosen1 yang mendapatkannya)
    printf("\nLangkah f: Proses peminjaman Buku1\n");
    MemberNode* nextMember = dequeueHighestPriority(&(book1->queue));
    if (nextMember != NULL) {
        borrowBook(book1, nextMember->name, &activityStack);
        free(nextMember);
    }
    displayBooks(bookList);

    // g. Proses pengembalian Buku1 oleh Dosen1, selanjutnya Anggota1 yang dapat
    printf("\nLangkah g: Dosen1 mengembalikan Buku1\n");
    returnBook(book1, &activityStack);
    displayBooks(bookList);

    // h. Umum1 membatalkan aktivitas terakhir (peminjaman Buku2)
    printf("\nLangkah h: Umum1 membatalkan aktivitas terakhir (antrian Buku2)\n");
    cancelLastActivity(bookList, &activityStack);
    displayBooks(bookList);

    // Tampilkan riwayat aktivitas
    printf("\nRiwayat Aktivitas:\n");
    displayActivities(activityStack);

    return 0;
}