#ifndef MOCKBUS_H
#define MOCKBUS_H
#include "cg_scan.h"

/* Bus CAN tiruan berisi beberapa motor. Dipakai untuk menguji cg_scan tanpa hardware. */
#define MOCK_MAX_MOTORS 8
#define MOCK_QUEUE 64

typedef struct {
    uint8_t  motors[MOCK_MAX_MOTORS];   /* ID motor yang "ada" di bus */
    uint8_t  n_motors;
    CgFrame  q[MOCK_QUEUE];
    uint8_t  head, tail;
    uint32_t clock_ms;
    bool     noise;                     /* jika true: tiap kirim, antrean diisi feedback nyasar dulu */
    uint32_t sent_count;
} MockBus;

void mock_init(MockBus *b);
void mock_add_motor(MockBus *b, uint8_t id);
CgTransport mock_transport(MockBus *b);

#endif
