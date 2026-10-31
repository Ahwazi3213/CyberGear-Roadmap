#include "scaling.h"

uint16_t float_to_uint(float val, float min, float max)
{
    /* TODO:
     *   1) batasi val ke [min, max]
     *   2) kembalikan (uint16_t)((val - min) * 65535.0f / (max - min))
     */
    (void)val; (void)min; (void)max;
    return 0;
}

float uint_to_float(uint16_t raw, float min, float max)
{
    /* TODO: ((float)raw * (max - min) / 65535.0f) + min */
    (void)raw; (void)min; (void)max;
    return 0.0f;
}

float scale_resolution(float min, float max)
{
    /* TODO */
    (void)min; (void)max;
    return 0.0f;
}
