#include "payload.h"
#include <string.h>     /* memcpy */

void     put_u16_be(uint8_t *buf, uint16_t v) { /* TODO */ (void)buf; (void)v; }
uint16_t get_u16_be(const uint8_t *buf)       { /* TODO */ (void)buf; return 0; }
void     put_u16_le(uint8_t *buf, uint16_t v) { /* TODO */ (void)buf; (void)v; }
uint16_t get_u16_le(const uint8_t *buf)       { /* TODO */ (void)buf; return 0; }

uint32_t get_u32_le(const uint8_t *buf)
{
    /* TODO: buf[0] adalah byte paling rendah. */
    (void)buf;
    return 0;
}

void put_f32_le(uint8_t *buf, float v)
{
    /* TODO: salin bit float ke uint32_t dengan memcpy (bukan casting nilai!),
     * lalu tulis 4 byte dari yang terendah. */
    (void)buf; (void)v;
    (void)memcpy;
}

float get_f32_le(const uint8_t *buf)
{
    /* TODO: kebalikan put_f32_le: susun uint32_t dari 4 byte, lalu memcpy ke float. */
    (void)buf;
    return 0.0f;
}

bool host_is_little_endian(void)
{
    /* TODO: simpan uint16_t 0x0001, lalu baca byte pertamanya lewat pointer uint8_t*. */
    return false;
}
