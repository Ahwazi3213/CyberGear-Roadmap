#ifndef SCALING_H
#define SCALING_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/* float dalam rentang [min, max] -> angka 16 bit 0..65535 (dibatasi/clamp, dipotong ke bawah) */
uint16_t float_to_uint(float val, float min, float max);

/* angka 16 bit 0..65535 -> float dalam rentang [min, max] */
float    uint_to_float(uint16_t raw, float min, float max);

/* satu "anak tangga" kuantisasi: (max - min) / 65535 */
float    scale_resolution(float min, float max);

#ifdef __cplusplus
}
#endif
#endif
