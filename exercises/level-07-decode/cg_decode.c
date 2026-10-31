#include "cg_decode.h"
#include "cg_id.h"
#include "payload.h"
#include "scaling.h"
#include <string.h>

bool cg_decode_feedback(const CgFrame *f, CgFeedback *out)
{
    /* TODO:
     *  1) tolak jika f atau out NULL, tipe ID bukan CG_T_FEEDBACK, atau dlc < 8
     *  2) dari ext_id: motor_id, host_id (target), mode, fault_bits   (pakai fungsi Level 2)
     *  3) dari data: byte 0-1 posisi, 2-3 kecepatan, 4-5 torsi (semuanya big-endian -> uint_to_float)
     *     byte 6-7 suhu mentah (big-endian) -> dibagi 10.0f
     */
    (void)f; (void)out;
    return false;
}

CgFaultBits cg_decode_fault_bits(uint8_t bits)
{
    CgFaultBits r;
    memset(&r, 0, sizeof r);
    /* TODO: bit 0 -> under_voltage, bit 1 -> over_current, bit 2 -> over_temp,
     *       bit 3 -> magnetic_fault, bit 4 -> hall_fault, bit 5 -> not_calibrated */
    (void)bits;
    return r;
}

const char *cg_mode_name(uint8_t mode)
{
    /* TODO: 0 "RESET", 1 "CALI", 2 "RUN", selain itu "UNKNOWN" */
    (void)mode;
    return "";
}

bool cg_decode_fault_report(const CgFrame *f, CgFaultReport *out)
{
    /* TODO: tipe harus 0x15 dan dlc >= 8.
     * fault_value   = uint32 little-endian dari data[0..3]
     * warning_value = uint32 little-endian dari data[4..7]
     * CATATAN: manual tidak menyebut urutan byte; kita ASUMSIKAN little-endian. Verifikasi di motor nyata. */
    (void)f; (void)out;
    return false;
}

bool cg_decode_device_id(const CgFrame *f, uint8_t *motor_id, uint8_t uid[8])
{
    /* TODO: tipe harus 0 dan byte target (bit 7..0) harus 0xFE; dlc >= 8.
     * motor_id = bit 15..8 dari ID; uid = salin 8 byte data apa adanya. */
    (void)f; (void)motor_id; (void)uid;
    return false;
}
