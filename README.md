# finproprogc


| Anggota kelompok 6                | NPM        |
| ------------------- | ---------- |
| Shasya Nurhaliza    | 2306251576 |
| Michelle Winona     | 2306251525 |
| Nayla Danika Rahman | 2306251550 |

**Latar Belakang BMI Tracker**
Baik dokter maupun pasien menghadapi tantangan dalam pengelolaan gizi. Pasien sering kekurangan pemahaman personal tentang BMI, BMR, dan TDEE, sehingga panduan umum tidak efektif. Dokter kesulitan mengelola data pasien dan memberikan rekomendasi yang dipersonalisasi secara efisien.

Studi seperti yang diterbitkan di BMJ oleh Ordovas JM, et al. (2018) dan di The American Journal of Clinical Nutrition oleh Mozaffarian D, et al. (2018) menunjukkan bahwa pendekatan personal adalah kunci efektivitas. Ini menciptakan kebutuhan mendesak akan sistem yang mendukung pengelolaan data pasien yang efisien bagi dokter dan memberikan panduan kesehatan yang spesifik bagi pasien.

**Fitur Code**
* Registrasi dokter dan login dokter
* Registrasi pasien
* Update data pasien
* Hitung BMI & TDEE berdasarkan data input
* Simpan dan tampilkan data (dengan array of struct)
* Manipulasi data pasien (dengan pointer)

| Materi C              | Penggunaan dalam Program Kamu                                                                                              |
| --------------------- | -------------------------------------------------------------------------------------------------------------------------- |
|  **If Statement**    | Digunakan untuk: <br>• Login validasi (username/password) <br>• Cek duplikat username <br>• Validasi data (misal usia > 0) |
|  **While**           | Digunakan pada menu utama agar program terus berjalan sampai user memilih keluar                                           |
|  **Do While**        | Digunakan dalam submenu dokter atau pasien agar user bisa mengulangi pilihan                                       |
|  **For Loop**        | • Mengecek duplikat username dokter <br>• Menelusuri array pasien saat update <br>• Menampilkan data semua pasien          |
|  **Switch**          | Digunakan dalam **menu utama** dan **submenu dokter/pasien** untuk memilih fitur tertentu                                  |
|  **Function**        | Program dipisah dalam banyak fungsi: <br>• `tambah_pasien()` <br>• `login_dokter()` <br>• `hitung_bmi()` dll               |
|  **Array**           | • Menyimpan data pasien dan dokter <br>• String input <br>• List makanan pasien                                            |
|  **Pointer I**       | • Digunakan untuk manipulasi struct pasien via parameter fungsi: `Pasien *p`                                               |
|  **Pointer II**      | • Mengakses field struct via pointer: `p->berat`, `p->tinggi`                                                           |
|  **Struct**          | • `struct Pasien` menyimpan semua data pasien <br>• `struct Dokter` menyimpan akun login                                   |
