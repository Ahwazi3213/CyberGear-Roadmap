#ifndef PAYLOAD_H
#define PAYLOAD_H
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

/* 16 bit: big-endian = byte tinggi di buf[0]; little-endian = byte rendah di buf[0] */
void     put_u16_be(uint8_t *buf, uint16_t v);
uint16_t get_u16_be(const uint8_t *buf);
void     put_u16_le(uint8_t *buf, uint16_t v);
uint16_t get_u16_le(const uint8_t *buf);

/* 32 bit little-endian dan float IEEE-754 little-endian (tulis sendiri per byte, jangan bergantung CPU) */
uint32_t get_u32_le(const uint8_t *buf);
void     put_f32_le(uint8_t *buf, float v);
float    get_f32_le(const uint8_t *buf);

/* Apakah CPU ini little-endian? (STM32F103 dan laptopmu: ya) */
bool     host_is_little_endian(void);

#ifdef __cplusplus
}
#endif
#endif
