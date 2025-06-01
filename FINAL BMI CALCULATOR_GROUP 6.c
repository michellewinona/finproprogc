#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

#define MAX_PASIEN 100
#define MAX_DOKTER 10

typedef struct {
    char id[10];
    char nama[50];
    char gender;
    int usia;
    float berat;
    float tinggi;
    float bmi;
    char status_bmi[20];
    float bmr;
    float tdee;
    char aktivitas[30];
} Pasien;

typedef struct {
    char username[20];
    char password[20];
} Dokter;

Pasien daftar_pasien[MAX_PASIEN];
int jumlah_pasien = 0;

Dokter daftar_dokter[MAX_DOKTER];
int jumlah_dokter = 0;

void clear_screen() {
    system("cls || clear");
}

void garis_pemisah() {
    printf("--------------------------------------------------\n");
}

void tampilkan_header(const char* judul) {
    clear_screen();
    garis_pemisah();
    printf("          %s\n", judul);
    garis_pemisah();
    printf("\n");
}

void hitung_bmi(Pasien *p) {
    p->bmi = p->berat / pow((p->tinggi/100), 2);
    
    if (p->bmi < 18.5) {
        strcpy(p->status_bmi, "Underweight");
    } else if (p->bmi < 25) {
        strcpy(p->status_bmi, "Normal");
    } else if (p->bmi < 30) {
        strcpy(p->status_bmi, "Overweight");
    } else {
        strcpy(p->status_bmi, "Obese");
    }
}

float hitung_bmr(Pasien p) {
    if (p.gender == 'L') {
        return (10 * p.berat) + (6.25 * p.tinggi) - (5 * p.usia) + 5;
    } else {
        return (10 * p.berat) + (6.25 * p.tinggi) - (5 * p.usia) - 161;
    }
}

float hitung_tdee(float bmr, const char* aktivitas) {
    float faktor;
    
    if (strcmp(aktivitas, "Sedentary") == 0) {
        faktor = 1.2;
    } else if (strcmp(aktivitas, "Ringan") == 0) {
        faktor = 1.375;
    } else if (strcmp(aktivitas, "Sedang") == 0) {
        faktor = 1.55;
    } else if (strcmp(aktivitas, "Berat") == 0) {
        faktor = 1.725;
    } else if (strcmp(aktivitas, "Sangat Aktif") == 0) {
        faktor = 1.9;
    } else {
        faktor = 1.2; 
    }
    
    return bmr * faktor;
}

void tampilkan_menu_utama() {
    tampilkan_header("SKINNY QUEEN CLINIC");
    printf("1. Registrasi Dokter\n");
    printf("2. Login sebagai Dokter\n");
    printf("3. Login sebagai Pasien\n");
    printf("4. Keluar\n");
    printf("\nPilih menu (1-4): ");
}

void tampilkan_menu_dokter() {
    tampilkan_header("MENU DOKTER");
    printf("1. Manajemen Pasien\n");
    printf("2. Perhitungan Kesehatan Pasien\n");
    printf("3. Catatan Dokter untuk Pasien\n");
    printf("4. Kembali ke Menu Utama\n");
    printf("\nPilih menu (1-4): ");
}

void tampilkan_submenu_manajemen_pasien() {
    tampilkan_header("MANAJEMEN PASIEN");
    printf("1. Tambah Pasien Baru\n");
    printf("2. Lihat Data Pasien\n");
    printf("3. Update Data Pasien\n");
    printf("4. Kembali ke Menu Dokter\n");
    printf("\nPilih menu (1-4): ");
}

void tampilkan_menu_pasien() {
    tampilkan_header("MENU PASIEN");
    printf("1. Lihat Data Saya\n");
    printf("2. Perbarui Data Aktivitas & Hitung Ulang TDEE Saya\n");
    printf("3. Catatan Dokter untuk Saya\n");
    printf("4. Kembali ke Menu Utama\n");
    printf("\nPilih menu (1-4): ");
}

void tambah_pasien_data_dasar(Pasien *p) {
    int i;
    char confirm_char;
    int temp_usia;

    printf("  Masukkan ID Pasien (contoh: P001): ");
    scanf("%s", p->id);
    getchar();

    for (i = 0; i < jumlah_pasien; i++) {
        if (strcmp(daftar_pasien[i].id, p->id) == 0) {
            printf("  ID Pasien sudah ada. Mohon gunakan ID lain.\n");
            strcpy(p->id, "");
            return;
        }
    }

    printf("  Nama Pasien: ");
    scanf(" %[^\n]s", p->nama);
    
    printf("  Jenis Kelamin (L/P): ");
    scanf(" %c", &p->gender);
    p->gender = toupper(p->gender);
    getchar();

    do {
        printf("  Usia: ");
        scanf("%d", &temp_usia);
        getchar();

        if (temp_usia > 120) {
            printf("  Usia %d tahun terlihat tidak biasa. Apakah Anda yakin usia ini benar (Y/T)? ", temp_usia);
            scanf(" %c", &confirm_char);
            getchar();
            confirm_char = toupper(confirm_char);

            if (confirm_char == 'Y') {
                printf("  Dokter tidak dapat menerima pasien ini karena usia yang tidak wajar (>120 tahun).\n");
                strcpy(p->id, "");
                return; 
            } else if (confirm_char == 'T') {
                printf("  Silakan masukkan usia yang benar.\n");
            } else {
                printf("  Input tidak valid. Silakan masukkan 'Y' atau 'T'.\n");
            }
        } else if (temp_usia <= 0) {
            printf("  Usia harus lebih dari 0. Silakan masukkan usia yang benar.\n");
        }
    } while (temp_usia > 120 || temp_usia <= 0);
    p->usia = temp_usia;
    
    printf("  Berat Badan (kg): ");
    scanf("%f", &p->berat);
    getchar();
    
    printf("  Tinggi Badan (cm): ");
    scanf("%f", &p->tinggi);
    getchar();
    
    hitung_bmi(p);
    
    strcpy(p->aktivitas, "Belum diinput");
    p->bmr = 0.0;
    p->tdee = 0.0;
}

int cari_pasien(const char* id) {
    int i;
    for (i = 0; i < jumlah_pasien; i++) {
        if (strcmp(daftar_pasien[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

void tampilkan_data_pasien(int index, int role_dokter) {
    if (index == -1) {
        printf("Data pasien tidak ditemukan.\n");
        return;
    }
    Pasien p = daftar_pasien[index];
    
    float ideal_berat_min = 18.5 * pow((p.tinggi/100), 2);
    float ideal_berat_max = 24.9 * pow((p.tinggi/100), 2);

    garis_pemisah();
    printf("DETAIL DATA PASIEN\n");
    garis_pemisah();
    printf("  ID Pasien         : %s\n", p.id);
    printf("  Nama              : %s\n", p.nama);
    printf("  Jenis Kelamin     : %c\n", p.gender);
    printf("  Usia              : %d tahun\n", p.usia);
    printf("  Berat Badan       : %.1f kg\n", p.berat);
    printf("  Tinggi Badan      : %.1f cm\n", p.tinggi);
    printf("  BMI               : %.1f (%s)\n", p.bmi, p.status_bmi);
    printf("  Rentang BB Ideal  : %.1f - %.1f kg (untuk BMI Normal)\n", ideal_berat_min, ideal_berat_max);

    if (strcmp(p.aktivitas, "Belum diinput") != 0 || role_dokter == 1) {
        printf("  BMR               : %.1f kalori/hari\n", p.bmr);
        printf("  Aktivitas         : %s\n", p.aktivitas);
        printf("  TDEE              : %.1f kalori/hari\n", p.tdee);
    } else {
        printf("  Aktivitas         : %s\n", p.aktivitas);
        printf("  BMR dan TDEE belum dihitung. Silakan perbarui aktivitas.\n");
    }
    garis_pemisah();
}

void update_pasien() {
    char id[10];
    char input_str[50];
    int index;
    int i;
    int is_numeric;
    int decimal_count;
    char confirm_char;
    int temp_usia;

    tampilkan_header("UPDATE DATA PASIEN");
    
    printf("  Masukkan ID Pasien yang ingin diupdate: ");
    scanf("%s", id);
    getchar();
    
    index = cari_pasien(id);
    if (index == -1) {
        printf("  Pasien tidak ditemukan!\n");
        return;
    }
    
    Pasien *p = &daftar_pasien[index];
    
    printf("\n  Data saat ini:\n");
    tampilkan_data_pasien(index, 1);
    
    printf("\n  Masukkan data baru (ketik '-' jika tidak ingin mengubah):\n");
    
    printf("  Nama [%s]: ", p->nama);
    scanf(" %[^\n]s", input_str);
    if (strcmp(input_str, "-") != 0) {
        strcpy(p->nama, input_str);
    }
    
    printf("  Jenis Kelamin [%c] (L/P): ", p->gender);
    scanf(" %s", input_str);
    if (strcmp(input_str, "-") != 0) {
        if (strlen(input_str) > 0) {
            p->gender = toupper(input_str[0]);
        }
    }
    getchar();

    printf("  Usia [%d]: ", p->usia);
    scanf("%s", input_str);
    if (strcmp(input_str, "-") != 0) {
        if (strlen(input_str) > 0) {
            is_numeric = 1;
            for (i = 0; i < strlen(input_str); i++) {
                if (!isdigit(input_str[i])) {
                    is_numeric = 0;
                    break;
                }
            }
            if (is_numeric) {
                temp_usia = atoi(input_str);

                if (temp_usia > 120) {
                    printf("  Usia %d tahun terlihat tidak biasa. Apakah Anda yakin usia ini benar (Y/T)? ", temp_usia);
                    scanf(" %c", &confirm_char);
                    getchar();
                    confirm_char = toupper(confirm_char);

                    if (confirm_char == 'Y') {
                        printf("  Dokter tidak dapat menerima usia >120 tahun. Usia pasien tidak diubah.\n");
                    } else if (confirm_char == 'T') {
                        printf("  Usia tidak diubah. Silakan update lagi dengan usia yang benar jika diperlukan.\n");
                    } else {
                        printf("  Input konfirmasi tidak valid. Usia tidak diubah.\n");
                    }
                } else if (temp_usia <= 0) {
                    printf("  Usia harus lebih dari 0. Usia tidak diubah.\n");
                } else {
                    p->usia = temp_usia;
                }
            } else {
                printf("  Input usia tidak valid. Usia tidak diubah.\n");
            }
        }
    }
    getchar();
    
    printf("  Berat Badan (kg) [%.1f]: ", p->berat);
    scanf("%s", input_str);
    if (strcmp(input_str, "-") != 0) {
        if (strlen(input_str) > 0) {
            is_numeric = 1;
            decimal_count = 0;
            for (i = 0; i < strlen(input_str); i++) {
                if (input_str[i] == '.') {
                    decimal_count++;
                } else if (!isdigit(input_str[i])) {
                    is_numeric = 0;
                    break;
                }
            }
            if (is_numeric && decimal_count <= 1) {
                p->berat = atof(input_str);
            } else {
                printf("  Input berat badan tidak valid. Berat badan tidak diubah.\n");
            }
        }
    }
    getchar();
    
    printf("  Tinggi Badan (cm) [%.1f]: ", p->tinggi);
    scanf("%s", input_str);
    if (strcmp(input_str, "-") != 0) {
        if (strlen(input_str) > 0) {
            is_numeric = 1;
            decimal_count = 0;
            for (i = 0; i < strlen(input_str); i++) {
                if (input_str[i] == '.') {
                    decimal_count++;
                } else if (!isdigit(input_str[i])) {
                    is_numeric = 0;
                    break;
                }
            }
            if (is_numeric && decimal_count <= 1) {
                p->tinggi = atof(input_str);
            } else {
                printf("  Input tinggi badan tidak valid. Tinggi badan tidak diubah.\n");
            }
        }
    }
    getchar();
    
    hitung_bmi(p);
    p->bmr = hitung_bmr(*p);
    if (strcmp(p->aktivitas, "Belum diinput") != 0) {
        p->tdee = hitung_tdee(p->bmr, p->aktivitas);
    }
    
    printf("\n  Data pasien berhasil diperbarui!\n");
}

void hitung_bmi_tdee_dokter() {
    char id[10];
    int index;
    int pilihan;
    const char* aktivitas_str;

    tampilkan_header("PERHITUNGAN KESEHATAN PASIEN");
    
    printf("  Masukkan ID Pasien: ");
    scanf("%s", id);
    getchar();
    
    index = cari_pasien(id);
    if (index == -1) {
        printf("  Pasien tidak ditemukan!\n");
        return;
    }
    
    Pasien *p = &daftar_pasien[index];
    
    hitung_bmi(p);
    
    printf("\n  Pilih level aktivitas pasien %s (%s):\n", p->nama, p->id);
    printf("  1. Sedentary (sedikit atau tanpa olahraga)\n");
    printf("  2. Ringan (olahraga ringan 1-3 hari/minggu)\n");
    printf("  3. Sedang (olahraga sedang 3-5 hari/minggu)\n");
    printf("  4. Berat (olahraga berat 6-7 hari/minggu)\n");
    printf("  5. Sangat aktif (olahraga sangat berat setiap hari)\n");
    printf("\n  Pilihan (1-5): ");
    
    scanf("%d", &pilihan);
    getchar();
    
    switch(pilihan) {
        case 1: aktivitas_str = "Sedentary"; break;
        case 2: aktivitas_str = "Ringan"; break;
        case 3: aktivitas_str = "Sedang"; break;
        case 4: aktivitas_str = "Berat"; break;
        case 5: aktivitas_str = "Sangat Aktif"; break;
        default: 
            printf("  Pilihan tidak valid, aktivitas diatur sebagai Sedentary.\n");
            aktivitas_str = "Sedentary";
    }
    
    strcpy(p->aktivitas, aktivitas_str);
    
    p->bmr = hitung_bmr(*p);
    p->tdee = hitung_tdee(p->bmr, p->aktivitas);
    
    printf("\n  Hasil Perhitungan untuk Pasien %s (%s):\n", p->nama, p->id);
    tampilkan_data_pasien(index, 1);
}

void catatan_dokter_untuk_pasien() {
    char id[10];
    int index;
    
    tampilkan_header("CATATAN DOKTER UNTUK PASIEN");
    
    printf("  Masukkan ID Pasien: ");
    scanf("%s", id);
    getchar();
    
    index = cari_pasien(id);
    if (index == -1) {
        printf("  Pasien tidak ditemukan!\n");
        return;
    }
    
    Pasien *p = &daftar_pasien[index];
    
    printf("\n  Data Pasien:\n");
    tampilkan_data_pasien(index, 1);
    
    printf("\n--- Catatan Diet Medis untuk Pasien %s ---\n", p->nama);
    
    if (strcmp(p->aktivitas, "Belum diinput") == 0) {
        printf("  Catatan: Data aktivitas pasien belum diinput. Rekomendasi mungkin kurang spesifik.\n");
        printf("  Harap perbarui data aktivitas untuk perhitungan TDEE yang akurat.\n");
    }

    if (strcmp(p->status_bmi, "Underweight") == 0) {
        printf("\n  Status BMI: Underweight (BMI: %.1f)\n", p->bmi);
        printf("  Tujuan: Mencapai BMI Normal (18.5 - 24.9) dan peningkatan berat badan yang sehat.\n");
        printf("  Estimasi Peningkatan BB: 0.25 - 0.5 kg per minggu.\n");
        printf("\n  Strategi Diet:\n");
        printf("    - Peningkatan Asupan Kalori: Disarankan penambahan 300-500 kalori/hari dari TDEE pasien (TDEE saat ini: %.1f kalori/hari).\n", p->tdee);
        printf("    - Fokus Makanan Padat Nutrisi: Pilih makanan tinggi kalori dan nutrisi. Contoh: karbohidrat kompleks (nasi, roti gandum, ubi), protein berkualitas (daging tanpa lemak, ayam, ikan, telur, produk susu, tahu, tempe), lemak sehat (alpukat, kacang-kacangan, minyak zaitun).\n");
        printf("    - Porsi & Frekuensi: Makan 5-6 kali sehari dengan porsi sedang, bukan hanya 3 porsi besar. Sertakan camilan sehat di antara waktu makan utama.\n");
        printf("    - Minuman: Konsumsi minuman berkalori sehat seperti susu, jus buah murni, atau smoothie, hindari minuman nol kalori sebelum makan.\n");
        printf("    - Hindari: Makanan cepat saji dan olahan tinggi gula/garam/lemak trans yang minim nutrisi.\n");
        printf("\n  Strategi Aktivitas Fisik:\n");
        printf("    - Fokus pada latihan kekuatan untuk membangun massa otot, bukan hanya lemak. Konsultasikan dengan pelatih fisik jika memungkinkan.\n");
    }
    else if (strcmp(p->status_bmi, "Normal") == 0) {
        printf("\n  Status BMI: Normal (BMI: %.1f)\n", p->bmi);
        printf("  Tujuan: Mempertahankan BMI ideal dan menjaga kesehatan optimal.\n");
        printf("\n  Strategi Diet:\n");
        printf("    - Keseimbangan Kalori: Pertahankan asupan kalori sesuai TDEE (saat ini: %.1f kalori/hari) untuk menjaga berat badan.\n", p->tdee);
        printf("    - Diversifikasi Makanan: Konsumsi makanan seimbang dari semua kelompok makanan: buah dan sayuran berlimpah, biji-bijian utuh, protein tanpa lemak, dan lemak sehat.\n");
        printf("    - Kontrol Porsi: Tetap perhatikan porsi makan agar tidak terjadi kelebihan kalori.\n");
        printf("    - Hidrasi: Pastikan asupan air yang cukup sepanjang hari.\n");
        printf("    - Batasi: Makanan tinggi gula, garam, dan lemak jenuh yang berlebihan.\n");
        printf("\n  Strategi Aktivitas Fisik:\n");
        printf("    - Lakukan aktivitas fisik moderat secara teratur setidaknya 150 menit per minggu atau 75 menit aktivitas intens.\n");
    }
    else if (strcmp(p->status_bmi, "Overweight") == 0 || strcmp(p->status_bmi, "Obese") == 0) {
        printf("\n  Status BMI: %s (BMI: %.1f)\n", p->status_bmi, p->bmi);
        printf("  Tujuan: Mencapai BMI Normal (18.5 - 24.9) dan penurunan berat badan secara sehat.\n");
        printf("  Estimasi Penurunan BB: 0.5 - 1 kg per minggu (disarankan defisit 500-750 kalori/hari).\n");
        printf("\n  Strategi Diet:\n");
        printf("    - Defisit Kalori Terkontrol: Kurangi asupan kalori dari TDEE (saat ini: %.1f kalori/hari) dengan defisit sekitar 500-750 kalori per hari.\n", p->tdee);
        printf("    - Fokus Makanan Padat Nutrisi Rendah Kalori: Perbanyak konsumsi sayuran hijau, buah-buahan, protein tanpa lemak (dada ayam tanpa kulit, ikan, tahu, tempe, telur), dan karbohidrat kompleks berserat tinggi (nasi merah, oatmeal, roti gandum).\n");
        printf("    - Kontrol Porsi: Sangat penting untuk memperhatikan ukuran porsi dan menghindari makan berlebihan.\n");
        printf("    - Hidrasi: Minum air putih yang cukup untuk membantu rasa kenyang dan metabolisme.\n");
        printf("\n  Pantangan Makanan:\n");
        printf("    - Gula Tambahan: Minuman manis (soda, teh kemasan manis), kue, permen, biskuit, sereal manis.\n");
        printf("    - Lemak Jenuh & Trans: Gorengan, makanan cepat saji, makanan olahan (sosis, nugget), margarin, mentega berlebihan.\n");
        printf("    - Karbohidrat Olahan: Roti putih, nasi putih berlebihan, pasta instan.\n");
        printf("    - Alkohol: Tinggi kalori dan memicu nafsu makan.\n");
        printf("    - Junk Food: Keripik, camilan gurih tinggi garam dan lemak.\n");
        printf("\n  Strategi Aktivitas Fisik:\n");
        printf("    - Peningkatan Aktivitas: Mulai dengan aktivitas ringan (jalan kaki cepat 30 menit/hari) dan tingkatkan intensitas serta durasi secara bertahap (jogging, berenang, bersepeda).\n");
        printf("    - Gabungkan latihan kardio dan kekuatan untuk memaksimalkan pembakaran kalori dan membangun otot.\n");
        printf("\n  Rekomendasi Tambahan:\n");
        printf("    - Pertimbangkan konsultasi dengan ahli gizi/dokter untuk rencana diet yang lebih personal dan pemantauan progres.\n");
    }
    printf("--------------------------------------------------\n");
}

void catatan_pasien(int index) {
    Pasien p = daftar_pasien[index];
    
    tampilkan_header("CATATAN DOKTER UNTUK SAYA");
    printf("\n  Data Anda:\n");
    tampilkan_data_pasien(index, 0);
    
    printf("\n--- Catatan Diet Personal ---\n");

    if (strcmp(p.aktivitas, "Belum diinput") == 0) {
        printf("  Data aktivitas Anda belum diinput. Mohon perbarui data aktivitas Anda di menu 'Perbarui Data Aktivitas & Hitung Ulang TDEE Saya' untuk mendapatkan catatan diet yang akurat.\n");
    } else if (strcmp(p.status_bmi, "Underweight") == 0) {
        printf("\n  Anda berada dalam kategori Berat Badan Kurang.\n");
        printf("  Tujuan Anda: Menambah berat badan secara sehat untuk mencapai berat ideal.\n");
        printf("  Target: Coba naik sekitar 0.25 - 0.5 kg per minggu.\n");
        printf("\n  Yang bisa Anda lakukan:\n");
        printf("    - Perbanyak porsi makan sehat: Tambahkan nasi, roti gandum, daging, telur, ikan, tahu, tempe, alpukat, dan kacang-kacangan dalam diet Anda.\n");
        printf("    - Makan lebih sering: Coba makan 5-6 kali sehari dengan porsi yang tidak terlalu besar.\n");
        printf("    - Camilan sehat: Konsumsi buah-buahan, yogurt, atau roti gandum dengan selai kacang di antara waktu makan.\n");
        printf("    - Olahraga: Lakukan latihan beban untuk membangun otot, bukan hanya lemak.\n");
        printf("    - Hindari: Minuman bersoda dan makanan ringan yang tinggi gula namun rendah nutrisi.\n");
    }
    else if (strcmp(p.status_bmi, "Normal") == 0) {
        printf("\n  Selamat, BMI Anda Normal!\n");
        printf("  Tujuan Anda: Mempertahankan berat badan ideal dan hidup sehat.\n");
        printf("\n  Yang bisa Anda lakukan:\n");
        printf("    - Makan seimbang: Tetap konsumsi berbagai jenis makanan sehat seperti sayuran, buah-buahan, nasi/roti gandum, daging tanpa lemak, dan ikan.\n");
        printf("    - Jaga porsi: Makanlah secukupnya, jangan terlalu kenyang.\n");
        printf("    - Tetap aktif: Lakukan olahraga teratur, seperti jalan kaki, lari, atau berenang.\n");
        printf("    - Minum air: Pastikan minum air putih yang cukup setiap hari.\n");
    }
    else if (strcmp(p.status_bmi, "Overweight") == 0 || strcmp(p.status_bmi, "Obese") == 0) {
        printf("\n  Anda berada dalam kategori Berat Badan Berlebih/Obesitas.\n");
        printf("  Tujuan Anda: Menurunkan berat badan secara sehat untuk mencapai berat ideal.\n");
        printf("  Target: Coba turun sekitar 0.5 - 1 kg per minggu.\n");
        printf("\n  Yang bisa Anda lakukan:\n");
        printf("    - Kurangi porsi makan: Ambil porsi lebih kecil dari biasanya.\n");
        printf("    - Pilih makanan yang tepat: Perbanyak sayuran dan buah, pilih protein tanpa lemak (dada ayam, ikan, tahu, tempe), dan karbohidrat kompleks (nasi merah, ubi).\n");
        printf("    - Tingkatkan aktivitas fisik: Mulai dengan jalan kaki cepat setiap hari, lalu tingkatkan intensitasnya.\n");
        printf("\n  Pantangan Makanan (Hindari atau Batasi):\n");
        printf("    - Minuman Manis: Soda, teh manis kemasan, minuman berenergi.\n");
        printf("    - Gorengan dan Makanan Cepat Saji: Contohnya ayam goreng tepung, kentang goreng, burger.\n");
        printf("    - Camilan Tinggi Gula/Lemak: Permen, kue, biskuit, keripik.\n");
        printf("    - Nasi Putih Berlebihan & Roti Putih: Lebih baik ganti dengan nasi merah atau roti gandum.\n");
        printf("  - Minum air putih: Perbanyak minum air putih, terutama sebelum makan.\n");
    }
    printf("--------------------------------------------------\n");
}

void manajemen_pasien() {
    int pilihan_manajemen;
    int i;

    do {
        tampilkan_submenu_manajemen_pasien();
        scanf("%d", &pilihan_manajemen);
        getchar();

        switch (pilihan_manajemen) {
            case 1:
                tampilkan_header("TAMBAH PASIEN BARU");
                if (jumlah_pasien >= MAX_PASIEN) {
                    printf("  Maaf, kapasitas pasien penuh.\n");
                } else {
                    Pasien p;
                    strcpy(p.id, "");
                    tambah_pasien_data_dasar(&p);
                    if (strlen(p.id) > 0) {
                        daftar_pasien[jumlah_pasien++] = p;
                        printf("\n  Pasien berhasil ditambahkan!\n");
                    } else {
                        printf("\n  Penambahan pasien dibatalkan.\n");
                    }
                }
                break;
            case 2:
                tampilkan_header("LIHAT DATA PASIEN");
                if (jumlah_pasien == 0) {
                    printf("  Belum ada pasien terdaftar.\n");
                } else {
                    for (i = 0; i < jumlah_pasien; i++) {
                        printf("\n--- Pasien %d ---\n", i + 1);
                        tampilkan_data_pasien(i, 1);
                    }
                }
                break;
            case 3:
                update_pasien();
                break;
            case 4:
                printf("  Kembali ke Menu Dokter...\n");
                break;
            default:
                printf("  Pilihan tidak valid. Silakan coba lagi.\n");
        }
        printf("\n  Tekan ENTER untuk melanjutkan...");
        getchar();
    } while (pilihan_manajemen != 4);
}

void run_menu_dokter() {
    int pilihan_dokter;

    do {
        tampilkan_menu_dokter();
        scanf("%d", &pilihan_dokter);
        getchar();

        switch (pilihan_dokter) {
            case 1:
                manajemen_pasien();
                break;
            case 2:
                hitung_bmi_tdee_dokter();
                break;
            case 3:
                catatan_dokter_untuk_pasien();
                break;
            case 4:
                printf("  Kembali ke Menu Utama...\n");
                break;
            default:
                printf("  Pilihan tidak valid. Silakan coba lagi.\n");
        }
        printf("\n  Tekan ENTER untuk melanjutkan...");
        getchar();
    } while (pilihan_dokter != 4);
}

void run_menu_pasien() {
    char id_pasien[10];
    int index_pasien;
    int pilihan_pasien;
    int pilihan_aktivitas;
    const char* aktivitas_str;

    tampilkan_header("LOGIN PASIEN");
    printf("  Masukkan ID Pasien Anda: ");
    scanf("%s", id_pasien);
    getchar();

    index_pasien = cari_pasien(id_pasien);
    if (index_pasien == -1) {
        printf("\n  ID Pasien tidak ditemukan. Mohon hubungi dokter Anda untuk pendaftaran.\n");
        printf("  Tekan ENTER untuk kembali ke Menu Utama...");
        getchar();
        return;
    }

    do {
        tampilkan_menu_pasien();
        scanf("%d", &pilihan_pasien);
        getchar();

        switch (pilihan_pasien) {
            case 1:
                tampilkan_header("DATA SAYA");
                tampilkan_data_pasien(index_pasien, 0);
                break;
            case 2:
                { 
                    Pasien *p = &daftar_pasien[index_pasien];

                    tampilkan_header("PERBARUI DATA AKTIVITAS & HITUNG ULANG TDEE SAYA");
                    hitung_bmi(p);

                    printf("\n  Pilih level aktivitas Anda (%s):\n", p->nama);
                    printf("  1. Sedentary (sedikit atau tanpa olahraga)\n");
                    printf("  2. Ringan (olahraga ringan 1-3 hari/minggu)\n");
                    printf("  3. Sedang (olahraga sedang 3-5 hari/minggu)\n");
                    printf("  4. Berat (olahraga berat 6-7 hari/minggu)\n");
                    printf("  5. Sangat aktif (olahraga sangat berat setiap hari)\n");
                    printf("\n  Pilihan (1-5): ");
                    scanf("%d", &pilihan_aktivitas);
                    getchar();

                    switch(pilihan_aktivitas) {
                        case 1: aktivitas_str = "Sedentary"; break;
                        case 2: aktivitas_str = "Ringan"; break;
                        case 3: aktivitas_str = "Sedang"; break;
                        case 4: aktivitas_str = "Berat"; break;
                        case 5: aktivitas_str = "Sangat Aktif"; break;
                        default: 
                            printf("  Pilihan tidak valid, aktivitas diatur sebagai Sedentary.\n");
                            aktivitas_str = "Sedentary";
                    }
                    strcpy(p->aktivitas, aktivitas_str);
                    p->bmr = hitung_bmr(*p);
                    p->tdee = hitung_tdee(p->bmr, p->aktivitas);
                    printf("\n  Hasil Perhitungan Anda:\n");
                    tampilkan_data_pasien(index_pasien, 0);
                }
                break;
            case 3:
                catatan_pasien(index_pasien);
                break;
            case 4:
                printf("  Kembali ke Menu Utama...\n");
                break;
            default:
                printf("  Pilihan tidak valid. Silakan coba lagi.\n");
        }
        printf("\n  Tekan ENTER untuk melanjutkan...");
        getchar();
    } while (pilihan_pasien != 4);
}

void registrasi_dokter() {
    int i;

    tampilkan_header("REGISTRASI DOKTER BARU");

    if (jumlah_dokter >= MAX_DOKTER) {
        printf("  Maaf, kapasitas dokter penuh.\n");
        return;
    }

    Dokter d;
    printf("  Masukkan Username Dokter baru: ");
    scanf("%s", d.username);
    getchar();

    for (i = 0; i < jumlah_dokter; i++) {
        if (strcmp(daftar_dokter[i].username, d.username) == 0) {
            printf("  Username Dokter sudah ada. Mohon gunakan username lain.\n");
            return;
        }
    }

    printf("  Masukkan Password Dokter baru: ");
    scanf("%s", d.password);
    getchar();

    daftar_dokter[jumlah_dokter++] = d;
    printf("\n  Registrasi Dokter berhasil! Silakan login.\n");
}

int otentikasi_dokter(const char* username, const char* password) {
    int i;
    for (i = 0; i < jumlah_dokter; i++) {
        if (strcmp(daftar_dokter[i].username, username) == 0 && strcmp(daftar_dokter[i].password, password) == 0) {
            return 1;
        }
    }
    return 0;
}

int main() {
    int pilihan_utama;
    char user[20], pass[20];

    if (jumlah_dokter == 0) {
        strcpy(daftar_dokter[0].username, "dokter");
        strcpy(daftar_dokter[0].password, "gizi");
        jumlah_dokter = 1;
    }

    do {
        tampilkan_menu_utama();
        scanf("%d", &pilihan_utama);
        getchar();

        switch (pilihan_utama) {
            case 1:
                registrasi_dokter();
                break;
            case 2:
                printf("\n--- Login Dokter ---\n");
                printf("  Username: ");
                scanf("%s", user);
                printf("  Password: ");
                scanf("%s", pass);
                getchar();

                if (otentikasi_dokter(user, pass)) {
                    printf("\n  Login Dokter Berhasil!\n");
                    printf("  Tekan ENTER untuk melanjutkan...");
                    getchar();
                    run_menu_dokter();
                } else {
                    printf("\n  Username atau password salah.\n");
                    printf("  Tekan ENTER untuk melanjutkan...");
                    getchar();
                }
                break;
            case 3:
                run_menu_pasien();
                break;
            case 4:
                printf("\n  Terima kasih telah menggunakan SKINNY QUEEN CLINIC.\n");
                break;
            default:
                printf("\n  Pilihan tidak valid. Silakan coba lagi.\n");
                printf("  Tekan ENTER untuk melanjutkan...");
                getchar();
        }
    } while (pilihan_utama != 4);

    return 0;
}
