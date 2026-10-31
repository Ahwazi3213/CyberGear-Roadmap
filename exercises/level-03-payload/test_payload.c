#include <stdio.h>
#include <string.h>
#include "payload.h"

static int failures = 0;
#define CHECK(cond, name) do { if (cond) printf("PASS  %s\n", name); else { printf("FAIL  %s\n", name); failures++; } } while (0)

int main(void)
{
    uint8_t b[8];

    memset(b, 0, sizeof b); put_u16_be(b, 0x1234);
    CHECK(b[0] == 0x12 && b[1] == 0x34,                 "put_u16_be(0x1234) -> 12 34");
    memset(b, 0, sizeof b); put_u16_le(b, 0x1234);
    CHECK(b[0] == 0x34 && b[1] == 0x12,                 "put_u16_le(0x1234) -> 34 12");
    CHECK(get_u16_be((uint8_t[]){0x12, 0x34}) == 0x1234, "get_u16_be(12 34) = 0x1234");
    CHECK(get_u16_le((uint8_t[]){0x34, 0x12}) == 0x1234, "get_u16_le(34 12) = 0x1234");
    CHECK(get_u16_be((uint8_t[]){0xFF, 0xFF}) == 0xFFFF, "get_u16_be(FF FF) = 0xFFFF (tidak negatif)");
    CHECK(get_u16_be((uint8_t[]){0x7F, 0xFF}) == 0x7FFF, "get_u16_be(7F FF) = 0x7FFF");

    CHECK(get_u32_le((uint8_t[]){0x78, 0x56, 0x34, 0x12}) == 0x12345678u, "get_u32_le(78 56 34 12) = 0x12345678");

    memset(b, 0xEE, sizeof b); put_f32_le(b, 1.0f);
    CHECK(b[0] == 0x00 && b[1] == 0x00 && b[2] == 0x80 && b[3] == 0x3F, "put_f32_le(1.0f) -> 00 00 80 3F");
    put_f32_le(b, 5.0f);
    CHECK(b[0] == 0x00 && b[1] == 0x00 && b[2] == 0xA0 && b[3] == 0x40, "put_f32_le(5.0f) -> 00 00 A0 40");
    put_f32_le(b, -1.0f);
    CHECK(b[0] == 0x00 && b[1] == 0x00 && b[2] == 0x80 && b[3] == 0xBF, "put_f32_le(-1.0f) -> 00 00 80 BF");
    put_f32_le(b, 12.5f);
    CHECK(get_f32_le(b) == 12.5f,                       "round-trip float 12.5");
    put_f32_le(b, -0.125f);
    CHECK(get_f32_le(b) == -0.125f,                     "round-trip float -0.125");

    CHECK(host_is_little_endian(),                      "host_is_little_endian() = true");

    printf("\n%s (%d gagal)\n", failures == 0 ? "SEMUA LOLOS" : "ADA YANG GAGAL", failures);
    return failures != 0;
}
