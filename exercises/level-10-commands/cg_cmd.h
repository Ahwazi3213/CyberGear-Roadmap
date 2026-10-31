#ifndef CG_CMD_H
#define CG_CMD_H
#include "cg_defs.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Semua fungsi mengisi satu CgFrame (ext_id, dlc = 8, data) dan TIDAK mengirim apa pun.
 * Mengirim adalah tugas lapisan transport (HAL_CAN_AddTxMessage di STM32). */

void cg_build_get_device_id(CgFrame *f, uint8_t host, uint8_t motor);
void cg_build_enable       (CgFrame *f, uint8_t host, uint8_t motor);
void cg_build_disable      (CgFrame *f, uint8_t host, uint8_t motor);
void cg_build_clear_fault  (CgFrame *f, uint8_t host, uint8_t motor);   /* tipe 4, data[0] = 1 */
void cg_build_set_zero     (CgFrame *f, uint8_t host, uint8_t motor);   /* tipe 6, data[0] = 1 */
void cg_build_set_can_id   (CgFrame *f, uint8_t host, uint8_t motor, uint8_t new_id);

void cg_build_motion(CgFrame *f, uint8_t motor, float pos, float vel, float torque, float kp, float kd);

void cg_build_param_read     (CgFrame *f, uint8_t host, uint8_t motor, uint16_t index);
void cg_build_param_write_u8 (CgFrame *f, uint8_t host, uint8_t motor, uint16_t index, uint8_t value);
void cg_build_param_write_f32(CgFrame *f, uint8_t host, uint8_t motor, uint16_t index, float value);

void cg_build_set_run_mode   (CgFrame *f, uint8_t host, uint8_t motor, uint8_t mode);

#ifdef __cplusplus
}
#endif
#endif
