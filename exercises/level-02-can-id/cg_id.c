#include "cg_id.h"

uint32_t cg_id_pack(uint8_t type, uint16_t data2, uint8_t target)
{
    /* TODO: ((uint32_t)type << 24) | ((uint32_t)data2 << 8) | target
     * Ingat: tipe hanya 5 bit, jadi mask dulu dengan 0x1F. */
    (void)type; (void)data2; (void)target;
    return 0;
}

uint8_t  cg_id_type  (uint32_t id) { /* TODO */ (void)id; return 0; }
uint16_t cg_id_data2 (uint32_t id) { /* TODO */ (void)id; return 0; }
uint8_t  cg_id_target(uint32_t id) { /* TODO */ (void)id; return 0; }
uint8_t  cg_id_mode  (uint32_t id) { /* TODO */ (void)id; return 0; }
uint8_t  cg_id_fault (uint32_t id) { /* TODO */ (void)id; return 0; }
uint8_t  cg_id_motor (uint32_t id) { /* TODO */ (void)id; return 0; }
