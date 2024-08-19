/*
 * Fuses bits
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */

/*
 * Fuses:
 * 
 *   LFUSE: low fuse byte
 *     0,1,2,3 -> CKSEL0,1,2,3: select clock source
 *     4,5 -> SUT0,1: select start-up time
 *     6 -> BODEN: brown-out detector enable
 *     7 -> BODLEVEL: brown-out detector trigger level - if select this, brown-out reset on 4v, else on 2.7v
 * 
 *   HFUSE: high fuse byte
 *     0 -> BOOTRST: select reset vector - if select this, reset vector on boot size pointer, else on 0x0000
 *     1,2 -> BOOTSZ0,1: select boot size
 *     3 -> EESAVE: eeprom memory is preserved through the chip erase
 *     4 -> CKOPT: oscillator options - nois reduction if external clock source selected
 *     5 -> SPIEN: SPI serial program and data downloading enable
 *     6 -> m32,m16=JTAGEN: JTAG interface enable - m8=WDTON: watchdig timer always ON
 *     7 -> m32,m16=OCDEN: on chip debug enable - m8=RSTDISBL: external reset disable
 */


#ifndef _FUSE_H_
#define _FUSE_H_ 1


#include <avr/fuse.h>


/* LFUSE select clock source (fuse_set(lfu)) */
#define FUSE_CK_EXT             (FUSE_CKSEL0 & FUSE_CKSEL1 & FUSE_CKSEL2 & FUSE_CKSEL3)  /* external crystal */
#define FUSE_CK_INT_1MHZ        (FUSE_CKSEL1 & FUSE_CKSEL2 & FUSE_CKSEL3)  /* internal calibrated oscillator: 1MHz */
#define FUSE_CK_INT_2MHZ        (FUSE_CKSEL0 & FUSE_CKSEL2 & FUSE_CKSEL3)  /* internal calibrated oscillator: 2MHz */
#define FUSE_CK_INT_4MHZ        (FUSE_CKSEL2 & FUSE_CKSEL3)                /* internal calibrated oscillator: 4MHz */
#define FUSE_CK_INT_8MHZ        (FUSE_CKSEL0 & FUSE_CKSEL1 & FUSE_CKSEL3)  /* internal calibrated oscillator: 8MHz */
#define FUSE_CK_RC_9MHZ         (FUSE_CKSEL1 & FUSE_CKSEL3)                /* external rc: 9MHz */
#define FUSE_CK_RC_3MHZ_9MHZ    (FUSE_CKSEL0 & FUSE_CKSEL3)                /* external rc: 3MHz ~ 9MHz */
#define FUSE_CK_RC_3MHZ_8MHZ    (FUSE_CKSEL3)                              /* external rc: 3MHz ~ 8MHz */
#define FUSE_CK_RC_8MHZ_12MHZ   (FUSE_CKSEL0 & FUSE_CKSEL1 & FUSE_CKSEL2)  /* external rc: 8MHz ~ 12MHz */
#define FUSE_CK_EXT_LOW_1       (FUSE_CKSEL1 & FUSE_CKSEL2)                /* external low frequency crystal */
#define FUSE_CK_EXT_LOW_2       (FUSE_CKSEL0 & FUSE_CKSEL2)                /* external low frequency crystal */
#define FUSE_CK_EXT_LOW_3       (FUSE_CKSEL2)                              /* external low frequency crystal */
#define FUSE_CK_EXT_MEDIOCRE_1  (FUSE_CKSEL0 & FUSE_CKSEL1)                /* external middle frequency crystal */
#define FUSE_CK_EXT_MEDIOCRE_2  (FUSE_CKSEL1)                              /* external middle frequency crystal */
#define FUSE_CK_EXT_HIGH_1      (FUSE_CKSEL0)                              /* external high frequency crystal */
#define FUSE_CK_EXT_HIGH_2      (~0)                                       /* external high frequency crystal */

/* HFUSE select boot loader memory size (fuse_set(hfu)) */
#define FUSE_BOOT_SIZE_1  (FUSE_BOOTSZ0 & FUSE_BOOTSZ1)  /* 1 (minimum) */
#define FUSE_BOOT_SIZE_2  (FUSE_BOOTSZ1)                 /* 2 */
#define FUSE_BOOT_SIZE_3  (FUSE_BOOTSZ0)                 /* 3 */
#define FUSE_BOOT_SIZE_4  (~0)                           /* 4 (maximum) */


/* fuses bits macros */
#define fuse_set(lfu, hfu)  FUSES={(lfu),(hfu)}


#ifdef _FUSE_H_TEST_

fuse_set(FUSE_CK_INT_8MHZ & FUSE_SUT0, HFUSE_DEFAULT);

int main(void) {
    for (;;);
    return 0;
}

#endif /* _FUSE_H_TEST_ */


#endif /* _FUSE_H_ */

