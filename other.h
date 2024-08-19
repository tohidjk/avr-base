/*
 * Other controls and statuss
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */

/*
 * Registers:
 * 
 *   SREG: status register
 *     0 -> SREG_C: carry flag
 *     1 -> SREG_Z: zero flag
 *     2 -> SREG_N: negative flag
 *     3 -> SREG_V: two's complement overflow flag
 *     4 -> SREG_S: sign bit
 *     5 -> SREG_H: half carry flag
 *     6 -> SREG_T: bit copy storage
 *     7 -> SREG_I: global interrupt enable
 * 
 *   MCUCSR: MCU control and status register
 *     0 -> PORF: power-on reset flag
 *     1 -> EXTRF: external reset flag
 *     2 -> BORF: brown-out reset flag
 *     3 -> WDRF: watchdog reset flag
 *     4 -> JTRF: JTAG reset flag
 *     6 -> ISC2: external interrupt 2 sense control
 *     7 -> JTD: JTAG interface disable
 * 
 *   SFIOR: special function IO register
 *     0 -> PSR10: prescaler reset timer/counter1&0
 *     1 -> PSR2: prescaler Reset timer/counter2
 *     2 -> PUD: pull-up disable
 *     3 -> ACME: analog comparator multiplexer enable
 *     4 -> m8=ADHSM: ADC high speed mode
 *     5,6,7 -> ADTS0,1,2: ADC auto trigger source
 * 
 *   OSCCAL: oscillator calibration value
 */

/* SREG=SREG_C|SREG_Z|SREG_N|SREG_V|SREG_S|SREG_H|SREG_T|SREG_I */
/* MCUCSR=JTD|ISC2||JTRF|WDRF|BORF|EXTRF|PORF */
/* SFIOR=ADTS2|ADTS1|ADTS0|ADHSM|ACME|PUD|PSR2|PSR10 */


#ifndef _OTHER_H_
#define _OTHER_H_ 1


#include "util.h"


/* power-on reset flag */
#ifdef PORF
#define RESET_PORF  b1(PORF)
#else /* !PORF */
#define RESET_PORF  (0)
#endif /* PORF */

/* external reset flag */
#ifdef EXTRF
#define RESET_EXTRF  b1(EXTRF)
#else /* !EXTRF */
#define RESET_EXTRF  (0)
#endif /* EXTRF */

/* brown-out reset flag */
#ifdef BORF
#define RESET_BORF  b1(BORF)
#else /* !BORF */
#define RESET_BORF  (0)
#endif /* BORF */

/* watchdog reset flag */
#ifdef WDRF
#define RESET_WDRF  b1(WDRF)
#else /* !WDRF */
#define RESET_WDRF  (0)
#endif /* WDRF */

/* JTAG reset flag */
#ifdef JTRF
#define RESET_JTRF  b1(JTRF)
#else /* !JTRF */
#define RESET_JTRF  (0)
#endif /* JTRF */


/* other controls and status */
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
#define reset_check()  mis(MCUCSR, RESET_PORF|RESET_EXTRF|RESET_BORF|RESET_WDRF|RESET_JTRF)  /* check reset source */
#define reset_clear()  cmi(MCUCSR, RESET_PORF|RESET_EXTRF|RESET_BORF|RESET_WDRF|RESET_JTRF)  /* clear reset value for next use */


#ifdef _OTHER_H_TEST_

#if !RESET_WDRF
    #warning "RESET_WDRF"
#endif

int main(void) {
    PORTB = 0;
    DDRB = ~0;

    if (reset_check() == RESET_WDRF)
        PORTB = 1;
    reset_clear();

    for (;;);

    return 0;
}

#endif /* _OTHER_H_TEST_ */


#endif /* _OTHER_H_ */

