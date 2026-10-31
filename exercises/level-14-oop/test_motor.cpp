#include <stdio.h>
#include <string.h>
#include <math.h>
#include "cg_motor.hpp"

static int failures = 0;
#define CHECK(cond, name) do { if (cond) printf("PASS  %s\n", name); else { printf("FAIL  %s\n", name); failures++; } } while (0)

/* Transport tiruan: merekam frame terakhir yang dikirim */
class MockTransport : public CanTransport {
public:
    CgFrame last;
    int     count;
    bool    ok;
    MockTransport() : count(0), ok(true) { memset(&last, 0, sizeof last); }
    bool send(const CgFrame &f) { last = f; count++; return ok; }
};

int main()
{
    MockTransport bus;
    CyberGearMotor m34(bus, 253, 34);
    CyberGearMotor m77(bus, 253, 77);

    CHECK(m34.id() == 34 && m77.id() == 77,               "dua objek punya ID masing-masing");

    CHECK(m34.enable() && bus.last.ext_id == 0x0300FD22u, "m34.enable() mengirim 0x0300FD22");
    CHECK(m77.enable() && bus.last.ext_id == 0x0300FD4Du, "m77.enable() mengirim 0x0300FD4D (ID 77 = 0x4D)");
    CHECK(m34.disable() && bus.last.ext_id == 0x0400FD22u && bus.last.data[0] == 0, "disable");
    CHECK(m34.clearFault() && bus.last.ext_id == 0x0400FD22u && bus.last.data[0] == 1, "clearFault");
    CHECK(m34.setZero() && bus.last.ext_id == 0x0600FD22u && bus.last.data[0] == 1,    "setZero");
    CHECK(m34.setRunMode(2) && bus.last.ext_id == 0x1200FD22u && bus.last.data[0] == 0x05 && bus.last.data[1] == 0x70 && bus.last.data[4] == 2, "setRunMode(2)");
    CHECK(m77.sendMotion(0, 0, 0, 1, 0) && bus.last.ext_id == 0x017FFF4Du && bus.last.data[5] == 0x83, "sendMotion ke m77");

    int before = bus.count;
    bus.ok = false;
    CHECK(!m34.enable() && bus.count == before + 1,       "hasil send (gagal) diteruskan ke pemanggil");
    bus.ok = true;

    /* Feedback milik motor 34 (suhu 35,5 C, mode RUN, fault over-temp) */
    CgFrame fb; memset(&fb, 0, sizeof fb);
    fb.ext_id = 0x028422FDu; fb.dlc = 8;
    fb.data[0] = 0x80; fb.data[2] = 0xFF; fb.data[3] = 0xFF; fb.data[6] = 0x01; fb.data[7] = 0x63;

    CHECK(!m34.isUpdated(),                               "awalnya belum ada update");
    CHECK(m77.onFrame(fb) == false && !m77.isUpdated(),   "frame motor 34 DITOLAK oleh objek motor 77");
    CHECK(m34.onFrame(fb) == true && m34.isUpdated(),     "frame motor 34 diterima objek motor 34");
    CHECK(fabsf(m34.temperature() - 35.5f) < 0.01f,       "temperature() = 35,5");
    CHECK(fabsf(m34.velocity() - 30.0f) < 0.01f,          "velocity() = 30");
    CHECK(m34.mode() == 2 && m34.hasFault(),              "mode() = 2 dan hasFault() = true");
    m34.clearUpdated();
    CHECK(!m34.isUpdated(),                               "clearUpdated() menurunkan flag");

    CgFrame bad = fb; bad.ext_id = 0x03000000u | 0x22FDu;
    CHECK(!m34.onFrame(bad),                              "frame bertipe lain ditolak");

    /* polimorfisme: referensi ke kelas dasar */
    CanTransport &base = bus;
    CgFrame f2; memset(&f2, 0, sizeof f2);
    CHECK(base.send(f2),                                  "lewat referensi CanTransport& tetap memanggil send milik MockTransport");

    printf("\n%s (%d gagal)\n", failures == 0 ? "SEMUA LOLOS" : "ADA YANG GAGAL", failures);
    return failures != 0;
}
