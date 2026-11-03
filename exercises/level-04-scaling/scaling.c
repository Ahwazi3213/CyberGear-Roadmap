#include "scaling.h"

uint16_t float_to_uint(float val, float min, float max)
{
    if(val < min) val = min;
    if(val > max) val = max;

    uint16_t raw = ((val-min)/(max-min))*65535.0f;
    return raw;
}

float uint_to_float(uint16_t raw, float min, float max)
{
    float val = (raw*(max-min)/65535.0f)+min;
    return val;
}

float scale_resolution(float min, float max)
{
    float res = (max-min)/65535.0f;
    return res;
}
