#include <stdio.h>
#include <string.h>
#include "bits.h"

static int failures = 0;
#define CHECK(cond, name) do { if (cond) printf("PASS  %s\n", name); else { printf("FAIL  %s\n", name); failures++; } } while (0)

int main(void)
{
    CHECK(bit_set(0x00, 3) == 0x08,            "bit_set(0, 3) = 0x08");
    CHECK(bit_set(0x08, 3) == 0x08,            "bit_set pada bit yang sudah 1 tidak berubah");
    CHECK(bit_clear(0xFF, 0) == 0xFE,          "bit_clear(0xFF, 0) = 0xFE");
    CHECK(bit_toggle(0x0F, 4) == 0x1F,         "bit_toggle(0x0F, 4) = 0x1F");
    CHECK(bit_toggle(0x1F, 4) == 0x0F,         "bit_toggle dua kali kembali semula");
    CHECK(bit_test(0xA5, 0) && !bit_test(0xA5, 1) && bit_test(0xA5, 7), "bit_test(0xA5): bit0=1 bit1=0 bit7=1");
    CHECK(bit_set(0, 31) == 0x80000000u,       "bit_set(0, 31) = 0x80000000");

    CHECK(mask_bits(2) == 0x03,                "mask_bits(2) = 0x03");
    CHECK(mask_bits(5) == 0x1F,                "mask_bits(5) = 0x1F");
    CHECK(mask_bits(6) == 0x3F,                "mask_bits(6) = 0x3F");
    CHECK(mask_bits(8) == 0xFF,                "mask_bits(8) = 0xFF");
    CHECK(mask_bits(16) == 0xFFFF,             "mask_bits(16) = 0xFFFF");
    CHECK(mask_bits(32) == 0xFFFFFFFFu,        "mask_bits(32) = 0xFFFFFFFF (jebakan geser 32)");
    CHECK(mask_bits(0) == 0,                   "mask_bits(0) = 0");

    /* Frame feedback contoh: tipe 2, mode 2, fault 0b000100, motor 34, host 253 */
    uint32_t id = 0x028422FDu;
    CHECK(get_field(id, 24, 5) == 2,           "tipe   = bit 28..24 = 2");
    CHECK(get_field(id, 22, 2) == 2,           "mode   = bit 23..22 = 2");
    CHECK(get_field(id, 16, 6) == 4,           "fault  = bit 21..16 = 4");
    CHECK(get_field(id,  8, 8) == 34,          "motor  = bit 15..8  = 34");
    CHECK(get_field(id,  0, 8) == 253,         "host   = bit 7..0   = 253");

    CHECK(set_field(0, 8, 8, 0x22) == 0x2200,                 "set_field(0, 8, 8, 0x22) = 0x2200");
    CHECK(set_field(0xFFFFFFFFu, 8, 8, 0) == 0xFFFF00FFu,     "set_field menghapus field lama dulu");
    CHECK(set_field(0, 0, 4, 0x1F) == 0x0F,                   "field yang kebesaran dimask (0x1F -> 0x0F)");
    CHECK(set_field(0x12345678u, 0, 32, 0xAABBCCDDu) == 0xAABBCCDDu, "set_field lebar 32");

    char buf[40];
    to_binary_string(0xA5, 8, buf);
    CHECK(strcmp(buf, "10100101") == 0,        "to_binary_string(0xA5, 8) = \"10100101\"");
    to_binary_string(0x5, 4, buf);
    CHECK(strcmp(buf, "0101") == 0,            "to_binary_string(0x5, 4) = \"0101\" (nol di depan)");

    printf("\n%s (%d gagal)\n", failures == 0 ? "SEMUA LOLOS" : "ADA YANG GAGAL", failures);
    return failures != 0;
}
