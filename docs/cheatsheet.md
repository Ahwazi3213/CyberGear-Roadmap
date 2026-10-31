# Cheatsheet P5

[← Beranda](../README.md)

Rujukan cepat. Semua nilai contoh memakai **host = 253 (`0xFD`)**, **motor = 34 (`0x22`)**.

## 1. Peta Extended ID

| Tipe | Nama | Bit 23..8 (data area 2) | Bit 7..0 | Contoh ExtId |
|---:|---|---|---|---|
| `0x00` | Get Device ID | host | motor | `0x0000FD22` |
| `0x01` | Kontrol (MIT) | torsi 16 bit | motor | `0x017FFF22` |
| `0x02` | Feedback | mode 23..22, fault 21..16, motor 15..8 | host | `0x028422FD` |
| `0x03` | Enable | host | motor | `0x0300FD22` |
| `0x04` | Disable / Clear Fault | host | motor | `0x0400FD22` |
| `0x06` | Set Zero | host | motor | `0x0600FD22` |
| `0x07` | Set CAN ID | ID baru 23..16, host 15..8 | motor lama | `0x0723FD22` |
| `0x11` | Param Read | host | motor | `0x1100FD22` |
| `0x12` | Param Write | host | motor | `0x1200FD22` |
| `0x15` | Laporan fault | (lihat manual) | | |

Balasan Get Device ID dari motor: tipe 0, motor di bit 15..8, **`0xFE`** di bit 7..0, 8 byte UID.

## 2. Data frame

| Frame | Isi (8 byte) | Urutan |
|---|---|---|
| Kontrol (tipe 1) | sudut(2) kecepatan(2) **Kp**(2) **Kd**(2) | big-endian |
| Feedback (tipe 2) | sudut(2) kecepatan(2) torsi(2) suhu×10(2) | big-endian |
| Param (0x11/0x12) | index(2, **LE**) `00 00` nilai(4, **LE**) | little-endian |
| Enable/Disable/Zero/GetID | nol (Disable=0, Clear Fault=1, Set Zero=1, Set ID=1 di `data[0]`) | |

## 3. Skala

$$ raw = \frac{(val - min) \times 65535}{max - min} \qquad val = \frac{raw \times (max - min)}{65535} + min $$

| Besaran | Min | Max | Resolusi |
|---|---:|---:|---:|
| Posisi (rad) | −4π | 4π | 0,000383 |
| Kecepatan (rad/s) | −30 | 30 | 0,000916 |
| Torsi (Nm) | −12 | 12 | 0,000366 |
| Kp | 0 | 500 | 0,00763 |
| Kd | 0 | 5 | 0,0000763 |

Suhu: `raw / 10.0f` °C (tanpa interpolasi). Contoh: 355 → 35,5 °C.

## 4. Fault

**Di ID feedback (bit 21..16):**

| Bit ID | Arti |
|:-:|---|
| 21 | belum dikalibrasi |
| 20 | encoder HALL |
| 19 | encoder magnetik |
| 18 | suhu berlebih |
| 17 | arus berlebih |
| 16 | tegangan kurang |

Mode (bit 23..22): 0 Reset, 1 Kalibrasi, 2 Run.

**Tipe 21 (`data[0..3]` fault, `data[4..7]` warning; urutan byte diasumsikan LE):** bit 0 over-temp (80 °C), 1 driver chip, 2 under-voltage, 3 over-voltage, 4 arus lebih B, 5 arus lebih C, 7 encoder belum kalibrasi, 15..8 overload, 16 arus lebih A. Warning bit 0: peringatan suhu (75 °C).

## 5. Urutan mode

| Mode | `run_mode` | Urutan |
|---|:-:|---|
| Operasi (MIT) | 0 | Enable → Kontrol (1) berulang |
| Posisi | 1 | run_mode=1 → Enable → `limit_spd` → `loc_ref` |
| Kecepatan | 2 | run_mode=2 → Enable → `limit_cur` → `spd_ref` |
| Arus | 3 | run_mode=3 → Enable → `iq_ref` |
| Stop | | tipe 4 |

Parameter: `0x7005` run_mode (u8), `0x7006` iq_ref, `0x700A` spd_ref, `0x700B` limit_torque, `0x7016` loc_ref, `0x7017` limit_spd, `0x7018` limit_cur (semua float kecuali run_mode).

## 6. Bit timing bxCAN (APB1 = 36 MHz)

$$ \text{baud} = \frac{36\,000\,000}{\text{prescaler} \times (1 + BS1 + BS2)} \qquad \text{sampel} = \frac{1 + BS1}{1 + BS1 + BS2} $$

| Baud | Prescaler | BS1 | BS2 | BTR |
|---|---:|---:|---:|---|
| 1 Mbps | 4 | 6 | 2 | `0x00150003` |
| 500 kbps | 8 | 6 | 2 | `0x00150007` |
| 1 Mbps (alt.) | 2 | 13 | 4 | `0x003C0001` |

## 7. HAL CAN

| Tujuan | Fungsi |
|---|---|
| Filter | `HAL_CAN_ConfigFilter(&hcan, &f)` (ID 0, mask 0 = terima semua) |
| Mulai | `HAL_CAN_Start(&hcan)` |
| Interrupt RX | `HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING)` |
| Kirim | `HAL_CAN_AddTxMessage(&hcan, &txHeader, data, &mailbox)` |
| Mailbox kosong | `HAL_CAN_GetTxMailboxesFreeLevel(&hcan)` (maks 3) |
| Terima | `HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxHeader, data)` |
| Callback | `HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)` |
| Error | `HAL_CAN_GetError(&hcan)`, register `hcan.Instance->ESR` |

Header kirim: `ExtId`, `IDE = CAN_ID_EXT`, `RTR = CAN_RTR_DATA`, `DLC = 8`, `TransmitGlobalTime = DISABLE`.

## 8. Register bxCAN (ringkas)

| Register | Bit penting |
|---|---|
| MCR | INRQ(0), SLEEP(1), NART(4) |
| MSR | INAK(0), SLAK(1) |
| TSR | TME0..2 = bit 26..28 |
| RF0R | FMP0 = bit 1..0, RFOM0 = bit 5 |
| IER | FMPIE0 = bit 1 |
| BTR | BRP 9..0, TS1 19..16, TS2 22..20, SJW 25..24, LBKM 30, SILM 31 (nilai-1) |
| TIR | TXRQ(0), RTR(1), IDE(2), ID di bit 31..3 |
| ESR | BOFF(2), LEC 6..4, TEC 23..16, REC 31..24 |
| FMR | FINIT(0) |

LEC: 0 ok, 1 stuff, 2 form, **3 ACK**, 4 bit recessive, 5 bit dominant, 6 CRC.

## 9. C yang sering dipakai

| Tujuan | Kode |
|---|---|
| Ambil field | `(v >> shift) & ((1u << w) - 1u)` |
| Tulis field | `(v & ~(m << s)) \| ((f & m) << s)` |
| 16 bit → 2 byte (BE) | `hi = v >> 8; lo = v & 0xFF` |
| 2 byte → 16 bit (BE) | `((uint16_t)hi << 8) \| lo` |
| Float → byte | `memcpy(&u32, &f, 4)` lalu pecah per byte |
| Pointer ke struct | `m->field` (sama dengan `(*m).field`) |
| Cek NULL | `if (m == NULL) return;` |
| Kali dulu, bagi kemudian | `a * b / c` (bukan `a / c * b`) |

## 10. C++ yang dipakai di Level 14

| Sintaks | Arti |
|---|---|
| `class X { public: ... private: ... };` | kelas dengan akses terkontrol |
| `X::X(args) : a_(a), b_(b) {}` | constructor + daftar inisialisasi |
| `virtual bool f() = 0;` | method abstrak (wajib diisi turunan) |
| `class D : public B` + `override` | pewarisan dan penimpaan |
| `T &r` | referensi (alias, bukan salinan) |
| `float f() const` | method tidak mengubah objek |
| `extern "C" { ... }` | pakai tautan gaya C (untuk HAL dan callback) |
