#ifndef CG_DEFS_H
#define CG_DEFS_H

#include <stdint.h>
#include <stdbool.h>

#define CG_PI 3.14159265359f

/* Batas skala (sama dengan library yang diberikan) */
#define CG_POSITION_MAX  (4.0f * CG_PI)   /* rad   */
#define CG_VELOCITY_MAX  30.0f            /* rad/s */
#define CG_TORQUE_MAX    12.0f            /* Nm    */
#define CG_KP_MAX        500.0f
#define CG_KD_MAX        5.0f

/* Tipe komunikasi (bit 28..24 pada Extended ID) */
typedef enum {
    CG_T_GET_DEVICE_ID  = 0x00,
    CG_T_CONTROL        = 0x01,
    CG_T_FEEDBACK       = 0x02,
    CG_T_ENABLE         = 0x03,
    CG_T_DISABLE        = 0x04,
    CG_T_SET_ZERO       = 0x06,
    CG_T_SET_CAN_ID     = 0x07,
    CG_T_PARAM_READ     = 0x11,
    CG_T_PARAM_WRITE    = 0x12,
    CG_T_FAULT_REPORT   = 0x15
} CgCommType;

/* Indeks parameter (tabel 4.1.12 manual) */
#define CG_PARAM_RUN_MODE      0x7005u
#define CG_PARAM_IQ_REF        0x7006u
#define CG_PARAM_SPD_REF       0x700Au
#define CG_PARAM_LIMIT_TORQUE  0x700Bu
#define CG_PARAM_LOC_REF       0x7016u
#define CG_PARAM_LIMIT_SPD     0x7017u
#define CG_PARAM_LIMIT_CUR     0x7018u

/* Nilai run_mode */
#define CG_RUN_MODE_OPERATION  0u   /* kontrol operasi (MIT) */
#define CG_RUN_MODE_POSITION   1u
#define CG_RUN_MODE_SPEED      2u
#define CG_RUN_MODE_CURRENT    3u

/* Satu frame CAN Extended */
typedef struct {
    uint32_t ext_id;     /* 29 bit */
    uint8_t  dlc;        /* 0..8   */
    uint8_t  data[8];
} CgFrame;

#endif /* CG_DEFS_H */
