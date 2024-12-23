#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum untuk status perbaikan
enum Status { Menunggu, Dikerjakan, Selesai };

// Struct untuk data layanan kendaraan
struct Kendaraan {
    int id;                              // ID kendaraan
    char platNomor[15];                  // Nomor plat kendaraan
    char jenisLayanan[50];               // Jenis layanan yang dipilih
    union EstimasiWaktu estimasi;        // Estimasi waktu pengerjaan
    int jenisEstimasi;                   // 0 = menit, 1 = jam
    enum Status status;                  // Status pengerjaan
    struct Kendaraan *next;              // Pointer untuk linked list
};
 
// Union untuk estimasi waktu pengerjaan
union EstimasiWaktu {
    int menit;   // Estimasi dalam menit
    float jam;   // Estimasi dalam jam
};

// Fungsi untuk menampilkan menu
void tampilkanMenu() {
    printf("\n--- Sistem Layanan Bengkel ---\n");
    printf("1. Tambah Kendaraan\n");
    printf("2. Lihat Daftar Kendaraan\n");
    printf("3. Ubah Status Pengerjaan\n");
    printf("4. Hapus Data Kendaraan\n");
    printf("5. Keluar\n");
    printf("Pilihan: ");
}


// Fungsi untuk menambah kendaraan dan layanan
struct Kendaraan* tambahKendaraan(struct Kendaraan *head) {
    struct Kendaraan *baru = (struct Kendaraan*) malloc(sizeof(struct Kendaraan));
    if (baru == NULL) {
        printf("Gagal menambahkan kendaraan! Memori penuh.\n");
        return head;
    }

    // Input data kendaraan
    printf("Masukkan ID Kendaraan: ");
    scanf("%d", &baru->id);
    printf("Masukkan Plat Nomor Kendaraan: ");
    scanf("%s", baru->platNomor);
    printf("Masukkan Jenis Layanan (contoh: Ganti Oli, Servis Rem): ");
    scanf(" %[^\n]", baru->jenisLayanan);

    // Input estimasi waktu pengerjaan
    printf("Pilih jenis estimasi waktu (0: Menit, 1: Jam): ");
    scanf("%d", &baru->jenisEstimasi);
    if (baru->jenisEstimasi == 0) {
        printf("Masukkan estimasi waktu (dalam menit): ");
        scanf("%d", &baru->estimasi.menit);
    } else if (baru->jenisEstimasi == 1) {
        printf("Masukkan estimasi waktu (dalam jam): ");
        scanf("%f", &baru->estimasi.jam);
    } else {
        printf("Jenis estimasi tidak valid! Default ke 0 menit.\n");
        baru->estimasi.menit = 0;
    }

    // Set status awal
    baru->status = Menunggu;

    // Tambah ke linked list
    baru->next = head;
    printf("Data kendaraan berhasil ditambahkan!\n");
    return baru;
}


// Fungsi untuk menampilkan daftar kendaraan
void tampilkanKendaraan(struct Kendaraan *head) {
    if (head == NULL) {
        printf("Belum ada kendaraan yang didaftarkan.\n");
        return;
    }

    struct Kendaraan *temp = head;
    printf("\n--- Daftar Kendaraan di Bengkel ---\n");
    while (temp != NULL) {
        printf("ID Kendaraan : %d\n", temp->id);
        printf("Plat Nomor   : %s\n", temp->platNomor);
        printf("Layanan      : %s\n", temp->jenisLayanan);
        if (temp->jenisEstimasi == 0) {
            printf("Estimasi     : %d menit\n", temp->estimasi.menit);
        } else {
            printf("Estimasi     : %.2f jam\n", temp->estimasi.jam);
        }
        printf("Status       : %s\n",
               temp->status == Menunggu ? "Menunggu" :
               temp->status == Dikerjakan ? "Sedang Dikerjakan" : "Selesai");
        printf("-----------------------------\n");
        temp = temp->next;
    }
}


// Fungsi untuk mengubah status pengerjaan
void ubahStatus(struct Kendaraan *head) {
    if (head == NULL) {
        printf("Belum ada kendaraan yang terdaftar.\n");
        return;
    }

    int id;
    printf("Masukkan ID Kendaraan yang ingin diubah statusnya: ");
    scanf("%d", &id);

    struct Kendaraan *temp = head;
    while (temp != NULL && temp->id != id) {
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Kendaraan dengan ID %d tidak ditemukan.\n", id);
        return;
    }

    printf("Pilih status baru (0: Menunggu, 1: Sedang Dikerjakan, 2: Selesai): ");
    int statusBaru;
    scanf("%d", &statusBaru);

    if (statusBaru >= 0 && statusBaru <= 2) {
        temp->status = (enum Status) statusBaru;
        printf("Status berhasil diperbarui!\n");
    } else {
        printf("Status tidak valid.\n");
    }
}


// Fungsi untuk menghapus data kendaraan
struct Kendaraan* hapusKendaraan(struct Kendaraan *head, int id) {
    struct Kendaraan *temp = head, *prev = NULL;

    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Kendaraan dengan ID %d tidak ditemukan.\n", id);
        return head;
    }

    if (prev == NULL) {
        head = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
    printf("Data kendaraan berhasil dihapus!\n");
    return head;
}


// Fungsi utama
int main() {
    struct Kendaraan *head = NULL;
    int pilihan, id;

    do {
        tampilkanMenu();
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                head = tambahKendaraan(head);
                break;
            case 2:
                tampilkanKendaraan(head);
                break;
            case 3:
                ubahStatus(head);
                break;
            case 4:
                printf("Masukkan ID Kendaraan yang ingin dihapus: ");
                scanf("%d", &id);
                head = hapusKendaraan(head, id);
                break;
            case 5:
                printf("Keluar dari program. Terima kasih!\n");
                break;
            default:
                printf("Pilihan tidak valid! Coba lagi.\n");
        }
    } while (pilihan != 5);

    // Bersihkan memori
    while (head != NULL) {
        struct Kendaraan *temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}
