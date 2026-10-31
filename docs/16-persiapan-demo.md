# Level 16: Persiapan Demo

[← Level 15](15-baremetal.md) · [Beranda](../README.md)

> [!NOTE]
> Tujuan level ini: berlatih menghadapi **lima tantangan demo** dan pertanyaan penguji. Kamu sudah membangun semuanya; sekarang buktikan bahwa kamu bisa **menjelaskan dan mempertanggungjawabkannya**.

> [!IMPORTANT]
> Dokumen penugasan menyatakan: *"AI tidak dilarang. Semua yang kamu kerjakan harus bisa dipertanggungjawabkan."* Ukuran kesiapanmu sederhana: **bisa kamu jelaskan tiap baris tanpa melihat kode, dan bisa kamu ubah di tempat bila diminta?** Kalau tidak, kembali ke levelnya.

## Tujuan belajar

- [ ] Menyiapkan bukti untuk tiap tantangan demo
- [ ] Menjalankan demo 10 menit dengan lancar
- [ ] Menjawab pertanyaan teori dan kode dengan kata-kata sendiri
- [ ] Menangani kegagalan di tempat

---

## 1. Peta tantangan demo → kesiapan

| # | Tantangan | Bukti yang kamu siapkan | Level |
|--:|---|---|---|
| 1 | Jelaskan kode (termasuk library) | diagram lapisan, tabel fungsi, `make test-XX` lolos | 0, 9 |
| 2 | Jelaskan tambahan dan perubahan beserta alasan | `docs/changelog.md` terisi | 9D, 10, 11 |
| 3 | Test case dari penguji | *test runner* + latihan 13G | 13 |
| 4 | Dua motor dalam satu bus | kedua `online`, kontrol independen | 12 |
| 5 | Temukan CAN ID motor tak dikenal | `cybergear_scan`, waktu < 1 detik | 12 |
| + | OOP (plus) | `make test-14` + versi class berjalan | 14 |
| + | Baremetal (A+) | `make test-15` + loopback tanpa HAL | 15 |

## 2. Skrip demo 10 menit

| Menit | Kegiatan | Yang kamu katakan |
|--:|---|---|
| 0:00 | Tunjukkan rangkaian dan diagram | "STM32 → transceiver → bus 1 Mbps terminasi 120 Ω → dua motor." |
| 1:00 | Jalankan `make test-all` di laptop | "Logika protokol saya uji otomatis di laptop sebelum hardware." |
| 2:00 | Scan ID di bus | "Get Device ID ke tiap ID 0 sampai 127; balasan tipe 0 dengan target 0xFE." |
| 3:30 | Enable, baca suhu/mode/fault | "Suhu byte 6–7 dibagi 10; fault 6 bit di ID feedback." |
| 5:00 | Kontrol dua motor | "ID membedakan; tiap feedback dipilah berdasarkan ID motor." |
| 7:00 | Test case dari penguji | pakai *test runner* |
| 9:00 | Ringkas perubahan dan plus | "Saya menambah ..., memperbaiki ..., karena ..." |

> [!TIP]
> Latih skrip ini **tiga kali dengan stopwatch**, dan sekali dengan temanmu yang sengaja menyela dengan pertanyaan.

## 3. Pertanyaan teori

| Pertanyaan | Jawaban singkat |
|---|---|
| Kenapa CyberGear memakai Extended frame? | ID 29 bit memuat tipe, data area (mis. torsi), dan ID motor sekaligus. |
| Jelaskan `0x0300FD22`. | Tipe 3 (enable), host `0xFD` = 253 di bit 15..8, motor `0x22` = 34 di bit 7..0. |
| Kenapa tipe diletakkan di bit teratas? | Arbitrasi membandingkan dari bit tertinggi; ID kecil menang, jadi tipe bernomor kecil lebih prioritas. |
| Kenapa pembaginya 65535 pada skala float? | Agar min → 0 dan max → 65535 tepat di kedua ujung (65535 celah dari 65536 nilai). |
| Kenapa nilai di-clamp? | Float di luar rentang menjadi nilai tak terdefinisi saat dikonversi ke unsigned; clamp menahannya di batas. |
| Berapa resolusi torsi? | $24/65535 \approx 0{,}000366$ Nm. |
| Kenapa ada big-endian dan little-endian sekaligus? | Frame kontrol/feedback 16 bit big-endian; index dan nilai parameter little-endian (disalin dengan `memcpy` di MCU little-endian). |
| Bagaimana suhu didecode? | Byte 6–7 big-endian, `raw / 10.0f` dalam °C. |
| Apa itu fault 6 bit dan laporan tipe 21? | 6 bit di ID feedback = ringkasan (under-voltage, over-current, over-temp, magnetic, hall, not calibrated); tipe 21 = nilai 32 bit rinci + warning. |
| Mana yang belum terverifikasi? | Urutan byte dan penempatan ID pada frame fault tipe 21 (asumsi little-endian; diuji dengan injeksi frame). |
| Kenapa 1 Mbps dan bagaimana hitungnya? | $36\,\text{MHz}/(4 \times 9) = 1$ Mbps; titik sampel 77,8%. |
| Kenapa terminasi 120 Ω di dua ujung? | Mencocokkan impedansi kabel agar sinyal tidak memantul; diukur ≈ 60 Ω dari CANH ke CANL. |
| Apa itu ACK error? | Tidak ada node yang mengakui frame (motor mati, kabel, baud); pengirim mengulang dan TEC naik. |

## 4. Pertanyaan kode dan desain

| Pertanyaan | Jawaban singkat |
|---|---|
| Apa yang kamu tambahkan ke library, dan kenapa? | Fungsi yang hilang (`set_zero` yang hanya dideklarasikan, Get Device ID, Clear Fault, Set CAN ID, parameter read/write, mode), decode suhu/mode/fault, dispatcher per tipe, perbaikan bug enable/disable. Alasan tiap baris ada di changelog. |
| Kenapa `rx_handler` memakai `switch` per tipe? | Balasan Device ID dan laporan fault sebelumnya dibuang karena handler hanya menerima feedback dan memeriksa host ID. |
| Kenapa pemeriksaan host dipindah? | Balasan Device ID bertarget `0xFE`, bukan ID host. |
| Bagaimana scan ID bekerja? | Kirim Get Device ID ke setiap ID, tunggu 5 ms, catat balasan tipe 0 dengan target `0xFE`; abaikan frame lain. |
| Kenapa loop scan memakai `uint16_t`? | Dengan `uint8_t`, `id <= 255` selalu benar (255 + 1 = 0) sehingga tidak berhenti. |
| Bagaimana dua motor tidak bentrok? | Tiap motor ber-ID unik; frame berisi ID tujuan; feedback dipilah per ID motor. |
| Apa batas `HAL_CAN_AddTxMessage`? | Hanya 3 mailbox kirim; pengiriman beruntun harus memeriksa `GetTxMailboxesFreeLevel` atau hasilnya. |
| Kenapa fault tidak diuji di motor? | Berbahaya; saya menyuntikkan frame palsu lewat loopback untuk membuktikan decode. |
| OOP: kenapa abstraksi transport? | Kelas motor tidak peduli HAL atau mock; jadi diuji di laptop dan diganti tanpa mengubah logika. |
| OOP: apa biayanya? | Satu pointer vtable per objek dan satu lompatan tidak langsung: tidak terasa pada 100 Hz. |
| Baremetal: urutan init bxCAN? | Keluar sleep, masuk INRQ, atur NART dan BTR, filter, keluar INRQ. |
| Baremetal: kenapa `RFOM0`? | Melepas pesan dari FIFO; tanpa itu FIFO tak maju. |

## 5. Pertanyaan jebakan

| Situasi | Jawaban yang baik |
|---|---|
| "Apakah kodemu ditulis AI?" | Jawab jujur dan konkret: bagian mana kamu tulis sendiri, bagian mana dibantu, dan **bagaimana kamu memverifikasinya** (uji otomatis, injeksi frame, uji motor). Lalu jelaskan satu bagian detail tanpa melihat kode. |
| "Kenapa posisi terukur beda 0,5%?" | Tabel protokol memakai ±4π, contoh kode manual memakai ±12,5; saya memakai tabel dan memverifikasinya. |
| "Bagaimana jika motor mati di tengah jalan?" | `last_rx_ms` tidak maju; tandai `online = false` setelah timeout dan hentikan perintah gerak. |
| "Bagaimana jika dua motor ber-ID sama?" | Balasan bertabrakan; ID harus diubah satu per satu dengan Set CAN ID (satu motor di bus). |
| "Apa kelemahan solusimu?" | Scan memblokir ±0,6 detik; belum ada pemulihan bus-off otomatis; fault tipe 21 berbasis asumsi. Rencana perbaikan: state machine non-blocking, `AutoBusOff`/pemulihan, verifikasi dengan frame nyata. |

## 6. Jika gagal saat demo

| Gejala | Periksa berurutan |
|---|---|
| Motor tidak merespons | LED merah (daya)? tegangan 24 V? CANH/CANL tertukar? `hcan.Instance->ESR` LEC = 3? |
| Scan tidak menemukan | `rx_count` naik? `handle_device_id` tidak dibuang host check? perlebar ke 0..255 |
| Suhu aneh | pembagian `10.0f`? urutan byte? |
| Dua motor kacau | ID unik? terminasi ≈ 60 Ω? stub terlalu panjang? |
| Frame terkirim tapi tak ada efek | urutan byte parameter (little-endian)? urutan mode (run_mode sebelum enable)? |
| Build gagal | file `.cpp` di project C++? `extern "C"`? duplikat ISR dari HAL? |

> [!CAUTION]
> Jika ada yang tidak biasa pada motor (bau, panas, bunyi aneh): **putus daya segera**, baru analisis.

## 7. Paket penyerahan

- [ ] Repositori bersih (tanpa `Debug/`), `README.md` P5 yang menjelaskan arsitektur dan cara menjalankan
- [ ] `docs/changelog.md`: apa, kenapa, bagaimana diuji
- [ ] Hasil `make test-all` dilampirkan (tangkapan layar atau teks)
- [ ] Tag Git (`p5-final`)
- [ ] Daftar asumsi dan batasan yang kamu tahu

## 8. Rubrik penilaian diri

Beri nilai 0 (tidak bisa), 1 (dengan melihat catatan), 2 (lancar tanpa catatan).

| Topik | 0/1/2 |
|---|:-:|
| Menyusun dan membongkar Extended ID | |
| Skala float ↔ 16 bit | |
| Endianness di CyberGear | |
| Decode feedback (suhu, mode, fault) | |
| Urutan komunikasi dan mode kontrol | |
| Scan ID dan jebakannya | |
| Wiring, terminasi, dan keselamatan | |
| Arsitektur library dan changelog | |
| OOP (plus) | |
| Baremetal (plus) | |

Total ≥ 16 dari 20 (tanpa plus) berarti siap. Skor 0 atau 1 pada satu topik: ulangi levelnya.

## 9. Latihan terakhir

1. Minta teman membacakan **ID acak** (mis. `0x0723FD7F`); kamu menjelaskannya secara lisan dalam 10 detik.
2. Minta teman memilih satu fungsi library secara acak; jelaskan baris demi baris.
3. Teman mengubah satu angka di kodemu (mis. `/ 10.0f` → `/ 10`); kamu menemukan dampaknya.
4. Teman mencabut kabel CANH saat motor aktif; kamu menjelaskan apa yang terjadi dan memulihkannya.

---

[← Level 15](15-baremetal.md) · [Beranda](../README.md)
