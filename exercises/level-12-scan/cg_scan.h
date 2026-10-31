#ifndef CG_SCAN_H
#define CG_SCAN_H
#include "cg_defs.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Lapisan transport: apa pun yang bisa mengirim dan menerima frame
 * (di STM32: HAL CAN; di laptop: bus tiruan/mock). */
typedef struct {
    bool     (*send)  (void *ctx, const CgFrame *f);   /* true jika frame diterima antrean kirim          */
    bool     (*recv)  (void *ctx, CgFrame *f);         /* NON-blocking: true jika ada frame yang bisa diambil */
    uint32_t (*now_ms)(void *ctx);                     /* jam dalam ms (di STM32: HAL_GetTick)           */
    void     *ctx;
} CgTransport;

/* Cari motor: kirim Get Device ID ke setiap ID pada rentang [first, last] (inklusif),
 * tunggu balasan sampai timeout_ms per ID, simpan ID yang menjawab ke found[] (tanpa duplikat).
 * Mengembalikan jumlah motor yang ditemukan (maksimal max_found). */
uint8_t cg_scan(const CgTransport *t, uint8_t host, uint8_t first, uint8_t last,
                uint32_t timeout_ms, uint8_t *found, uint8_t max_found);

#ifdef __cplusplus
}
#endif
#endif
