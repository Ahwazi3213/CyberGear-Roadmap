#ifndef CG_ID_H
#define CG_ID_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/* Susun Extended ID: [tipe:5][data2:16][target:8] = 29 bit */
uint32_t cg_id_pack(uint8_t type, uint16_t data2, uint8_t target);

/* Bongkar Extended ID */
uint8_t  cg_id_type  (uint32_t id);   /* bit 28..24 */
uint16_t cg_id_data2 (uint32_t id);   /* bit 23..8  */
uint8_t  cg_id_target(uint32_t id);   /* bit 7..0   */

/* Khusus frame feedback (tipe 2) */
uint8_t  cg_id_mode  (uint32_t id);   /* bit 23..22 */
uint8_t  cg_id_fault (uint32_t id);   /* bit 21..16 */
uint8_t  cg_id_motor (uint32_t id);   /* bit 15..8  */

#ifdef __cplusplus
}
#endif
#endif
