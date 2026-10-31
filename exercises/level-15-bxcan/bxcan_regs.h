#ifndef BXCAN_REGS_H
#define BXCAN_REGS_H

/* Peta register bxCAN STM32F103 (RM0008, bab "CAN registers").
 * Struct ini DIBERIKAN. Tugasmu: mengerti arti tiap field dan memakainya di bxcan.c.
 * Alamat dasar CAN1 = 0x40006400 (di mikrokontroler), tetapi semua fungsi menerima POINTER
 * sehingga di laptop kita bisa menunjuk ke memori palsu untuk pengujian. */

#include <stdint.h>
#include <stddef.h>

typedef struct { volatile uint32_t TIR, TDTR, TDLR, TDHR; } BxTxMailbox;
typedef struct { volatile uint32_t RIR, RDTR, RDLR, RDHR; } BxRxMailbox;
typedef struct { volatile uint32_t FR1, FR2; }              BxFilterReg;

typedef struct {
    volatile uint32_t MCR;        /* 0x000 master control         */
    volatile uint32_t MSR;        /* 0x004 master status          */
    volatile uint32_t TSR;        /* 0x008 transmit status        */
    volatile uint32_t RF0R;       /* 0x00C receive FIFO 0         */
    volatile uint32_t RF1R;       /* 0x010 receive FIFO 1         */
    volatile uint32_t IER;        /* 0x014 interrupt enable       */
    volatile uint32_t ESR;        /* 0x018 error status           */
    volatile uint32_t BTR;        /* 0x01C bit timing             */
    uint32_t          RESERVED0[88];
    BxTxMailbox       TX[3];      /* 0x180 mailbox kirim 0..2     */
    BxRxMailbox       RX[2];      /* 0x1B0 mailbox terima FIFO 0,1*/
    uint32_t          RESERVED1[12];
    volatile uint32_t FMR;        /* 0x200 filter master          */
    volatile uint32_t FM1R;       /* 0x204 filter mode (0 mask)   */
    uint32_t          RESERVED2;
    volatile uint32_t FS1R;       /* 0x20C filter scale (1 = 32b) */
    uint32_t          RESERVED3;
    volatile uint32_t FFA1R;      /* 0x214 filter -> FIFO         */
    uint32_t          RESERVED4;
    volatile uint32_t FA1R;       /* 0x21C filter activation      */
    uint32_t          RESERVED5[8];
    BxFilterReg       FILTER[28]; /* 0x240 bank filter            */
} BxCan;

/* Bukti di waktu kompilasi bahwa susunan struct cocok dengan datasheet */
_Static_assert(offsetof(BxCan, BTR)    == 0x01C, "BTR harus di 0x01C");
_Static_assert(offsetof(BxCan, TX)     == 0x180, "TX mailbox harus di 0x180");
_Static_assert(offsetof(BxCan, RX)     == 0x1B0, "RX mailbox harus di 0x1B0");
_Static_assert(offsetof(BxCan, FMR)    == 0x200, "FMR harus di 0x200");
_Static_assert(offsetof(BxCan, FA1R)   == 0x21C, "FA1R harus di 0x21C");
_Static_assert(offsetof(BxCan, FILTER) == 0x240, "FILTER harus di 0x240");

#define BX_CAN1_BASE   0x40006400UL

/* MCR */
#define BX_MCR_INRQ    (1u << 0)
#define BX_MCR_SLEEP   (1u << 1)
#define BX_MCR_NART    (1u << 4)    /* 1 = TIDAK mengirim ulang otomatis */
/* MSR */
#define BX_MSR_INAK    (1u << 0)
#define BX_MSR_SLAK    (1u << 1)
/* TSR: mailbox kosong */
#define BX_TSR_TME(i)  (1u << (26 + (i)))
/* RF0R */
#define BX_RF0R_FMP0   (0x3u)
#define BX_RF0R_RFOM0  (1u << 5)
/* IER */
#define BX_IER_FMPIE0  (1u << 1)
/* BTR */
#define BX_BTR_BRP(v)  ((uint32_t)(v) << 0)    /* 10 bit, nilai = prescaler - 1 */
#define BX_BTR_TS1(v)  ((uint32_t)(v) << 16)   /*  4 bit, nilai = BS1 - 1       */
#define BX_BTR_TS2(v)  ((uint32_t)(v) << 20)   /*  3 bit, nilai = BS2 - 1       */
#define BX_BTR_SJW(v)  ((uint32_t)(v) << 24)   /*  2 bit, nilai = SJW - 1       */
#define BX_BTR_LBKM    (1u << 30)
#define BX_BTR_SILM    (1u << 31)
/* TIR / RIR */
#define BX_TIR_TXRQ    (1u << 0)
#define BX_TIR_RTR     (1u << 1)
#define BX_TIR_IDE     (1u << 2)
#define BX_TIR_EXID_SHIFT 3
/* FMR */
#define BX_FMR_FINIT   (1u << 0)

#endif
