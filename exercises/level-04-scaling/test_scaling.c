#include <stdio.h>
#include <math.h>
#include "scaling.h"
#include "../common/cg_defs.h"

static int failures = 0;
#define CHECK(cond, name) do { if (cond) printf("PASS  %s\n", name); else { printf("FAIL  %s\n", name); failures++; } } while (0)
#define NEAR(a, b, tol) (fabsf((a) - (b)) <= (tol))

int main(void)
{
    /* torsi -12..12 Nm */
    CHECK(float_to_uint(-12.0f, -12.0f, 12.0f) == 0,      "torsi -12 Nm -> 0");
    CHECK(float_to_uint( 12.0f, -12.0f, 12.0f) == 65535,  "torsi +12 Nm -> 65535");
    CHECK(float_to_uint(  0.0f, -12.0f, 12.0f) == 32767,  "torsi 0 Nm -> 32767 (32767.5 dipotong)");
    CHECK(float_to_uint(100.0f, -12.0f, 12.0f) == 65535,  "nilai di atas max di-clamp ke 65535");
    CHECK(float_to_uint(-100.0f, -12.0f, 12.0f) == 0,     "nilai di bawah min di-clamp ke 0");

    /* posisi -4pi..4pi */
    CHECK(float_to_uint(0.0f, -CG_POSITION_MAX, CG_POSITION_MAX) == 32767,   "posisi 0 rad -> 32767");
    CHECK(float_to_uint(CG_PI, -CG_POSITION_MAX, CG_POSITION_MAX) == 40959,  "posisi pi rad -> 40959 (5/8 x 65535)");

    /* Kp 0..500, Kd 0..5 */
    CHECK(float_to_uint(500.0f, 0.0f, CG_KP_MAX) == 65535, "Kp 500 -> 65535");
    CHECK(float_to_uint(1.0f, 0.0f, CG_KP_MAX) == 131,     "Kp 1 -> 131 (131,07 dipotong)");
    CHECK(float_to_uint(5.0f, 0.0f, CG_KD_MAX) == 65535,   "Kd 5 -> 65535");

    /* arah sebaliknya */
    CHECK(NEAR(uint_to_float(0, -12.0f, 12.0f), -12.0f, 1e-4f),      "0 -> -12 Nm");
    CHECK(NEAR(uint_to_float(65535, -12.0f, 12.0f), 12.0f, 1e-4f),   "65535 -> +12 Nm");
    CHECK(NEAR(uint_to_float(32768, -12.0f, 12.0f), 0.0f, 5e-4f),    "32768 -> sekitar 0 Nm");
    CHECK(NEAR(uint_to_float(0xFFFF, -CG_VELOCITY_MAX, CG_VELOCITY_MAX), 30.0f, 1e-3f), "kecepatan 0xFFFF -> +30 rad/s");

    /* resolusi */
    CHECK(NEAR(scale_resolution(-12.0f, 12.0f), 0.000366f, 1e-6f),   "resolusi torsi = 0,000366 Nm");
    CHECK(NEAR(scale_resolution(-CG_POSITION_MAX, CG_POSITION_MAX), 0.000383f, 1e-5f), "resolusi posisi = 0,000383 rad");

    /* round trip: galat tidak boleh lebih dari satu anak tangga */
    int ok = 1;
    for (float t = -11.9f; t < 11.9f; t += 0.37f) {
        float back = uint_to_float(float_to_uint(t, -12.0f, 12.0f), -12.0f, 12.0f);
        if (fabsf(back - t) > scale_resolution(-12.0f, 12.0f) * 1.01f) ok = 0;
    }
    CHECK(ok, "round-trip: galat <= 1 anak tangga");

    printf("\n%s (%d gagal)\n", failures == 0 ? "SEMUA LOLOS" : "ADA YANG GAGAL", failures);
    return failures != 0;
}
