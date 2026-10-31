#include <stdio.h>
#include "cg_id.h"

static int failures = 0;
#define CHECK(cond, name) do { if (cond) printf("PASS  %s\n", name); else { printf("FAIL  %s\n", name); failures++; } } while (0)

int main(void)
{
    /* Enable motor 34 dari host 253: tipe 3, data2 = host */
    CHECK(cg_id_pack(3, 253, 34) == 0x0300FD22u,      "pack(3, 253, 34) = 0x0300FD22");
    CHECK(cg_id_pack(0, 0xFD, 34) == 0x0000FD22u,     "pack tipe 0 (Get Device ID) = 0x0000FD22");
    CHECK(cg_id_pack(1, 0xFFFF, 0x01) == 0x01FFFF01u, "pack data2 penuh 16 bit");
    CHECK(cg_id_pack(0x12, 253, 34) == 0x1200FD22u,   "pack tipe 0x12 (param write) = 0x1200FD22");
    CHECK(cg_id_pack(0x23, 0, 0) == 0x03000000u,      "tipe dimask 5 bit: 0x23 -> 0x03");
    CHECK((cg_id_pack(0xFF, 0xFFFF, 0xFF) >> 29) == 0, "hasil pack tidak pernah melebihi 29 bit");

    uint32_t fb = 0x028422FDu;   /* feedback: tipe 2, mode 2, fault 4, motor 34, host 253 */
    CHECK(cg_id_type(fb)   == 2,        "type  (0x028422FD) = 2");
    CHECK(cg_id_data2(fb)  == 0x8422,   "data2 (0x028422FD) = 0x8422");
    CHECK(cg_id_target(fb) == 0xFD,     "target(0x028422FD) = 0xFD");
    CHECK(cg_id_mode(fb)   == 2,        "mode  (0x028422FD) = 2");
    CHECK(cg_id_fault(fb)  == 4,        "fault (0x028422FD) = 4");
    CHECK(cg_id_motor(fb)  == 34,       "motor (0x028422FD) = 34");

    uint32_t dev = 0x000022FEu;  /* balasan Get Device ID dari motor 34 */
    CHECK(cg_id_type(dev) == 0 && cg_id_motor(dev) == 34 && cg_id_target(dev) == 0xFE, "balasan Device ID: tipe 0, motor 34, target 0xFE");

    /* pack lalu unpack harus kembali */
    int ok = 1;
    for (uint32_t t = 0; t < 32; t += 7)
        for (uint32_t d = 0; d < 65536; d += 4099)
            for (uint32_t a = 0; a < 256; a += 51) {
                uint32_t id = cg_id_pack((uint8_t)t, (uint16_t)d, (uint8_t)a);
                if (cg_id_type(id) != t || cg_id_data2(id) != d || cg_id_target(id) != a) ok = 0;
            }
    CHECK(ok, "pack lalu unpack mengembalikan nilai asal");

    printf("\n%s (%d gagal)\n", failures == 0 ? "SEMUA LOLOS" : "ADA YANG GAGAL", failures);
    return failures != 0;
}
