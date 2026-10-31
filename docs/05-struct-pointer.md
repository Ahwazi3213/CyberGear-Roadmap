# Level 5: struct, enum, Pointer, static, dan NULL

[← Level 4](04-skala-float.md) · [Beranda](../README.md) · [Level 6: CAN Bus →](06-can-bus.md)

> [!NOTE]
> Tujuan level ini: membaca kode driver tanpa tersesat. Hampir semua potongan "aneh" di `cyber_gear.c` (`CG_Motor *m = ...`, `m->position = ...`, `static ... s_motors[]`) dijelaskan di sini. **Pointer adalah topik yang paling sering membuat pemula macet**, jadi kita jalan pelan dan membuktikannya lewat eksperimen.

## Tujuan belajar

- [ ] Memakai `#define`, `enum`, `typedef`, dan `struct`
- [ ] Menjelaskan pointer: `&`, `*`, dan `->`
- [ ] Membedakan *pass by value* dan *pass by pointer*
- [ ] Memahami `static` dan `NULL`, lalu membaca `cybergear_get_motor`

**Prasyarat:** [Level 3](03-endianness.md). Folder: `exercises/level-05-registry/`.

---

## 🧠 Konsep

### 1. `#define`, `enum`, `typedef`

```c
#define CYBERGEAR_MAX_MOTORS 8       // nama untuk sebuah nilai (tanpa '=' dan tanpa ';')

typedef enum {                       // kumpulan konstanta bernama
    CG_CONTROL  = 0x01,
    CG_FEEDBACK = 0x02,
    CG_ENABLE   = 0x03,
} CG_CommType;

typedef struct { float position; } CG_Motor;   // "CG_Motor" jadi nama tipe
CG_Motor m;
```

Gunanya: kode terbaca (`CG_ENABLE` lebih jelas daripada `0x03`) dan mudah diubah (ubah satu baris).

### 2. `struct`: satu paket data

```c
typedef struct {
    uint8_t id;
    float   position;
    float   temperature;
    bool    is_error;
} CG_Motor;

CG_Motor m;
m.id = 1;                // operator titik: kita memegang struct itu sendiri
m.position = 1.57f;
```

Tanpa struct, data satu motor tersebar di banyak variabel lepas, dan makin berantakan untuk dua motor.

### 3. Pointer: alamat, bukan isi

Setiap variabel menempati lokasi di memori yang punya **alamat**. **Pointer adalah variabel yang menyimpan alamat** variabel lain.

> Analogi: variabel biasa = **isi rumah**; pointer = **alamat rumah**. Dengan alamat kamu bisa mengunjungi rumah itu dan membaca atau mengubah isinya.

| Simbol | Nama | Arti | Contoh |
|---|---|---|---|
| `&x` | address-of | "alamat dari x" | `p = &x;` |
| `int *p` | deklarasi pointer | "p menyimpan alamat sebuah int" | |
| `*p` | dereference | "isi di alamat yang disimpan p" | `a = *p;` atau `*p = 20;` |

**Telusuri dengan tabel.** Alamat dibuat-buat supaya mudah dibaca:

```c
int x = 10;
int *p = &x;
*p = 20;
```

| Baris | Variabel | Alamat | Nilai |
|---|---|---|---|
| `int x = 10;` | `x` | `0x2000` | 10 |
| `int *p = &x;` | `p` | `0x2004` | `0x2000` (alamat x) |
| `*p = 20;` | `x` | `0x2000` | **20** (diubah lewat p) |

Perhatikan bahwa `p` sendiri berisi `0x2000`, dan `*p` artinya "buka rumah di `0x2000`".

### 4. Pass by value vs pass by pointer

```c
void tambah_salah(int n)  { n = n + 1; }       // n adalah SALINAN
void tambah_benar(int *n) { *n = *n + 1; }     // n adalah ALAMAT variabel asli

int x = 5;
tambah_salah(x);     // x tetap 5
tambah_benar(&x);    // x menjadi 6
```

Inilah alasan fungsi C yang harus **mengisi beberapa hasil** (atau mengubah struct) menerima pointer: `cybergear_get_motor` mengembalikan alamat motor, dan penerima frame menulis datanya langsung ke array lewat alamat itu.

### 5. Operator panah `->`

Jika yang kita pegang adalah **pointer ke struct**, akses anggota dengan `->`:

```c
CG_Motor *m = cybergear_get_motor(1);
(*m).position = 1.0f;     // dereference dulu, lalu titik
m->position   = 1.0f;     // sama persis, lebih singkat
```

| Kita memegang... | Akses anggota |
|---|---|
| struct itu sendiri (`CG_Motor m`) | `m.position` |
| pointer ke struct (`CG_Motor *m`) | `m->position` |

### 6. Array dan aritmetika pointer

Nama array tanpa kurung siku bernilai **alamat elemen pertamanya**. `a[i]` identik dengan `*(a + i)`.

> [!IMPORTANT]
> Penjumlahan pointer bekerja dalam satuan **elemen**, bukan byte. `p + 1` berarti "elemen berikutnya": untuk pointer ke `float` alamat bertambah 4 byte, untuk pointer ke `CG_Motor` bertambah `sizeof(CG_Motor)`.

Hubungan inilah yang membuat `&s_motors[i]` (alamat elemen ke-i) bermakna.

### 7. `static` dan `NULL`

| Kata kunci | Arti | Contoh di driver |
|---|---|---|
| `static` pada variabel **file** | hanya terlihat di file itu (privat) | `static CG_Motor s_motors[8];` |
| `static` pada variabel **fungsi** | nilainya bertahan antar pemanggilan | penghitung internal |
| `NULL` | "pointer yang tidak menunjuk ke apa pun" | `get_motor` mengembalikan `NULL` jika ID tidak ada |

Aturan emas: **cek `NULL` sebelum men-dereference.**

```c
CG_Motor *m = cybergear_get_motor(id);
if (m == NULL) return;        // ID tidak terdaftar
m->position = ...;
```

> [!CAUTION]
> Jangan pernah mengembalikan alamat **variabel lokal** dari fungsi; variabel itu lenyap saat fungsi selesai, dan alamatnya menunjuk ke sampah. Mengembalikan alamat elemen array `static` aman.

### 8. Gambaran untuk driver

```text
s_motors (array static, 8 elemen CG_Motor):
 [0] id=34 pos=... temp=...  ← cybergear_get_motor(34) mengembalikan alamat ini
 [1] id=77 pos=... temp=...
 [2] (kosong)
```

Penerima frame mencari motor dengan ID dari frame, mendapat **alamat** elemennya, lalu mengisi `m->position`, `m->temperature`, dan seterusnya. Perubahannya langsung tersimpan di array.

---

## 🧪 Latihan

### 5A. Lihat alamat sendiri

```c
#include <stdio.h>
int main(void) {
    int x = 10; int *p = &x;
    printf("x=%d  &x=%p  p=%p  *p=%d\n", x, (void *)&x, (void *)p, *p);
    *p = 20;
    printf("x sekarang = %d\n", x);
}
```

**Tebak dulu:** apakah `&x` dan `p` sama? Setelah `*p = 20`, apa nilai `x`? Jalankan dan cocokkan dengan tabel penelusuran.

### 5B. Pointer ke elemen array

Buat `float a[4] = {1,2,3,4}; float *p = a;` lalu cetak `p`, `p+1`, `p+2`. **Berapa byte selisih** alamat yang tercetak? Mengapa 4?

### 5C. Implementasi `registry`

Isi semua fungsi di `exercises/level-05-registry/registry.c` (daftar motor, dengan `registry_get` mengembalikan **alamat**).

```bash
cd exercises && make test-05
```

<details>
<summary>▶ Petunjuk 1: <code>registry_add</code></summary>

Urutan keputusannya: (1) cari dulu dengan `registry_get`; kalau sudah ada, `return true`. (2) kalau penuh (`s_count >= REG_MAX_MOTORS`), `return false`. (3) ambil `Motor *m = &s_motors[s_count++];` lalu isi `m->id = id;`.

</details>

<details>
<summary>▶ Petunjuk 2: kalau tes menampilkan "program crash"</summary>

Itu hampir pasti dereference `NULL`: `registry_get` mengembalikan `NULL` dan tes memakai hasilnya. Selesaikan `registry_get` dulu (loop `i` dari 0 sampai `s_count - 1`, `return &s_motors[i];` jika `id` cocok).

</details>

### 5D. Telusuri kode library

Buka `cyber_gear.c` dari project yang diberikan, cari `cybergear_get_motor` dan penerima frame (`cybergear_rx_handler`). Tulis, dalam kalimatmu sendiri: (1) apa yang dikembalikan `get_motor`, (2) kenapa penerima frame memakai `->`, (3) apa yang terjadi jika ID tidak terdaftar.

---

## ✅ Checkpoint: lulus jika

- [ ] `make test-05` menampilkan `SEMUA LOLOS`
- [ ] Kamu bisa menelusuri tabel pointer dengan tangan
- [ ] Kamu bisa menjelaskan kapan memakai `.` dan kapan `->`

## ❓ Cek pemahaman

**1.** `int a = 1, b = 2; int *p = &a; *p = b; b = 9;` Berapa `a`, `b`?

**2.** Kenapa `void f(int n) { n = 0; }` tidak mengubah variabel pemanggil?

**3.** `CG_Motor *m` memegang alamat motor. Tulis cara mengisi suhunya jadi 30.

**4.** Apa risikonya jika `get_motor` mengembalikan `NULL` dan kita langsung menulis `m->position`?

<details>
<summary>▶ Kunci jawaban (tulis jawabanmu dulu)</summary>

1. `*p = b` menyalin nilai b (2) ke a, lalu b diubah jadi 9: **a = 2, b = 9**.
2. Parameter `n` adalah salinan; mengubah salinan tidak menyentuh aslinya. Gunakan pointer (`int *n`, `*n = 0`).
3. `m->temperature = 30.0f;`
4. Menulis ke alamat 0 menyebabkan crash/HardFault pada MCU (atau memori rusak). Selalu cek `NULL` dulu.

</details>

---

[← Level 4](04-skala-float.md) · [Beranda](../README.md) · [Level 6: CAN Bus →](06-can-bus.md)
