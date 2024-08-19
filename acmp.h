/*
 * Analog comparator
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */

/*
 * Registers:
 * 
 *   ACSR: ACMP control and status register
 *     0,1 -> ACIS0,1: interrupt mode
 *     2 -> ACIC: input capture enable
 *     3 -> ACIE: interrupt enable
 *     4 -> ACI: interrupt flag
 *     5 -> ACO: output
 *     6 -> ACBG: bandgap select
 *     7 -> ACD: disable
 * 
 *   SFIOR: special function IO register
 *     0 -> PSR10: prescaler reset timer/counter1&0
 *     1 -> PSR2: prescaler Reset timer/counter2
 *     2 -> PUD: pull-up disable
 *     3 -> ACME: analog comparator multiplexer enable
 *     4 -> m8=ADHSM: ADC high speed mode
 *     5,6,7 -> ADTS0,1,2: ADC auto trigger source
 */

/* ACSR=ACD|ACBG|ACO|ACI|ACIE|ACIC|ACIS1|ACIS0 */
/* SFIOR=ADTS2|ADTS1|ADTS0|ADHSM|ACME|PUD|PSR2|PSR10 */


#ifndef _ACMP_H_
#define _ACMP_H_ 1


#include "util.h"


/* ACMP options (acmp_set) */
#define ACMP_OUTPUT        b1(ACO)                         /* enable output */
#define ACMP_BANDGAP_VREF  b1(ACBG)                        /* positive input to bandgap voltage reference */
#define ACMP_ADC_MUX       (b1(ACME)<<8)                   /* negative input to adc multiplexer */
#define ACMP_CAPTURE       b1(ACIC)                        /* input capture by timer/counter1 */
#define ACMP_INT_TOGGLE    (b1(ACIE)|b0(ACIS1)|b0(ACIS0))  /* interrupt on output toggle */
#define ACMP_INT_FALL      (b1(ACIE)|b1(ACIS1)|b0(ACIS0))  /* interrupt on output falling edge */
#define ACMP_INT_RISE      (b1(ACIE)|b1(ACIS1)|b1(ACIS0))  /* interrupt on output rising edge */


/* ACMP macros */
#define acmp_set(cnt)  {out(ACSR, (cnt)&0xFF); smi(SFIOR, (cnt)>>8);}  /* setup */
#define acmp_en()      cbi(ACSR, ACD)           /* enable */
#define acmp_di()      sbi(ACSR, ACD)           /* disable */
#define acmp_wait()    wait_set_bit(ACSR, ACI)  /* wait to signal */


/* ACMP ISR */
#define ISR_ACMP()  ISR(ANA_COMP_vect)


#ifdef _ACMP_H_TEST_

int main(void) {
    PORTB = 0;
    DDRB = ~0;

    acmp_set(ACMP_BANDGAP_VREF | ACMP_INT_TOGGLE);
    acmp_en();

    for (;;) {
        acmp_wait()
        PORTB ^= 1;
    }

    return 0;
}

ISR_ACMP() {
    PORTB ^= 2;
}

#endif /* _ACMP_H_TEST_ */


#endif /* _ACMP_H_ */

