#include "cg_cmd.h"
#include "cg_id.h"
#include "payload.h"
#include "scaling.h"
#include <string.h>

/* Fungsi bantu: kosongkan frame dan isi ID + dlc. Dipakai semua builder. */
static void frame_init(CgFrame *f, uint8_t type, uint16_t data2, uint8_t target)
{
    memset(f, 0, sizeof *f);
    f->ext_id = cg_id_pack(type, data2, target);
    f->dlc    = 8;
}

void cg_build_get_device_id(CgFrame *f, uint8_t host, uint8_t motor)
{
    /* TODO: tipe CG_T_GET_DEVICE_ID, data2 = host, target = motor, data semua 0. */
    (void)f; (void)host; (void)motor; (void)frame_init;
}

void cg_build_enable(CgFrame *f, uint8_t host, uint8_t motor)
{
    /* TODO: tipe CG_T_ENABLE, data2 = host, target = motor. */
    (void)f; (void)host; (void)motor;
}

void cg_build_disable(CgFrame *f, uint8_t host, uint8_t motor)
{
    /* TODO: tipe CG_T_DISABLE, data semua 0. */
    (void)f; (void)host; (void)motor;
}

void cg_build_clear_fault(CgFrame *f, uint8_t host, uint8_t motor)
{
    /* TODO: sama dengan disable, tetapi data[0] = 1. */
    (void)f; (void)host; (void)motor;
}

void cg_build_set_zero(CgFrame *f, uint8_t host, uint8_t motor)
{
    /* TODO: tipe CG_T_SET_ZERO, data[0] = 1. */
    (void)f; (void)host; (void)motor;
}

void cg_build_set_can_id(CgFrame *f, uint8_t host, uint8_t motor, uint8_t new_id)
{
    /* TODO: tipe CG_T_SET_CAN_ID. ID baru ada di bit 23..16, host di bit 15..8
     *       -> data2 = (new_id << 8) | host.  data[0] = 1. */
    (void)f; (void)host; (void)motor; (void)new_id;
}

void cg_build_motion(CgFrame *f, uint8_t motor, float pos, float vel, float torque, float kp, float kd)
{
    /* TODO: skala semua ke 16 bit (float_to_uint dengan batas dari cg_defs.h).
     *   data2 = torsi mentah (bit 23..8 ID)
     *   data[0..1] posisi, [2..3] kecepatan, [4..5] Kp, [6..7] Kd  (semua BIG-endian) */
    (void)f; (void)motor; (void)pos; (void)vel; (void)torque; (void)kp; (void)kd;
}

void cg_build_param_read(CgFrame *f, uint8_t host, uint8_t motor, uint16_t index)
{
    /* TODO: tipe CG_T_PARAM_READ, data2 = host, data[0..1] = index (LITTLE-endian). */
    (void)f; (void)host; (void)motor; (void)index;
}

void cg_build_param_write_u8(CgFrame *f, uint8_t host, uint8_t motor, uint16_t index, uint8_t value)
{
    /* TODO: tipe CG_T_PARAM_WRITE, data[0..1] = index (LE), data[4] = value. */
    (void)f; (void)host; (void)motor; (void)index; (void)value;
}

void cg_build_param_write_f32(CgFrame *f, uint8_t host, uint8_t motor, uint16_t index, float value)
{
    /* TODO: sama, tetapi data[4..7] = float (LE). */
    (void)f; (void)host; (void)motor; (void)index; (void)value;
}

void cg_build_set_run_mode(CgFrame *f, uint8_t host, uint8_t motor, uint8_t mode)
{
    /* TODO: tulis parameter CG_PARAM_RUN_MODE dengan nilai mode (panggil builder yang sudah ada). */
    (void)f; (void)host; (void)motor; (void)mode;
}
