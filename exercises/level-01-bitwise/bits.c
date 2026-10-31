#include "bits.h"

uint32_t bit_set(uint32_t v, uint8_t n)    { /* TODO */ (void)n; return v; }
uint32_t bit_clear(uint32_t v, uint8_t n)  { /* TODO */ (void)n; return v; }
uint32_t bit_toggle(uint32_t v, uint8_t n) { /* TODO */ (void)n; return v; }
bool     bit_test(uint32_t v, uint8_t n)   { /* TODO */ (void)v; (void)n; return false; }

uint32_t mask_bits(uint8_t width)
{
    /* TODO: (1 << width) - 1.
     * Jebakan: menggeser 32-bit sebanyak 32 posisi itu undefined behavior di C.
     * Tangani width == 32 secara khusus. */
    (void)width;
    return 0;
}

uint32_t get_field(uint32_t v, uint8_t shift, uint8_t width)
{
    /* TODO: geser field ke posisi 0, lalu buang sisanya dengan mask. */
    (void)v; (void)shift; (void)width;
    return 0;
}

uint32_t set_field(uint32_t v, uint8_t shift, uint8_t width, uint32_t field)
{
    /* TODO: kosongkan dulu bagian field di v, lalu tulis field (yang juga harus dimask) ke posisinya. */
    (void)shift; (void)width; (void)field;
    return v;
}

void to_binary_string(uint32_t v, uint8_t nbits, char *out)
{
    /* TODO: karakter pertama = bit tertinggi (nbits-1), terakhir = bit 0; akhiri dengan '\0'. */
    (void)v; (void)nbits;
    out[0] = '\0';
}
