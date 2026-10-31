#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "registry.h"

static int failures = 0;
#define CHECK(cond, name) do { if (cond) printf("PASS  %s\n", name); else { printf("FAIL  %s\n", name); failures++; } } while (0)

static void on_crash(int sig)
{
    (void)sig;
    printf("FAIL  program crash (segfault): kemungkinan pointer NULL di-dereference\n");
    fflush(stdout);
    _exit(1);
}

int main(void)
{
    signal(SIGSEGV, on_crash);
    registry_init();
    CHECK(registry_count() == 0,              "awalnya kosong");
    CHECK(registry_get(34) == NULL,           "get motor yang belum ada -> NULL");

    CHECK(registry_add(34) && registry_add(77) && registry_add(5), "tambah 3 motor");
    CHECK(registry_count() == 3,              "count = 3");
    CHECK(registry_add(77) == true && registry_count() == 3, "tambah id yang sama: true, count tetap 3");

    Motor *a = registry_get(34), *b = registry_get(77);
    CHECK(a != NULL && b != NULL && a->id == 34 && b->id == 77, "get mengembalikan motor yang benar");
    CHECK(registry_get(34) == a,              "get dua kali -> ALAMAT yang sama");

    motor_set_torque(a, 1.5f);
    CHECK(registry_get(34)->torque == 1.5f && registry_get(34)->is_updated, "ubah lewat pointer tersimpan di array");
    CHECK(registry_get(77)->torque == 0.0f,   "motor lain tidak ikut berubah");
    motor_set_torque(NULL, 9.0f);
    CHECK(1,                                  "motor_set_torque(NULL, ...) tidak crash");

    CHECK((b - a) == 1,                       "pointer b - a = 1 (selisih dalam satuan ELEMEN)");
    CHECK((size_t)((char *)b - (char *)a) == sizeof(Motor), "selisih dalam byte = sizeof(Motor)");

    registry_init();
    int all = 1;
    for (uint8_t i = 0; i < REG_MAX_MOTORS; i++) if (!registry_add((uint8_t)(10 + i))) all = 0;
    CHECK(all && registry_count() == REG_MAX_MOTORS, "isi sampai penuh (8 motor)");
    CHECK(registry_add(99) == false,          "motor ke-9 ditolak");
    CHECK(registry_get(99) == NULL,           "motor yang ditolak tidak ada");

    uint8_t x = 3, y = 9;
    swap_u8(&x, &y);
    CHECK(x == 9 && y == 3,                   "swap_u8(&x, &y) menukar nilai asli");

    printf("\n%s (%d gagal)\n", failures == 0 ? "SEMUA LOLOS" : "ADA YANG GAGAL", failures);
    return failures != 0;
}
