# Level 1: Biner, Heksadesimal, dan Operator Bitwise

[← Level 0](00-peta-tugas.md) · [Beranda](../README.md) · [Level 2: Extended ID →](02-extended-id.md)

> [!NOTE]
> Tujuan level ini: membaca dan memanipulasi **bit** dengan lancar. Hampir semua yang terjadi di protokol CAN (ID 29 bit, flag fault, mode motor) adalah permainan bit.

## Tujuan belajar

- [ ] Mengonversi biner ↔ heksadesimal ↔ desimal
- [ ] Memakai `<<`, `>>`, `&`, `|`, `^`, `~`
- [ ] Menyalakan, mematikan, membalik, dan menguji satu bit
- [ ] Mengambil dan menulis *field* bit dengan mask

**Prasyarat:** [Level 0](00-peta-tugas.md). Folder latihan: `exercises/level-01-bitwise/`.

---

## 🧠 Konsep

### 1. Biner dan heksadesimal

Komputer menyimpan semuanya sebagai bit (0 atau 1). Heksadesimal (hex) adalah cara singkat menulis biner: **satu digit hex = tepat 4 bit**, dan **satu byte = dua digit hex**.

| Hex | Biner | Hex | Biner | Hex | Biner | Hex | Biner |
|---|---|---|---|---|---|---|---|
| 0 | 0000 | 4 | 0100 | 8 | 1000 | C | 1100 |
| 1 | 0001 | 5 | 0101 | 9 | 1001 | D | 1101 |
| 2 | 0010 | 6 | 0110 | A | 1010 | E | 1110 |
| 3 | 0011 | 7 | 0111 | B | 1011 | F | 1111 |

**Contoh:** `0xFD` = `1111 1101` = 253. Cara mengubah ke desimal: $15 \times 16 + 13 = 253$.

Posisi bit dihitung dari kanan, mulai **0**: bit 0 adalah **LSB** (paling tidak signifikan), bit tertinggi adalah **MSB**.

```text
bit:      7 6 5 4 3 2 1 0
nilai:    1 0 1 0 0 1 0 1     = 0xA5
```

### 2. Enam operator bitwise

Operator bitwise bekerja **per posisi bit**, berbeda dengan operator logika `&&`/`||` yang menilai benar/salah keseluruhan.

| Operator | Nama | Contoh | Hasil | Kegunaan |
|---|---|---|---|---|
| `<<` | geser kiri | `0000 0001 << 4` | `0001 0000` | pindah ke posisi tinggi (× 2ⁿ) |
| `>>` | geser kanan | `1010 0101 >> 4` | `0000 1010` | pindah ke posisi rendah (÷ 2ⁿ) |
| `&` | AND | `1010 0101 & 0000 1111` | `0000 0101` | **mengambil** (mask) bit tertentu |
| `\|` | OR | `1010 0000 \| 0000 0101` | `1010 0101` | **menggabungkan** field / menyalakan bit |
| `^` | XOR | `1111 1111 ^ 0000 1111` | `1111 0000` | **membalik** bit tertentu |
| `~` | NOT | `~(0000 1111)` | `1111 0000` | membalik semua bit |

Cara mengingat: **AND = saringan** (bit yang di-mask 0 hilang), **OR = penambah** (bit 1 menyalakan), **XOR = pembalik** (bit 1 membalik).

### 3. Empat idiom yang dipakai terus

| Tujuan | Rumus | Contoh (v = 0x00, n = 3) |
|---|---|---|
| Nyalakan bit n | `v \| (1 << n)` | `0x00 \| 0x08 = 0x08` |
| Matikan bit n | `v & ~(1 << n)` | `0xFF & ~0x01 = 0xFE` |
| Balik bit n | `v ^ (1 << n)` | `0x0F ^ 0x10 = 0x1F` |
| Uji bit n | `(v >> n) & 1` | `(0xA5 >> 7) & 1 = 1` |

### 4. Mask dan field

**Field** adalah sekelompok bit bersebelahan yang membawa satu arti, misalnya "tipe" di bit 28..24. **Mask** untuk field selebar $n$ bit:

$$ \text{mask}(n) = (1 \ll n) - 1 $$

| Lebar | Mask | Contoh pemakaian di CyberGear |
|---:|---|---|
| 2 bit | `0x03` | mode motor (bit 23..22) |
| 5 bit | `0x1F` | tipe komunikasi (bit 28..24) |
| 6 bit | `0x3F` | info fault (bit 21..16) |
| 8 bit | `0xFF` | satu byte: ID motor / host |
| 16 bit | `0xFFFF` | data area 2 |

**Mengambil field** (geser dulu ke posisi 0, baru buang sisanya):

```c
field = (v >> shift) & mask(width);
```

**Menulis field** (kosongkan bagian lama, lalu isi yang baru):

```c
v = (v & ~(mask(width) << shift)) | ((new_value & mask(width)) << shift);
```

**Contoh hitungan** dari ID feedback `0x028422FD`:

| Field | Perhitungan | Hasil |
|---|---|---|
| tipe (28..24) | `(0x028422FD >> 24) & 0x1F` | 2 |
| mode (23..22) | `(0x028422FD >> 22) & 0x03` | 2 |
| fault (21..16) | `(0x028422FD >> 16) & 0x3F` | 4 |
| motor (15..8) | `(0x028422FD >> 8) & 0xFF` | 0x22 = 34 |

### 5. Tiga jebakan klasik

> [!WARNING]
> **Jebakan 1: urutan operator.** Di C, `==` lebih kuat daripada `&`. Maka `if (x & 0x0F == 0)` dibaca `x & (0x0F == 0)`, hampir pasti bukan maksudmu. Selalu beri kurung: `if ((x & 0x0F) == 0)`.
>
> **Jebakan 2: geser 32 bit pada tipe 32 bit.** `1u << 32` itu *undefined behavior*. Tangani lebar 32 secara khusus pada `mask_bits`.
>
> **Jebakan 3: promosi ke int.** `~(uint8_t)0x0F` bukan `0xF0`; byte dipromosikan ke `int` dulu sehingga hasilnya `0xFFFFFFF0`. Gunakan `(uint8_t)~0x0F`, atau mask dengan `& 0xFF`.

---

## 🧪 Latihan

### 1A. Di kertas (tanpa komputer)

Isi tabel. Lalu cek hasilnya di kalkulator programmer atau Python (`bin()`, `hex()`).

| Soal | Jawabanmu |
|---|---|
| `0xB6` dalam biner | |
| `1100 1010` dalam hex | |
| `0xA5 >> 4` | |
| `0xA5 & 0x0F` | |
| `0x0F \| 0xF0` | |
| `0xFF ^ 0x0F` | |
| nyalakan bit 5 pada `0x00` | |
| matikan bit 7 pada `0xFF` | |

### 1B. Empat operasi satu bit

Di `exercises/level-01-bitwise/bits.c`, isi `bit_set`, `bit_clear`, `bit_toggle`, `bit_test` memakai empat idiom di atas.

```bash
cd exercises && make test-01
```

Targetnya, baris-baris `bit_...` berubah menjadi `PASS`.

<details>
<summary>▶ Petunjuk 1: <code>bit_clear</code></summary>

Untuk mematikan bit n, kamu butuh mask yang bernilai 1 di semua posisi **kecuali** bit n, lalu di-AND. Mask itu adalah kebalikan (`~`) dari `1u << n`.

</details>

<details>
<summary>▶ Petunjuk 2: <code>bit_test</code> mengembalikan <code>bool</code></summary>

`(v >> n) & 1u` menghasilkan 0 atau 1. Tipe `bool` dari `<stdbool.h>` menerimanya langsung.

</details>

### 1C. Mask dan field

Isi `mask_bits`, `get_field`, `set_field`. Perhatikan kasus lebar 32 (jebakan 2) dan kasus field yang kebesaran (harus dimask).

<details>
<summary>▶ Petunjuk: <code>set_field</code> dalam tiga langkah</summary>

1. `m = mask_bits(width)`
2. kosongkan: `v & ~(m << shift)`
3. isi: `| ((field & m) << shift)`

</details>

### 1D. Bit menjadi teks

Isi `to_binary_string(v, nbits, out)` yang menulis `"10100101"` untuk `0xA5` dengan `nbits = 8`. Ini alat debug yang akan kamu pakai terus untuk melihat ID dan flag fault.

<details>
<summary>▶ Petunjuk</summary>

Karakter ke-i (mulai 0) adalah bit nomor `nbits - 1 - i`. Jangan lupa `out[nbits] = '\0'`.

</details>

---

## ✅ Checkpoint: lulus jika

- [ ] `make test-01` menampilkan `SEMUA LOLOS`
- [ ] Tabel 1A benar semua
- [ ] Kamu bisa menjelaskan kenapa `1u << 32` berbahaya

## ❓ Cek pemahaman

**1.** Tulis dua baris C untuk mengambil bit 21..16 dari `id`.

**2.** `if (x & 0x04 == 0)`: apa yang salah?

**3.** Berapa mask untuk field selebar 3 bit? Untuk 10 bit?

**4.** `v = 0xA5`. Berapa hasil `v ^ 0xFF`? Apa artinya secara bit?

<details>
<summary>▶ Kunci jawaban (tulis jawabanmu dulu)</summary>

1. `uint8_t fault = (id >> 16) & 0x3F;` (satu baris cukup; hasilnya 6 bit).
2. `==` dievaluasi lebih dulu, jadi dibaca `x & (0x04 == 0)` = `x & 0`. Seharusnya `(x & 0x04) == 0`.
3. $2^3 - 1 = 7$ (`0x07`) dan $2^{10} - 1 = 1023$ (`0x3FF`).
4. `0xA5 ^ 0xFF = 0x5A`. XOR dengan 1 membalik bit, jadi seluruh bit terbalik.

</details>

---

[← Level 0](00-peta-tugas.md) · [Beranda](../README.md) · [Level 2: Extended ID →](02-extended-id.md)
