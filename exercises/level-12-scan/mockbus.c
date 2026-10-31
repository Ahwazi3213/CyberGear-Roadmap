#include "mockbus.h"
#include "cg_id.h"
#include <string.h>

static void push(MockBus *b, const CgFrame *f)
{
    uint8_t next = (uint8_t)((b->head + 1) % MOCK_QUEUE);
    if (next == b->tail) return;            /* penuh: buang */
    b->q[b->head] = *f;
    b->head = next;
}

static bool mock_send(void *ctx, const CgFrame *f)
{
    MockBus *b = (MockBus *)ctx;
    b->sent_count++;
    uint8_t type = cg_id_type(f->ext_id);
    uint8_t target = cg_id_target(f->ext_id);

    if (b->noise) {                         /* feedback motor lain yang kebetulan lewat */
        CgFrame n;
        memset(&n, 0, sizeof n);
        n.ext_id = cg_id_pack(CG_T_FEEDBACK, 0x8000 | 99, 253);
        n.dlc = 8;
        push(b, &n);
    }

    for (uint8_t i = 0; i < b->n_motors; i++) {
        if (b->motors[i] != target) continue;
        CgFrame r;
        memset(&r, 0, sizeof r);
        r.dlc = 8;
        if (type == CG_T_GET_DEVICE_ID) {   /* balasan: tipe 0, motor di bit 15..8, target 0xFE, UID 8 byte */
            r.ext_id = ((uint32_t)target << 8) | 0xFEu;
            for (int k = 0; k < 8; k++) r.data[k] = (uint8_t)(0xA0 + k);
            r.data[1] = target;
            push(b, &r);
        } else if (type == CG_T_ENABLE) {   /* balasan: feedback mode RUN */
            uint8_t host = (uint8_t)(cg_id_data2(f->ext_id) & 0xFF);
            r.ext_id = ((uint32_t)CG_T_FEEDBACK << 24) | (2u << 22) | ((uint32_t)target << 8) | host;
            push(b, &r);
        }
    }
    return true;
}

static bool mock_recv(void *ctx, CgFrame *f)
{
    MockBus *b = (MockBus *)ctx;
    if (b->head == b->tail) return false;
    *f = b->q[b->tail];
    b->tail = (uint8_t)((b->tail + 1) % MOCK_QUEUE);
    return true;
}

static uint32_t mock_now(void *ctx)
{
    MockBus *b = (MockBus *)ctx;
    return b->clock_ms++;                   /* waktu maju 1 ms setiap kali dibaca */
}

void mock_init(MockBus *b)
{
    memset(b, 0, sizeof *b);
}

void mock_add_motor(MockBus *b, uint8_t id)
{
    if (b->n_motors < MOCK_MAX_MOTORS) b->motors[b->n_motors++] = id;
}

CgTransport mock_transport(MockBus *b)
{
    CgTransport t = { mock_send, mock_recv, mock_now, b };
    return t;
}
