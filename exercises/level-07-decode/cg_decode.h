#ifndef CG_DECODE_H
#define CG_DECODE_H
#include "cg_defs.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Hasil decode frame feedback (tipe 2) */
typedef struct {
    uint8_t motor_id;      /* bit 15..8 dari ID               */
    uint8_t host_id;       /* bit 7..0 dari ID                */
    uint8_t mode;          /* 0 reset, 1 kalibrasi, 2 run     */
    uint8_t fault_bits;    /* 6 bit mentah (bit 21..16 ID)    */
    float   position;      /* rad                              */
    float   velocity;      /* rad/s                            */
    float   torque;        /* Nm                               */
    float   temperature;   /* derajat Celsius                  */
} CgFeedback;

/* Arti tiap bit pada fault_bits (nomor bit = posisi di ID 29 bit) */
typedef struct {
    bool under_voltage;    /* bit 16 */
    bool over_current;     /* bit 17 */
    bool over_temp;        /* bit 18 */
    bool magnetic_fault;   /* bit 19 */
    bool hall_fault;       /* bit 20 */
    bool not_calibrated;   /* bit 21 */
} CgFaultBits;

/* Frame fault (tipe 21): nilai 32 bit fault dan warning */
#define CG_FLT_OVER_TEMP        (1u << 0)
#define CG_FLT_DRIVER_CHIP      (1u << 1)
#define CG_FLT_UNDER_VOLTAGE    (1u << 2)
#define CG_FLT_OVER_VOLTAGE     (1u << 3)
#define CG_FLT_B_OVERCURRENT    (1u << 4)
#define CG_FLT_C_OVERCURRENT    (1u << 5)
#define CG_FLT_ENCODER_UNCAL    (1u << 7)
#define CG_FLT_OVERLOAD_MASK    (0xFFu << 8)
#define CG_FLT_A_OVERCURRENT    (1u << 16)
#define CG_WARN_OVER_TEMP       (1u << 0)

typedef struct {
    uint32_t fault_value;
    uint32_t warning_value;
} CgFaultReport;

bool        cg_decode_feedback(const CgFrame *f, CgFeedback *out);     /* false jika bukan tipe 2 atau dlc < 8 */
CgFaultBits cg_decode_fault_bits(uint8_t bits);
const char *cg_mode_name(uint8_t mode);                                /* "RESET", "CALI", "RUN", "UNKNOWN"     */
bool        cg_decode_fault_report(const CgFrame *f, CgFaultReport *out); /* false jika bukan tipe 0x15 / dlc < 8 */
bool        cg_decode_device_id(const CgFrame *f, uint8_t *motor_id, uint8_t uid[8]); /* balasan tipe 0, target 0xFE */

#ifdef __cplusplus
}
#endif
#endif
