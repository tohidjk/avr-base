/*
 * Other controls and statuss
 * Copyright (C) 2013-2021 Tohid Jafarzadeh <tohid.jk@gmail.com>
 * License GNU GPLv2
 * 2021-06-12 BETA
 */

/**
 * Registers:
 * 
 *        SREG: status register
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^ ^ ^ ^ ^ ^ ^
 *   | | | | | | | +--- SREG_C: carry flag
 *   | | | | | | +----- SREG_Z: zero flag
 *   | | | | | +------- SREG_N: negative flag
 *   | | | | +--------- SREG_V: two's complement overflow flag
 *   | | | +----------- SREG_S: sign bit
 *   | | +------------- SREG_H: half carry flag
 *   | +--------------- SREG_T: bit copy storage
 *   +----------------- SREG_I: global interrupt enable
 * 
 *       MCUCSR: MCU control and status register
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^   ^ ^ ^ ^ ^
 *   | |   | | | | +--- PORF: power-on reset flag
 *   | |   | | | +----- EXTRF: external reset flag
 *   | |   | | +------- BORF: brown-out reset flag
 *   | |   | +--------- WDRF: watchdog reset flag
 *   | |   +----------- JTRF: JTAG reset flag
 *   | +--------------- ISC2: external interrupt 2 sense control
 *   +----------------- JTD: JTAG interface disable
 * 
 *        SFIOR: special function IO register
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   \-+-/ ^ ^ ^ ^ ^
 *     |   | | | | +--- PSR10: prescaler reset timer/counter1&0
 *     |   | | | +----- PSR2: prescaler Reset timer/counter2
 *     |   | | +------- PUD: pull-up disable
 *     |   | +--------- ACME: analog comparator multiplexer enable
 *     |   +----------- m8=ADHSM: ADC high speed mode
 *     +--------------- ADTS2,1,0: ADC auto trigger source
 * 
 *   OSCCAL: oscillator calibration value
 */


#ifndef _OTHER_H_
#define _OTHER_H_ 1


#include "util.h"

/* SREG=SREG_C|SREG_Z|SREG_N|SREG_V|SREG_S|SREG_H|SREG_T|SREG_I */
/* MCUCSR=JTD|ISC2||JTRF|WDRF|BORF|EXTRF|PORF */
/* SFIOR=ADTS2|ADTS1|ADTS0|ADHSM|ACME|PUD|PSR2|PSR10 */


/* other controls and statuss */
#define osc_calibr(vlu)     out(OSCCAL, vlu)   /* oscillator calibration value (debug) */
#define pullup_en()         cbi(SFIOR, PUD)    /* pull-up enable */
#define pullup_di()         sbi(SFIOR, PUD)    /* pull-up disable */
#define timer01_ck_reset()  sbi(SFIOR, PSR10)  /* timer/counter0,1 prescaler reset */
#define timer2_ck_reset()   sbi(SFIOR, PSR2)   /* timer/counter2 prescaler reset */
#define signal_en()         sbi(SREG, SREG_I)  /* global interrupt enable */
#define signal_di()         cbi(SREG, SREG_I)  /* global interrupt disable */
#ifdef ADHSM
#define adc_highspeed_en()  sbi(SFIOR, ADHSM)  /* adc high speed mode enable */
#define adc_highspeed_di()  cbi(SFIOR, ADHSM)  /* adc high speed mode disable */
#endif /* ADHSM */
#ifdef JTD
#define jtag_en()           cbi(MCUCSR, JTD)   /* JTAG Interface enable */
#define jtag_di()           sbi(MCUCSR, JTD)   /* JTAG Interface disable */
#endif /* JTD */


#endif /* _OTHER_H_ */

