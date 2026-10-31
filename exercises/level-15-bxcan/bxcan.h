#ifndef BXCAN_H
#define BXCAN_H
#include "bxcan_regs.h"
#include <stdbool.h>

typedef struct {
    uint16_t prescaler;    /* 1..1024 (disimpan ke register sebagai prescaler - 1) */
    uint8_t  bs1;          /* 1..16 time quanta */
    uint8_t  bs2;          /* 1..8  time quanta */
    uint8_t  sjw;          /* 1..4  time quanta */
    bool     loopback;     /* BTR.LBKM */
    bool     silent;       /* BTR.SILM */
    bool     auto_retx;    /* true  -> MCR.NART = 0 (kirim ulang otomatis) */
} BxCanConfig;

#define BX_WAIT_MAX_ITER 100000u

/* Hook untuk pengujian: dipanggil di setiap putaran fungsi tunggu, sehingga laptop bisa
 * "berpura-pura menjadi hardware" (mengatur INAK/SLAK). Di mikrokontroler biarkan NULL. */
extern void (*bx_wait_hook)(void);

bool bx_init(BxCan *can, const BxCanConfig *cfg);                              /* false jika timeout          */
bool bx_transmit(BxCan *can, uint32_t ext_id, const uint8_t *data, uint8_t dlc); /* false jika tidak ada mailbox */
bool bx_receive(BxCan *can, uint32_t *ext_id, uint8_t *data, uint8_t *dlc);      /* false jika FIFO0 kosong      */
void bx_enable_rx_irq(BxCan *can);

#endif
