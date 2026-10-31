#include "bxcan.h"

void (*bx_wait_hook)(void) = 0;

/* DISEDIAKAN: tunggu sampai bit 'mask' di *reg bernilai want_set, atau menyerah (timeout). */
static bool wait_flag(volatile uint32_t *reg, uint32_t mask, bool want_set)
{
    for (uint32_t i = 0; i < BX_WAIT_MAX_ITER; i++) {
        if (bx_wait_hook) bx_wait_hook();
        bool is_set = (*reg & mask) != 0;
        if (is_set == want_set) return true;
    }
    return false;
}

bool bx_init(BxCan *can, const BxCanConfig *cfg)
{
    /* TODO (urutan penting!):
     *  1) Keluar dari sleep: MCR &= ~SLEEP, tunggu MSR.SLAK == 0
     *  2) Minta mode inisialisasi: MCR |= INRQ, tunggu MSR.INAK == 1
     *  3) Atur NART: cfg->auto_retx ? bersihkan bit NART : set bit NART
     *  4) BTR = BRP(prescaler-1) | TS1(bs1-1) | TS2(bs2-1) | SJW(sjw-1) | LBKM jika loopback | SILM jika silent
     *  5) Filter 0 terima-semua:  FMR |= FINIT; FA1R bank0 off; FS1R bank0 = 32 bit; FM1R bank0 = mask;
     *     FFA1R bank0 -> FIFO0; FILTER[0].FR1 = 0; FILTER[0].FR2 = 0 (mask 0 = semua lolos);
     *     FA1R bank0 on; FMR &= ~FINIT
     *  6) Keluar dari mode inisialisasi: MCR &= ~INRQ, tunggu MSR.INAK == 0
     *  Kembalikan false jika ada langkah tunggu yang timeout. */
    (void)can; (void)cfg; (void)wait_flag;
    return false;
}

bool bx_transmit(BxCan *can, uint32_t ext_id, const uint8_t *data, uint8_t dlc)
{
    /* TODO:
     *  1) cari mailbox i (0..2) yang kosong: TSR & BX_TSR_TME(i). Jika tidak ada -> return false
     *  2) TIR  = (ext_id << EXID_SHIFT) | IDE          (belum TXRQ)
     *  3) TDTR = dlc & 0xF
     *  4) TDLR = data[0] | data[1]<<8 | data[2]<<16 | data[3]<<24    (byte 0 = byte paling rendah)
     *     TDHR = data[4..7] dengan cara sama
     *  5) terakhir: TIR |= TXRQ  (memulai pengiriman)
     *  return true */
    (void)can; (void)ext_id; (void)data; (void)dlc;
    return false;
}

bool bx_receive(BxCan *can, uint32_t *ext_id, uint8_t *data, uint8_t *dlc)
{
    /* TODO:
     *  1) jika (RF0R & FMP0) == 0 -> FIFO kosong, return false
     *  2) dari RX[0]: RIR >> 3 = ID extended (jika bit IDE = 1); RDTR & 0xF = dlc;
     *     RDLR dan RDHR dipecah menjadi data[0..7] (byte rendah -> data[0])
     *  3) lepaskan pesan dari FIFO: RF0R |= RFOM0   (tanpa ini FIFO tidak maju!)
     *  return true */
    (void)can; (void)ext_id; (void)data; (void)dlc;
    return false;
}

void bx_enable_rx_irq(BxCan *can)
{
    /* TODO: IER |= FMPIE0 */
    (void)can;
}
