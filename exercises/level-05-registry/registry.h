#ifndef REGISTRY_H
#define REGISTRY_H
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

#define REG_MAX_MOTORS 8

typedef struct {
    uint8_t id;
    float   position;
    float   velocity;
    float   torque;
    float   temperature;
    bool    is_error;
    bool    is_updated;
} Motor;

void     registry_init(void);               /* kosongkan daftar (memset)                        */
bool     registry_add(uint8_t id);          /* true jika berhasil / sudah ada; false jika penuh */
Motor   *registry_get(uint8_t id);          /* alamat motor, atau NULL jika tidak ada           */
uint8_t  registry_count(void);

void     motor_set_torque(Motor *m, float t);   /* ubah lewat alamat; abaikan jika m == NULL     */
void     swap_u8(uint8_t *a, uint8_t *b);       /* latihan pointer klasik                        */

#ifdef __cplusplus
}
#endif
#endif
