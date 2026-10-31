#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "cg_scan.h"
#include "mockbus.h"

static int failures = 0;
#define CHECK(cond, name) do { if (cond) printf("PASS  %s\n", name); else { printf("FAIL  %s\n", name); failures++; } } while (0)

static void on_alarm(int sig)
{
    (void)sig;
    printf("FAIL  loop tidak berakhir dalam 5 detik (jebakan uint8_t dengan last = 255?)\n");
    fflush(stdout);
    _exit(1);
}

static int has(const uint8_t *a, uint8_t n, uint8_t v)
{
    for (uint8_t i = 0; i < n; i++) if (a[i] == v) return 1;
    return 0;
}

int main(void)
{
    signal(SIGALRM, on_alarm);
    alarm(5);

    MockBus bus; CgTransport t; uint8_t found[8]; uint8_t n;

    mock_init(&bus); mock_add_motor(&bus, 34); mock_add_motor(&bus, 77);
    t = mock_transport(&bus);
    n = cg_scan(&t, 253, 0, 127, 5, found, 8);
    CHECK(n == 2 && has(found, n, 34) && has(found, n, 77), "scan 0..127: menemukan motor 34 dan 77");
    CHECK(bus.sent_count == 128,                             "scan 0..127 mengirim tepat 128 frame (satu per ID)");

    mock_init(&bus); mock_add_motor(&bus, 34); mock_add_motor(&bus, 77);
    t = mock_transport(&bus);
    n = cg_scan(&t, 253, 40, 127, 5, found, 8);
    CHECK(n == 1 && found[0] == 77,                          "scan 40..127: hanya 77 (34 di luar rentang)");

    mock_init(&bus); t = mock_transport(&bus);
    n = cg_scan(&t, 253, 0, 127, 5, found, 8);
    CHECK(n == 0,                                            "bus kosong: tidak menemukan apa pun");

    mock_init(&bus); mock_add_motor(&bus, 1); mock_add_motor(&bus, 2); mock_add_motor(&bus, 3);
    t = mock_transport(&bus);
    n = cg_scan(&t, 253, 0, 127, 5, found, 2);
    CHECK(n == 2,                                            "max_found = 2: tidak melebihi kapasitas found[]");

    mock_init(&bus); mock_add_motor(&bus, 34); bus.noise = true;
    t = mock_transport(&bus);
    n = cg_scan(&t, 253, 0, 127, 5, found, 8);
    CHECK(n == 1 && found[0] == 34,                          "frame feedback nyasar diabaikan, hanya balasan Device ID yang dihitung");

    mock_init(&bus); mock_add_motor(&bus, 250); t = mock_transport(&bus);
    n = cg_scan(&t, 253, 245, 255, 5, found, 8);
    CHECK(n == 1 && found[0] == 250,                         "scan sampai last = 255 harus berakhir (tanpa overflow uint8_t)");

    mock_init(&bus); mock_add_motor(&bus, 34); t = mock_transport(&bus);
    n = cg_scan(&t, 253, 30, 30, 5, found, 8);
    CHECK(n == 0 && bus.sent_count == 1,                     "rentang satu ID (30..30): 1 frame, tidak ada motor");

    printf("\n%s (%d gagal)\n", failures == 0 ? "SEMUA LOLOS" : "ADA YANG GAGAL", failures);
    return failures != 0;
}
