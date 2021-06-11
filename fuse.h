/*
 * Fuses bits
 * Copyright (C) 2013-2021 Tohid Jafarzadeh <tohid.jk@gmail.com>
 * License GNU GPLv2
 * 2021-06-11 BETA
 */

/**
 * Fuses:
 * 
 *        LFUSE: low fuse byte
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^ \+/ \--+--/
 *   | |  |     +------ CKSEL3,2,1,0: select clock source
 *   | |  +------------ SUT1,0: select start-up time
 *   | +--------------- BODEN: brown-out detector enable
 *   +----------------- BODLEVEL: brown-out detector trigger level - if select this, brown-out reset on 4v, else on 2.7v
 * 
 *        HFUSE: high fuse byte
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^ ^ ^ ^ \+/ ^
 *   | | | | |  |  +--- BOOTRST: select reset vector - if select this, reset vector on boot size pointer, else on 0x0000
 *   | | | | |  +------ BOOTSZ1,0: select boot size
 *   | | | | +--------- EESAVE: eeprom memory is preserved through the chip erase
 *   | | | +----------- CKOPT: oscillator options - nois reduction if external clock source selected
 *   | | +------------- SPIEN: SPI serial program and data downloading enable
 *   | +--------------- m32,m16=JTAGEN: JTAG interface enable - m8=WDTON: watchdig timer always ON
 *   +----------------- m32,m16=OCDEN: on chip debug enable - m8=RSTDISBL: external reset disable
 */

/*
 * Example:
 * 
 * #include "fuse.h"
 * 
 * fuse_set(FUSE_CK_INT_8MHZ & FUSE_SUT0, HFUSE_DEFAULT);
 * 
 * int main(void) {
 *   for (;;);
 *   return 0;
 * }
 */


#ifndef _FUSE_H_
#define _FUSE_H_ 1


#include <avr/fuse.h>


/* fuses low byte select clock source (fuse_set(lfu)) */
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
#define FUSE_CK_EXT_MEDIOCRE_1  (FUSE_CKSEL0 & FUSE_CKSEL1)                /* external mediocre frequency crystal */
#define FUSE_CK_EXT_MEDIOCRE_2  (FUSE_CKSEL1)                              /* external mediocre frequency crystal */
#define FUSE_CK_EXT_HIGH_1      (FUSE_CKSEL0)                              /* external high frequency crystal */
#define FUSE_CK_EXT_HIGH_2      (~0)                                       /* external high frequency crystal */

/* fuses high byte select boot size (fuse_set(hfu)) */
#define FUSE_BOOT_SIZE_1  (FUSE_BOOTSZ0 & FUSE_BOOTSZ1)  /* boot loader memory size level 1 (minimum) */
#define FUSE_BOOT_SIZE_2  (FUSE_BOOTSZ1)                 /* boot loader memory size level 2 */
#define FUSE_BOOT_SIZE_3  (FUSE_BOOTSZ0)                 /* boot loader memory size level 3 */
#define FUSE_BOOT_SIZE_4  (~0)                           /* boot loader memory size level 4 (maximum) */


/* fuses bits macros */
#define fuse_set(lfu, hfu)  FUSES={(lfu),(hfu)}


#endif /* _FUSE_H_ */

