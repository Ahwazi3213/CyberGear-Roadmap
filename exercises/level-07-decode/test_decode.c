#include <stdio.h>
#include <string.h>
#include <math.h>
#include "cg_decode.h"

static int failures = 0;
#define CHECK(cond, name) do { if (cond) printf("PASS  %s\n", name); else { printf("FAIL  %s\n", name); failures++; } } while (0)
#define NEAR(a, b, tol) (fabsf((a) - (b)) <= (tol))

int main(void)
{
    /* Feedback: motor 34, host 253, mode RUN, fault = over temperature,
     * posisi raw 0x8000, kecepatan raw 0xFFFF, torsi raw 0x0000, suhu raw 355 (35,5 C) */
    CgFrame fb = { 0x028422FDu, 8, { 0x80, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x63 } };
    CgFeedback r;
    memset(&r, 0, sizeof r);

    CHECK(cg_decode_feedback(&fb, &r),          "feedback valid didecode");
    CHECK(r.motor_id == 34 && r.host_id == 253, "motor_id = 34, host_id = 253");
    CHECK(r.mode == 2,                          "mode = 2 (RUN)");
    CHECK(r.fault_bits == 4,                    "fault_bits = 0b000100");
    CHECK(NEAR(r.position, 0.000192f, 1e-3f),   "posisi sekitar 0 rad");
    CHECK(NEAR(r.velocity, 30.0f, 1e-2f),       "kecepatan = +30 rad/s");
    CHECK(NEAR(r.torque, -12.0f, 1e-2f),        "torsi = -12 Nm");
    CHECK(NEAR(r.temperature, 35.5f, 0.01f),    "suhu = 35,5 C (raw 355 / 10)");

    CgFrame wrong = fb; wrong.ext_id = 0x03000000u | 0xFD22u;
    CHECK(!cg_decode_feedback(&wrong, &r),      "tipe bukan 2 ditolak");
    CgFrame shortf = fb; shortf.dlc = 7;
    CHECK(!cg_decode_feedback(&shortf, &r),     "dlc < 8 ditolak");
    CHECK(!cg_decode_feedback(NULL, &r) && !cg_decode_feedback(&fb, NULL), "pointer NULL ditolak");

    CgFaultBits b = cg_decode_fault_bits(0x04);
    CHECK(b.over_temp && !b.under_voltage && !b.over_current && !b.magnetic_fault && !b.hall_fault && !b.not_calibrated, "bits 0x04: hanya over_temp");
    b = cg_decode_fault_bits(0x21);
    CHECK(b.not_calibrated && b.under_voltage && !b.over_temp, "bits 0x21: not_calibrated + under_voltage");
    b = cg_decode_fault_bits(0x3F);
    CHECK(b.under_voltage && b.over_current && b.over_temp && b.magnetic_fault && b.hall_fault && b.not_calibrated, "bits 0x3F: semua aktif");
    b = cg_decode_fault_bits(0x00);
    CHECK(!b.under_voltage && !b.over_current && !b.over_temp && !b.magnetic_fault && !b.hall_fault && !b.not_calibrated, "bits 0x00: tidak ada fault");

    CHECK(strcmp(cg_mode_name(0), "RESET") == 0 && strcmp(cg_mode_name(1), "CALI") == 0 &&
          strcmp(cg_mode_name(2), "RUN") == 0 && strcmp(cg_mode_name(3), "UNKNOWN") == 0, "nama mode");

    /* Frame fault tipe 21 */
    CgFrame ff = { (0x15u << 24) | (253u << 8) | 34u, 8, { 0x01, 0, 0, 0, 0x01, 0, 0, 0 } };
    CgFaultReport fr;
    CHECK(cg_decode_fault_report(&ff, &fr) && fr.fault_value == CG_FLT_OVER_TEMP && fr.warning_value == CG_WARN_OVER_TEMP, "fault report: over-temp + warning");
    ff.data[0] = 0x00; ff.data[1] = 0x01; ff.data[4] = 0;
    CHECK(cg_decode_fault_report(&ff, &fr) && fr.fault_value == 0x100 && fr.warning_value == 0, "fault report: overload (byte 1) = 0x100");
    ff.data[1] = 0x00; ff.data[2] = 0x01;
    CHECK(cg_decode_fault_report(&ff, &fr) && fr.fault_value == CG_FLT_A_OVERCURRENT, "fault report: A-phase overcurrent = bit 16");
    CgFrame ffw = ff; ffw.ext_id = (0x02u << 24);
    CHECK(!cg_decode_fault_report(&ffw, &fr),   "fault report: tipe lain ditolak");

    /* Balasan Get Device ID */
    CgFrame dv = { 0x000022FEu, 8, { 1, 2, 3, 4, 5, 6, 7, 8 } };
    uint8_t mid = 0, uid[8] = {0};
    CHECK(cg_decode_device_id(&dv, &mid, uid) && mid == 34 && memcmp(uid, dv.data, 8) == 0, "device id: motor 34 + UID disalin");
    CgFrame dv2 = dv; dv2.ext_id = 0x000022FDu;
    CHECK(!cg_decode_device_id(&dv2, &mid, uid), "device id: target bukan 0xFE ditolak");
    CgFrame dv3 = dv; dv3.ext_id = 0x020022FEu;
    CHECK(!cg_decode_device_id(&dv3, &mid, uid), "device id: tipe bukan 0 ditolak");

    printf("\n%s (%d gagal)\n", failures == 0 ? "SEMUA LOLOS" : "ADA YANG GAGAL", failures);
    return failures != 0;
}
