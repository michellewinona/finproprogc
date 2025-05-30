#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_PASIEN 100
#define MAX_RIWAYAT 30

enum StatusBMI { UNDERWEIGHT, NORMAL, OVERWEIGHT, OBESE };

typedef struct {
    const char *nama;
    int kalori;
} DataMakanan;

struct RiwayatHarian {
    char tanggal[15];
    float kaloriKonsumsi;
    float kaloriTerbakar;
    float totalKaloriNet;
};

struct Pasien {
    char id[20];
    char nama[50];
    char gender;
    int usia;
    float berat;
    float tinggi;
    float bmi;
    enum StatusBMI status;
    float totalKaloriHarianSaatIni;
    
    struct RiwayatHarian riwayat[MAX_RIWAYAT];
    int jumlahRiwayat;
};

DataMakanan daftar_makanan_database[] = {
    {"abon ikan", 435}, {"alpukat", 85}, {"apel", 58}, {"asinan bogor", 98},
    {"dada ayam kentucky", 338}, {"paha ayam goreng pasundan", 245}, {"ayam taliwang", 264},
    {"babi rica", 491}, {"bakpia", 272}, {"bakwan", 280}, {"bayam rebus", 23},
    {"bebek goreng", 300}, {"beef burger", 258}, {"beef teriyaki", 151},
    {"belut goreng", 417}, {"bihun goreng", 381}, {"bika ambon", 199}, {"nasi", 180},
    {"nasi merah", 149}, {"ketan hitam", 181}, {"misoa", 345}, {"roti putih", 248},
    {"biskuit", 458}, {"ketoprak", 153}, {"kue sus", 221}, {"mie ayam", 102},
    {"spaghetti", 139}, {"tahu goreng", 115}, {"kacang tanah rebus", 220}
};

const int JUMLAH_MAKANAN_DATABASE = sizeof(daftar_makanan_database) / sizeof(daftar_makanan_database[0]);

struct Pasien* pasienList[MAX_PASIEN];
int jumlahPasien = 0;

const char* status_bmi_to_string(enum StatusBMI s) {
    switch (s) {
        case UNDERWEIGHT: return "Underweight";
        case NORMAL: return "Normal";
        case OVERWEIGHT: return "Overweight";
        case OBESE: return "Obese";
        default: return "Unknown";
    }
}

void hitung_bmi(struct Pasien* p) {
    p->bmi = p->berat / pow(p->tinggi / 100, 2);
    if (p->bmi < 18.5) {
        p->status = UNDERWEIGHT;
    } else if (p->bmi < 25) {
        p->status = NORMAL;
    } else if (p->bmi < 30) {
        p->status = OVERWEIGHT;
    } else {
        p->status = OBESE;
    }
}

float hitung_bmr(struct Pasien* p) {
    if (p->gender == 'P' || p->gender == 'p') {
        return 10 * p->berat + 6.25 * p->tinggi - 5 * p->usia + 5;
    } else if (p->gender == 'W' || p->gender == 'w') {
        return 10 * p->berat + 6.25 * p->tinggi - 5 * p->usia - 161;
    } else {
        printf("Gender tidak valid. Diasumsikan pria untuk perhitungan BMR.\n");
        return 10 * p->berat + 6.25 * p->tinggi - 5 * p->usia + 5;
    }
}

float hitung_tdee(float bmr) {
    int pilihan;
    float faktor = 1.2;

    printf("\n--- Tingkat Aktivitas --- \n");
    printf("1. Tidak aktif (sedentary - sedikit atau tanpa olahraga)\n");
    printf("2. Aktivitas ringan (olahraga ringan 1-3 hari/minggu)\n");
    printf("3. Aktivitas sedang (olahraga sedang 3-5 hari/minggu)\n");
    printf("4. Aktivitas berat (olahraga berat 6-7 hari/minggu)\n");
    printf("5. Atlet (olahraga sangat berat setiap hari)\n");
    printf("Pilih (1-5): ");
    scanf("%d", &pilihan);

    switch (pilihan) {
        case 1: faktor = 1.2; break;
        case 2: faktor = 1.375; break;
        case 3: faktor = 1.55; break;
        case 4: faktor = 1.725; break;
        case 5: faktor = 1.9; break;
        default: printf("Pilihan tidak valid, diasumsikan sedentary.\n");
    }

    return bmr * faktor;
}

float hitung_kalori_aktivitas() {
    int pilihanAktivitas;
    float durasiMenit;
    float kaloriTerbakar = 0;
    char lanjutAktivitas;

    printf("\n--- Input Aktivitas Fisik --- \n");
    do {
        printf("Pilih jenis aktivitas yang sudah dilakukan:\n");
        printf("1. Duduk/Istirahat (sangat ringan)\n");
        printf("2. Jalan Kaki Santai (ringan)\n");
        printf("3. Olahraga Sedang (misal: jogging, bersepeda)\n");
        printf("4. Olahraga Berat (misal: lari cepat, angkat beban berat)\n");
        printf("Pilih (1-4): ");
        scanf("%d", &pilihanAktivitas);

        printf("Durasi aktivitas (menit): ");
        scanf("%f", &durasiMenit);

        switch (pilihanAktivitas) {
            case 1: kaloriTerbakar += (0.0175 * 60 * durasiMenit); break;
            case 2: kaloriTerbakar += (0.03 * 60 * durasiMenit); break;
            case 3: kaloriTerbakar += (0.08 * 60 * durasiMenit); break;
            case 4: kaloriTerbakar += (0.12 * 60 * durasiMenit); break;
            default: printf("Pilihan aktivitas tidak valid.\n");
        }
        printf("Total kalori terbakar saat ini dari aktivitas: %.1f kalori\n", kaloriTerbakar);
        printf("Tambahkan aktivitas lain? (y/n): ");
        scanf(" %c", &lanjutAktivitas);
    } while (lanjutAktivitas == 'y' || lanjutAktivitas == 'Y');

    return kaloriTerbakar;
}

float hitung_kalori_konsumsi() {
    char namaMakananInput[50];
    float kaloriMakanan;
    float totalKalori = 0;
    char lanjut;
    int pilihan_input;

    printf("\n--- Input Makanan yang Dikonsumsi --- \n");
    do {
        printf("Bagaimana Anda ingin memasukkan makanan?\n");
        printf("1. Cari dari Database Makanan\n");
        printf("2. Input Nama & Kalori Manual\n");
        printf("Pilih (1/2): ");
        scanf("%d", &pilihan_input);

        while (getchar() != '\n');

        switch (pilihan_input) {
            case 1: {
                if (JUMLAH_MAKANAN_DATABASE == 0) {
                    printf("Database makanan kosong. Mohon input manual.\n");
                    goto manual_input_label; 
                }

                printf("Masukkan nama makanan untuk dicari (misal: apel, nasi): ");
                fgets(namaMakananInput, sizeof(namaMakananInput), stdin);
                namaMakananInput[strcspn(namaMakananInput, "\n")] = 0;

                int ditemukan = 0;
                int i;
                for (i = 0; i < JUMLAH_MAKANAN_DATABASE; i++) {
                    if (strcmp(daftar_makanan_database[i].nama, namaMakananInput) == 0) {
                        kaloriMakanan = (float)daftar_makanan_database[i].kalori;
                        ditemukan = 1;
                        printf("Ditemukan: '%s' dengan %.0f kalori (per 100 gram).\n", daftar_makanan_database[i].nama, kaloriMakanan);
                        break;
                    }
                }
                if (!ditemukan) {
                    printf("Makanan '%s' tidak ditemukan di database. Silakan input manual kalorinya.\n", namaMakananInput);
                    manual_input_label:
                    printf("Jumlah Kalori '%s': ", namaMakananInput);
                    scanf("%f", &kaloriMakanan);
                    while (getchar() != '\n');
                }
                break;
            }
            case 2: {
                printf("Nama Makanan (misal: Nasi Goreng, Sandwich): ");
                fgets(namaMakananInput, sizeof(namaMakananInput), stdin);
                namaMakananInput[strcspn(namaMakananInput, "\n")] = 0;

                printf("Jumlah Kalori '%s': ", namaMakananInput);
                scanf("%f", &kaloriMakanan);
                while (getchar() != '\n');
                break;
            }
            default: {
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                kaloriMakanan = -1;
                break;
            }
        }

        if (kaloriMakanan >= 0) {
            totalKalori += kaloriMakanan;
            printf("Anda menambahkan '%s' (%.0f kalori). Total kalori konsumsi saat ini: %.1f kalori\n", namaMakananInput, kaloriMakanan, totalKalori);
        } else {
            printf("Kalori tidak valid atau pilihan salah. Tidak ditambahkan.\n");
        }

        printf("Tambahkan makanan lain? (y/n): ");
        scanf(" %c", &lanjut);
        while (getchar() != '\n');
    } while (lanjut == 'y' || lanjut == 'Y');

    return totalKalori;
}

void input_data_pasien() {
    if (jumlahPasien >= MAX_PASIEN) {
        printf("Data pasien penuh! Tidak bisa menambah pasien baru.\n");
        return;
    }

    struct Pasien* p = (struct Pasien*) malloc(sizeof(struct Pasien));
    if (p == NULL) {
        printf("Gagal alokasi memori.\n");
        return;
    }

    printf("\n--- TAMBAH DATA PASIEN ---\n");
    printf("ID Pasien (unik): ");
    scanf("%s", p->id);

	int i;
    for (i = 0; i < jumlahPasien; i++) {
        if (strcmp(p->id, pasienList[i]->id) == 0) {
            printf("ID sudah digunakan. Mohon gunakan ID lain.\n");
            free(p);
            return;
        }
    }

    printf("Nama Pasien: ");
    while (getchar() != '\n'); 
    fgets(p->nama, sizeof(p->nama), stdin);
    p->nama[strcspn(p->nama, "\n")] = 0;

    printf("Jenis Kelamin (P = Pria, W = Wanita): ");
    scanf(" %c", &p->gender);

    printf("Usia: ");
    scanf("%d", &p->usia);
    printf("Berat (kg): ");
    scanf("%f", &p->berat);
    printf("Tinggi (cm): ");
    scanf("%f", &p->tinggi);

    hitung_bmi(p);

    p->totalKaloriHarianSaatIni = 0;
    p->jumlahRiwayat = 0;
    
    pasienList[jumlahPasien++] = p;
    printf("Pasien '%s' berhasil ditambahkan dengan BMI %.2f (%s).\n", p->nama, p->bmi, status_bmi_to_string(p->status));
}

struct Pasien* cari_pasien() {
    char cariID[20];
    printf("Masukkan ID pasien: ");
    scanf("%s", cariID);

	int i;
    for (i = 0; i < jumlahPasien; i++) {
        if (strcmp(pasienList[i]->id, cariID) == 0) {
            return pasienList[i];
        }
    }

    printf("Pasien dengan ID '%s' tidak ditemukan.\n", cariID);
    return NULL;
}

void tampilkan_detail_pasien() {
    struct Pasien* p = cari_pasien();
    if (p == NULL) return;

    float bmr = hitung_bmr(p);
    float tdee = hitung_tdee(bmr);

    printf("\n====== DETAIL PASIEN ======\n");
    printf("ID            : %s\n", p->id);
    printf("Nama          : %s\n", p->nama);
    printf("Jenis Kelamin : %s\n", (p->gender == 'P' || p->gender == 'p') ? "Pria" : "Wanita");
    printf("Usia          : %d tahun\n", p->usia);
    printf("Berat         : %.1f kg\n", p->berat);
    printf("Tinggi        : %.1f cm\n", p->tinggi);
    printf("BMI           : %.2f (%s)\n", p->bmi, status_bmi_to_string(p->status));
    printf("BMR           : %.2f kalori/hari (Energi yang dibutuhkan saat istirahat total)\n", bmr);
    printf("TDEE          : %.2f kalori/hari (Estimasi total kalori yang dibutuhkan)\n", tdee);
    printf("===========================\n");
}

void tampilkan_semua_pasien() {
    if (jumlahPasien == 0) {
        printf("Belum ada data pasien.\n");
        return;
    }
    printf("\n=== Data Semua Pasien ===\n");
    int i;
	for (i = 0; i < jumlahPasien; i++) {
        struct Pasien* p = pasienList[i];
        printf("ID: %s | Nama: %s | Berat: %.1f kg | Tinggi: %.1f cm | BMI: %.2f (%s)\n",
            p->id, p->nama, p->berat, p->tinggi, p->bmi, status_bmi_to_string(p->status));
    }
    printf("=========================\n");
}

void update_data_pasien() {
    struct Pasien* p = cari_pasien();
    if (p == NULL) return;

    printf("Perbarui data untuk %s:\n", p->nama);
    printf("Berat baru (kg, masukkan 0 jika tidak ingin mengubah): ");
    float beratBaru;
    scanf("%f", &beratBaru);
    if (beratBaru > 0) {
        p->berat = beratBaru;
    }

    printf("Tinggi baru (cm, masukkan 0 jika tidak ingin mengubah): ");
    float tinggiBaru;
    scanf("%f", &tinggiBaru);
    if (tinggiBaru > 0) {
        p->tinggi = tinggiBaru;
    }

    hitung_bmi(p);
    printf("Data pasien berhasil diperbarui. BMI terbaru: %.2f (%s).\n", p->bmi, status_bmi_to_string(p->status));
}

void hapus_data_pasien() {
    char cariID[20];
    printf("Masukkan ID pasien yang akan dihapus: ");
    scanf("%s", cariID);

	int i;
	int j;
    for (i = 0; i < jumlahPasien; i++) {
        if (strcmp(pasienList[i]->id, cariID) == 0) {
            printf("Apakah Anda yakin ingin menghapus pasien '%s'? (y/n): ", pasienList[i]->nama);
            char konfirmasi;
            scanf(" %c", &konfirmasi);
            if (konfirmasi == 'y' || konfirmasi == 'Y') {
                free(pasienList[i]);
                for (j = i; j < jumlahPasien - 1; j++) {
                    pasienList[j] = pasienList[j + 1];
                }
                jumlahPasien--;
                printf("Pasien berhasil dihapus.\n");
            } else {
                printf("Pembatalan penghapusan pasien.\n");
            }
            return;
        }
    }
    printf("Pasien tidak ditemukan.\n");
}

void get_current_date(char* date_str) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date_str, "%02d-%02d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void berikan_rekomendasi_lengkap(enum StatusBMI status_bmi, float kalori_net, float tdee) {
    printf("\n--- REKOMENDASI LENGKAP UNTUK ANDA ---\n");
    printf("Status BMI Anda: %s\n", status_bmi_to_string(status_bmi));

    printf("\n[ REKOMENDASI MAKANAN ]\n");
    int i;
	if (status_bmi == UNDERWEIGHT || kalori_net < tdee - 100) {
        printf("Untuk menambah berat badan / asupan kalori Anda, pertimbangkan makanan berikut:\n");
        int count = 0;
        for (i = 0; i < JUMLAH_MAKANAN_DATABASE && count < 3; i++) {
            if (daftar_makanan_database[i].kalori >= 150 && daftar_makanan_database[i].kalori <= 400) {
                printf("- %s (sekitar %d kalori per 100 gram)\n", daftar_makanan_database[i].nama, daftar_makanan_database[i].kalori);
                count++;
            }
        }
        if (count == 0) {
            printf("Tidak ada rekomendasi spesifik dari database. Coba konsumsi:\n");
            printf("- Sumber karbohidrat kompleks (nasi merah, roti gandum).\n");
            printf("- Protein (ayam, ikan, tempe, tahu) dalam porsi cukup.\n");
            printf("- Lemak sehat (alpukat, kacang-kacangan).\n");
        }
    } else if (status_bmi == OVERWEIGHT || status_bmi == OBESE || kalori_net > tdee + 100) {
        printf("Untuk menurunkan berat badan / mengurangi asupan kalori Anda, pertimbangkan makanan berikut:\n");
        int count = 0;
        int i;
        for (i = 0; i < JUMLAH_MAKANAN_DATABASE && count < 3; i++) {
            if (daftar_makanan_database[i].kalori <= 100) {
                printf("- %s (sekitar %d kalori per 100 gram)\n", daftar_makanan_database[i].nama, daftar_makanan_database[i].kalori);
                count++;
            }
        }
        if (count == 0) {
            printf("Tidak ada rekomendasi spesifik dari database. Coba konsumsi:\n");
            printf("- Perbanyak sayuran hijau dan buah-buahan berserat tinggi.\n");
            printf("- Pilih protein tanpa lemak (dada ayam, ikan rebus/panggang).\n");
            printf("- Kurangi makanan manis, gorengan, dan minuman berkalori tinggi.\n");
        }
    } else {
        printf("Asupan kalori dan BMI Anda sudah seimbang. Pertahankan pola makan sehat ini:\n");
        int count = 0;
        int i;
        for (i = 0; i < JUMLAH_MAKANAN_DATABASE && count < 3; i++) {
            if (daftar_makanan_database[i].kalori > 50 && daftar_makanan_database[i].kalori < 200) {
                printf("- %s (sekitar %d kalori per 100 gram)\n", daftar_makanan_database[i].nama, daftar_makanan_database[i].kalori);
                count++;
            }
        }
        if (count == 0) {
            printf("Pertahankan diet seimbang dengan variasi karbohidrat, protein, lemak sehat, serat, vitamin, dan mineral.\n");
        }
    }

    printf("\n[ REKOMENDASI OLAHRAGA ]\n");
    if (status_bmi == UNDERWEIGHT) {
        printf("Untuk membantu menambah massa otot (dan berat badan sehat), pertimbangkan:\n");
        printf("- Latihan kekuatan (angkat beban ringan).\n");
        printf("- Yoga atau pilates untuk fleksibilitas dan kekuatan inti.\n");
        printf("- Jalan kaki santai untuk menjaga kebugaran umum.\n");
    } else if (status_bmi == NORMAL) {
        printf("Untuk menjaga BMI normal dan kesehatan, pertimbangkan:\n");
        printf("- Olahraga kardio moderat (jogging, bersepeda, berenang) 3-5 kali seminggu.\n");
        printf("- Latihan kekuatan 2-3 kali seminggu.\n");
        printf("- Aktivitas yang Anda nikmati secara teratur.\n");
    } else if (status_bmi == OVERWEIGHT || status_bmi == OBESE) {
        printf("Untuk membantu menurunkan berat badan dan mencapai BMI normal, pertimbangkan:\n");
        printf("- Olahraga kardio intensitas sedang hingga tinggi (berjalan cepat, jogging, lompat tali) minimal 150-300 menit per minggu.\n");
        printf("- Latihan kekuatan untuk membangun otot yang membakar lebih banyak kalori.\n");
        printf("- Hindari terlalu banyak duduk; cobalah berdiri dan bergerak setiap jam.\n");
        printf("- Mulai dengan aktivitas ringan dan tingkatkan secara bertahap.\n");
    }
    printf("-----------------------------------------\n");
}

void berikan_rekomendasi_waktu(const char* waktu, float kalori_net, float tdee) {
    printf("\n===== SARAN UMUM UNTUK %s ====\n", waktu);
    
    if (kalori_net > tdee + 200) { 
        printf("Status Kalori (saat ini): SURPLUS KALORI.\n");
        printf("Saran untuk %s: Pertimbangkan mengurangi porsi atau memilih opsi yang lebih ringan.\n", waktu);
    } else if (kalori_net < tdee - 200) {
        printf("Status Kalori (saat ini): DEFISIT KALORI.\n");
        printf("Saran untuk %s: Pastikan asupan nutrisi Anda cukup, tambahkan sumber energi sehat.\n", waktu);
    } else {
        printf("Status Kalori (saat ini): SEIMBANG.\n");
        printf("Saran untuk %s: Pertahankan pilihan makanan dan aktivitas yang sudah baik.\n", waktu);
    }
    printf("=======================================\n");
}

void program_diet_rekomendasi() {
    struct Pasien* p = cari_pasien();
    if (p == NULL) return;

    printf("\n--- PROGRAM DIET & REKOMENDASI UNTUK %s ---\n", p->nama);

    float bmr = hitung_bmr(p);
    float tdee = hitung_tdee(bmr);
    printf("TDEE Anda hari ini diperkirakan: %.2f kalori.\n", tdee);

    printf("\nCatatan ini untuk waktu (Pagi/Siang/Sore/Malam)? ");
    while (getchar() != '\n'); 
    char waktuInput[20];
    fgets(waktuInput, sizeof(waktuInput), stdin);
    waktuInput[strcspn(waktuInput, "\n")] = 0;

    float kalori_konsumsi_sekarang = hitung_kalori_konsumsi();
    p->totalKaloriHarianSaatIni += kalori_konsumsi_sekarang; 

    float kalori_terbakar_sekarang = hitung_kalori_aktivitas();
    
    float kalori_net_harian = p->totalKaloriHarianSaatIni - kalori_terbakar_sekarang; 

    printf("\n=== ANALISIS HARI INI ===\n");
    printf("Kalori Konsumsi (akumulasi hari ini): %.1f kalori\n", p->totalKaloriHarianSaatIni);
    printf("Kalori Terbakar (aktivitas yang dicatat sekarang): %.1f kalori\n", kalori_terbakar_sekarang);
    printf("Estimasi Kalori Netto (akumulasi konsumsi - terbakar): %.1f kalori\n", kalori_net_harian);
    printf("TDEE Anda: %.2f kalori\n", tdee);

    berikan_rekomendasi_lengkap(p->status, kalori_net_harian, tdee);
    berikan_rekomendasi_waktu(waktuInput, kalori_net_harian, tdee);


    if (p->jumlahRiwayat < MAX_RIWAYAT) {
        get_current_date(p->riwayat[p->jumlahRiwayat].tanggal);
        p->riwayat[p->jumlahRiwayat].kaloriKonsumsi = p->totalKaloriHarianSaatIni;
        p->riwayat[p->jumlahRiwayat].kaloriTerbakar = kalori_terbakar_sekarang;
        p->riwayat[p->jumlahRiwayat].totalKaloriNet = kalori_net_harian;
        p->jumlahRiwayat++;
    } else {
        printf("Riwayat harian penuh. Tidak dapat menyimpan data hari ini.\n");
    }
}

void tampilkan_riwayat_pasien() {
    struct Pasien* p = cari_pasien();
    if (p == NULL) return;

    if (p->jumlahRiwayat == 0) {
        printf("Belum ada riwayat diet untuk pasien ini.\n");
        return;
    }

    printf("\n--- RIWAYAT DIET UNTUK %s ---\n", p->nama);
    int i;
	for (i = 0; i < p->jumlahRiwayat; i++) {
        printf("Tanggal: %s | Konsumsi: %.1f kalori | Terbakar: %.1f kalori | Net: %.1f kalori\n",
               p->riwayat[i].tanggal, p->riwayat[i].kaloriKonsumsi,
               p->riwayat[i].kaloriTerbakar, p->riwayat[i].totalKaloriNet);
    }
    printf("=============================\n");
}

void tampilkan_menu() {
    int pilih;
    do {
        printf("\n=========== MENU BMI & DIET CALCULATOR ==========\n");
        printf("1. Tambah Data Pasien Baru\n");
        printf("2. Lihat Semua Pasien Terdaftar\n");
        printf("3. Tampilkan Detail Pasien (BMI, BMR, TDEE)\n");
        printf("4. Perbarui Data Berat/Tinggi Pasien\n");
        printf("5. Hapus Pasien\n");
        printf("6. Mulai Program Diet & Rekomendasi (Input Kalori/Aktivitas)\n");
        printf("7. Tampilkan Riwayat Diet Pasien\n");
        printf("8. Keluar\n");
        printf("==================================================\n");
        printf("Pilih menu: ");
        scanf("%d", &pilih);

        switch (pilih) {
            case 1: input_data_pasien(); break;
            case 2: tampilkan_semua_pasien(); break;
            case 3: tampilkan_detail_pasien(); break;
            case 4: update_data_pasien(); break;
            case 5: hapus_data_pasien(); break;
            case 6: program_diet_rekomendasi(); break;
            case 7: tampilkan_riwayat_pasien(); break;
            case 8: printf("Terima kasih telah menggunakan program ini. Sampai jumpa!\n"); break;
            default: printf("Pilihan tidak valid. Silakan pilih menu yang tersedia (1-8).\n");
        }
    } while (pilih != 8);
}

int main() {
    tampilkan_menu();
	int i;
	
    for (i = 0; i < jumlahPasien; i++) {
        free(pasienList[i]);
    }

    return 0;
}
