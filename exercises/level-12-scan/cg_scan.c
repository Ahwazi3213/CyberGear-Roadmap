#include "cg_scan.h"
#include "cg_cmd.h"
#include "cg_decode.h"

uint8_t cg_scan(const CgTransport *t, uint8_t host, uint8_t first, uint8_t last,
                uint32_t timeout_ms, uint8_t *found, uint8_t max_found)
{
    /* TODO:
     *   - JEBAKAN: jika last = 255 dan variabel loop bertipe uint8_t, "id <= last" tidak pernah salah
     *     (255 + 1 kembali ke 0) -> loop tak berujung. Pakai tipe yang lebih lebar untuk penghitung loop.
     *   - untuk setiap id: bangun frame Get Device ID (cg_build_get_device_id), kirim lewat t->send
     *   - lalu selama (now_ms - t_mulai) < timeout_ms: ambil frame dengan t->recv;
     *       gunakan cg_decode_device_id; frame yang bukan balasan Device ID DIABAIKAN (bisa ada feedback nyasar)
     *   - simpan motor_id balasan jika belum ada di found[] dan found masih punya tempat
     */
    (void)t; (void)host; (void)first; (void)last; (void)timeout_ms; (void)found; (void)max_found;
    return 0;
}
