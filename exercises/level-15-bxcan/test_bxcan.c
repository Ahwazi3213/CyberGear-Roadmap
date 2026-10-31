#include <stdio.h>
#include <string.h>
#include "bxcan.h"

static int failures = 0;
#define CHECK(cond, name) do { if (cond) printf("PASS  %s\n", name); else { printf("FAIL  %s\n", name); failures++; } } while (0)

static BxCan fake;

/* "Hardware palsu": meniru perilaku INAK dan SLAK terhadap INRQ dan SLEEP */
static void hw_sim(void)
{
    if (fake.MCR & BX_MCR_INRQ)  fake.MSR |= BX_MSR_INAK;  else fake.MSR &= ~BX_MSR_INAK;
    if (fake.MCR & BX_MCR_SLEEP) fake.MSR |= BX_MSR_SLAK;  else fake.MSR &= ~BX_MSR_SLAK;
}

static void reset_fake(void)
{
    memset(&fake, 0, sizeof fake);
    fake.MCR = 0x00010002u;      /* nilai reset: SLEEP = 1, DBF = 1 */
    fake.MSR = 0x00000C02u;      /* SLAK = 1 */
    fake.FMR = 0x2A1C0E01u;      /* nilai reset: FINIT = 1 */
    bx_wait_hook = hw_sim;
}

int main(void)
{
    BxCanConfig cfg = { 4, 6, 2, 1, false, false, true };   /* 1 Mbps pada APB1 36 MHz */

    reset_fake();
    CHECK(bx_init(&fake, &cfg),                               "bx_init selesai tanpa timeout");
    CHECK((fake.MCR & BX_MCR_SLEEP) == 0,                     "SLEEP sudah dibersihkan");
    CHECK((fake.MCR & BX_MCR_INRQ)  == 0,                     "INRQ sudah dibersihkan (keluar mode init)");
    CHECK((fake.MCR & BX_MCR_NART)  == 0,                     "auto_retx = true -> NART = 0");
    CHECK(fake.BTR == 0x00150003u,                            "BTR = 0x00150003 (BRP=3, TS1=5, TS2=1, SJW=0)");
    CHECK((fake.FMR & BX_FMR_FINIT) == 0,                     "FINIT dibersihkan setelah filter siap");
    CHECK((fake.FA1R & 1u) && (fake.FS1R & 1u),               "filter 0 aktif dan skala 32 bit");
    CHECK((fake.FM1R & 1u) == 0 && (fake.FFA1R & 1u) == 0,    "filter 0 mode mask dan ke FIFO0");
    CHECK(fake.FILTER[0].FR1 == 0 && fake.FILTER[0].FR2 == 0, "filter 0: ID 0, mask 0 (terima semua)");

    reset_fake(); cfg.loopback = true; cfg.auto_retx = false;
    CHECK(bx_init(&fake, &cfg),                               "bx_init mode loopback");
    CHECK(fake.BTR == (0x00150003u | BX_BTR_LBKM),            "loopback menyalakan bit LBKM (bit 30)");
    CHECK((fake.MCR & BX_MCR_NART) != 0,                      "auto_retx = false -> NART = 1");

    reset_fake(); cfg.loopback = false; cfg.silent = true;
    bx_init(&fake, &cfg);
    CHECK(fake.BTR == (0x00150003u | BX_BTR_SILM),            "silent menyalakan bit SILM (bit 31)");

    /* TIMEOUT: hardware tidak pernah menjawab */
    reset_fake(); bx_wait_hook = 0;
    cfg.silent = false;
    CHECK(!bx_init(&fake, &cfg),                              "tanpa hardware yang menjawab: bx_init mengembalikan false");

    /* TRANSMIT */
    uint8_t d[8] = { 0x7F, 0xFF, 0x7F, 0xFF, 0x00, 0x83, 0x00, 0x00 };
    reset_fake();
    fake.TSR = BX_TSR_TME(0) | BX_TSR_TME(1) | BX_TSR_TME(2);
    CHECK(bx_transmit(&fake, 0x017FFF22u, d, 8),              "transmit: ada mailbox kosong");
    CHECK(fake.TX[0].TIR == ((0x017FFF22u << 3) | BX_TIR_IDE | BX_TIR_TXRQ), "TIR = (ID << 3) | IDE | TXRQ");
    CHECK((fake.TX[0].TDTR & 0xF) == 8,                       "DLC = 8");
    CHECK(fake.TX[0].TDLR == 0xFF7FFF7Fu,                     "TDLR = byte 0..3 (byte 0 paling rendah)");
    CHECK(fake.TX[0].TDHR == 0x00008300u,                     "TDHR = byte 4..7");

    reset_fake();
    fake.TSR = BX_TSR_TME(1) | BX_TSR_TME(2);                 /* mailbox 0 sibuk */
    CHECK(bx_transmit(&fake, 0x0300FD22u, d, 8) && fake.TX[1].TIR != 0 && fake.TX[0].TIR == 0, "mailbox 0 sibuk -> pakai mailbox 1");

    reset_fake(); fake.TSR = 0;
    CHECK(!bx_transmit(&fake, 0x0300FD22u, d, 8),             "semua mailbox sibuk -> false");

    reset_fake(); fake.TSR = BX_TSR_TME(2);
    uint8_t short_d[3] = { 1, 2, 3 };
    CHECK(bx_transmit(&fake, 0x0000FD22u, short_d, 3) && (fake.TX[2].TDTR & 0xF) == 3 && fake.TX[2].TDLR == 0x00030201u, "DLC 3: hanya 3 byte terisi");

    /* RECEIVE */
    reset_fake();
    uint32_t id = 0; uint8_t rx[8] = {0}, dlc = 0;
    CHECK(!bx_receive(&fake, &id, rx, &dlc) && (fake.RF0R & BX_RF0R_RFOM0) == 0, "FIFO kosong -> false, tidak menyentuh RFOM0");

    fake.RF0R = 1;                                            /* FMP0 = 1 pesan */
    fake.RX[0].RIR  = (0x028422FDu << 3) | BX_TIR_IDE;
    fake.RX[0].RDTR = 8;
    fake.RX[0].RDLR = 0xFFFF0080u;                            /* byte0=80 byte1=00 byte2=FF byte3=FF */
    fake.RX[0].RDHR = 0x63010000u;                            /* byte4=00 byte5=00 byte6=01 byte7=63 */
    CHECK(bx_receive(&fake, &id, rx, &dlc),                   "receive: ada pesan");
    CHECK(id == 0x028422FDu && dlc == 8,                      "ID extended dan DLC terbaca benar");
    CHECK(rx[0] == 0x80 && rx[1] == 0x00 && rx[2] == 0xFF && rx[3] == 0xFF && rx[6] == 0x01 && rx[7] == 0x63, "data terbaca benar (little-endian register)");
    CHECK((fake.RF0R & BX_RF0R_RFOM0) != 0,                   "RFOM0 di-set agar FIFO maju");

    reset_fake();
    bx_enable_rx_irq(&fake);
    CHECK((fake.IER & BX_IER_FMPIE0) != 0,                    "bx_enable_rx_irq menyalakan FMPIE0");

    printf("\n%s (%d gagal)\n", failures == 0 ? "SEMUA LOLOS" : "ADA YANG GAGAL", failures);
    return failures != 0;
}
