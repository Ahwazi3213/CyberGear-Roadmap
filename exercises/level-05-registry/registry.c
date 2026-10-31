#include "registry.h"
#include <string.h>

static Motor    s_motors[REG_MAX_MOTORS];
static uint8_t  s_count = 0;

void registry_init(void)
{
    /* TODO: s_count = 0 dan memset seluruh array dengan 0 (pakai sizeof). */
    (void)s_motors; (void)memset;
}

bool registry_add(uint8_t id)
{
    /* TODO:
     *  - jika id sudah terdaftar -> return true (jangan tambah lagi)
     *  - jika daftar penuh       -> return false
     *  - selain itu: ambil alamat elemen berikutnya, isi id-nya, naikkan s_count, return true
     */
    (void)id;
    return false;
}

Motor *registry_get(uint8_t id)
{
    /* TODO: cari id di s_motors[0..s_count-1]; kembalikan ALAMATNYA (&s_motors[i]), atau NULL. */
    (void)id;
    return NULL;
}

uint8_t registry_count(void) { return s_count; }

void motor_set_torque(Motor *m, float t)
{
    /* TODO: jika m bukan NULL, ubah m->torque dan tandai is_updated = true. */
    (void)m; (void)t;
}

void swap_u8(uint8_t *a, uint8_t *b)
{
    /* TODO: tukar isi dua variabel lewat alamatnya. */
    (void)a; (void)b;
}
