#include <stdio.h>
#include <string.h>
#include "cg_cmd.h"

static int failures = 0;
#define CHECK(cond, name) do { if (cond) printf("PASS  %s\n", name); else { printf("FAIL  %s\n", name); failures++; } } while (0)

static int is(const CgFrame *f, uint32_t id, const uint8_t d[8])
{
    return f->ext_id == id && f->dlc == 8 && memcmp(f->data, d, 8) == 0;
}

int main(void)
{
    CgFrame f;
    const uint8_t zero[8] = {0};

    memset(&f, 0xEE, sizeof f); cg_build_get_device_id(&f, 253, 34);
    CHECK(is(&f, 0x0000FD22u, zero),                         "get_device_id(253, 34)  -> 0x0000FD22, data 0");
    memset(&f, 0xEE, sizeof f); cg_build_enable(&f, 253, 34);
    CHECK(is(&f, 0x0300FD22u, zero),                         "enable(253, 34)         -> 0x0300FD22, data 0");
    memset(&f, 0xEE, sizeof f); cg_build_disable(&f, 253, 34);
    CHECK(is(&f, 0x0400FD22u, zero),                         "disable(253, 34)        -> 0x0400FD22, data 0");

    const uint8_t one[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };
    memset(&f, 0xEE, sizeof f); cg_build_clear_fault(&f, 253, 34);
    CHECK(is(&f, 0x0400FD22u, one),                          "clear_fault(253, 34)    -> 0x0400FD22, data[0] = 1");
    memset(&f, 0xEE, sizeof f); cg_build_set_zero(&f, 253, 34);
    CHECK(is(&f, 0x0600FD22u, one),                          "set_zero(253, 34)       -> 0x0600FD22, data[0] = 1");
    memset(&f, 0xEE, sizeof f); cg_build_set_can_id(&f, 253, 34, 35);
    CHECK(is(&f, 0x0723FD22u, one),                          "set_can_id(253, 34, 35) -> 0x0723FD22, data[0] = 1");

    /* Kontrol operasi (MIT) */
    const uint8_t m1[8] = { 0x7F, 0xFF, 0x7F, 0xFF, 0x00, 0x83, 0x00, 0x00 };
    memset(&f, 0xEE, sizeof f); cg_build_motion(&f, 34, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    CHECK(is(&f, 0x017FFF22u, m1),                           "motion(34, 0,0,0, kp=1, kd=0) -> 0x017FFF22, 7F FF 7F FF 00 83 00 00");
    const uint8_t m2[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    memset(&f, 0xEE, sizeof f); cg_build_motion(&f, 34, 100.0f, 100.0f, 100.0f, 9999.0f, 9999.0f);
    CHECK(is(&f, 0x01FFFF22u, m2),                           "motion nilai kebesaran di-clamp ke FFFF semua");
    const uint8_t m3[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    memset(&f, 0xEE, sizeof f); cg_build_motion(&f, 34, -100.0f, -100.0f, -100.0f, -5.0f, -5.0f);
    CHECK(is(&f, 0x01000022u, m3),                           "motion nilai terlalu kecil di-clamp ke 0000 semua");

    /* Parameter */
    const uint8_t pr[8] = { 0x05, 0x70, 0, 0, 0, 0, 0, 0 };
    memset(&f, 0xEE, sizeof f); cg_build_param_read(&f, 253, 34, CG_PARAM_RUN_MODE);
    CHECK(is(&f, 0x1100FD22u, pr),                           "param_read(0x7005)      -> 0x1100FD22, 05 70 00 ...");
    const uint8_t pw1[8] = { 0x05, 0x70, 0, 0, 0x02, 0, 0, 0 };
    memset(&f, 0xEE, sizeof f); cg_build_param_write_u8(&f, 253, 34, CG_PARAM_RUN_MODE, 2);
    CHECK(is(&f, 0x1200FD22u, pw1),                          "param_write_u8(run_mode, 2) -> 0x1200FD22, 05 70 00 00 02 ...");
    const uint8_t pw2[8] = { 0x0A, 0x70, 0, 0, 0x00, 0x00, 0xA0, 0x40 };
    memset(&f, 0xEE, sizeof f); cg_build_param_write_f32(&f, 253, 34, CG_PARAM_SPD_REF, 5.0f);
    CHECK(is(&f, 0x1200FD22u, pw2),                          "param_write_f32(spd_ref, 5.0) -> 0A 70 00 00 00 00 A0 40");
    const uint8_t pw3[8] = { 0x05, 0x70, 0, 0, 0x03, 0, 0, 0 };
    memset(&f, 0xEE, sizeof f); cg_build_set_run_mode(&f, 253, 34, CG_RUN_MODE_CURRENT);
    CHECK(is(&f, 0x1200FD22u, pw3),                          "set_run_mode(current=3) = param_write_u8(0x7005, 3)");

    printf("\n%s (%d gagal)\n", failures == 0 ? "SEMUA LOLOS" : "ADA YANG GAGAL", failures);
    return failures != 0;
}
