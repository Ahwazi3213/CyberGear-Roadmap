#ifndef BITS_H
#define BITS_H
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

uint32_t bit_set   (uint32_t v, uint8_t n);   /* jadikan bit ke-n = 1            */
uint32_t bit_clear (uint32_t v, uint8_t n);   /* jadikan bit ke-n = 0            */
uint32_t bit_toggle(uint32_t v, uint8_t n);   /* balik bit ke-n                  */
bool     bit_test  (uint32_t v, uint8_t n);   /* true jika bit ke-n = 1          */

uint32_t mask_bits (uint8_t width);           /* (1 << width) - 1, width 0..32   */
uint32_t get_field (uint32_t v, uint8_t shift, uint8_t width);                  /* ambil field   */
uint32_t set_field (uint32_t v, uint8_t shift, uint8_t width, uint32_t field);  /* tulis field   */

/* Tulis nbits bit terendah dari v sebagai teks "1010..." ke out (ukuran minimal nbits+1). */
void     to_binary_string(uint32_t v, uint8_t nbits, char *out);

#ifdef __cplusplus
}
#endif
#endif
