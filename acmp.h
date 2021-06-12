/*
 * Analog comparator
 * Copyright (C) 2013-2021 Tohid Jafarzadeh <tohid.jk@gmail.com>
 * License GNU GPLv2
 * 2021-06-12 BETA
 */

/**
 * Registers:
 * 
 *        ACSR: analog comparator control and status register
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^ ^ ^ ^ ^ \+/
 *   | | | | | |  +---- ACIS1,0: interrupt mode
 *   | | | | | +------- ACIC: input capture enable
 *   | | | | +--------- ACIE: interrupt enable
 *   | | | +----------- ACI: interrupt flag
 *   | | +------------- ACO: output
 *   | +--------------- ACBG: bandgap select
 *   +----------------- ACD: disable
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
 */

/*
 * Example:
 * 
 * #include <avr/io.h>
 * #include "acmp.h"
 * 
 * int main(void) {
 *   PORTB = 0;
 *   DDRB = ~0;
 * 
 *   acmp_set(ACMP_BANDGAP_VREF | ACMP_INT_TOGGLE);
 *   acmp_en();
 * 
 *   for (;;) {
 *     acmp_wait()
 *     PORTB ^= 1;
 *   }
 * 
 *   return 0;
 * }
 * 
 * ISR_ACMP() {
 *   PORTB ^= 2;
 * }
 */


#ifndef _ACMP_H_
#define _ACMP_H_ 1


#include "util.h"

/* ACSR=ACD|ACBG|ACO|ACI|ACIE|ACIC|ACIS1|ACIS0 */
/* SFIOR=ADTS2|ADTS1|ADTS0|ADHSM|ACME|PUD|PSR2|PSR10 */


/* analog comparator options (acmp_set) */
#define ACMP_OUTPUT        b1(ACO)                         /* enable output */
#define ACMP_BANDGAP_VREF  b1(ACBG)                        /* positive input to bandgap voltage reference */
#define ACMP_ADC_MUX       (b1(ACME)<<8)                   /* negative input to adc multiplexer */
#define ACMP_CAPTURE       b1(ACIC)                        /* input capture by timer/counter1 */
#define ACMP_INT_TOGGLE    (b1(ACIE)|b0(ACIS1)|b0(ACIS0))  /* interrupt on output toggle */
#define ACMP_INT_FALL      (b1(ACIE)|b1(ACIS1)|b0(ACIS0))  /* interrupt on output falling edge */
#define ACMP_INT_RISE      (b1(ACIE)|b1(ACIS1)|b1(ACIS0))  /* interrupt on output rising edge */


/* analog comparator macro routines */
#define acmp_set(cnt)  {out(ACSR, cnt); smi(SFIOR, (cnt)>>8);}  /* set analog comparator */
#define acmp_en()      cbi(ACSR, ACD)           /* analog comparator enable */
#define acmp_di()      sbi(ACSR, ACD)           /* analog comparator disable */
#define acmp_wait()    wait_set_bit(ACSR, ACI)  /* wait to analog comparator signal */


/* analog comparator interrupt service routine */
#define ISR_ACMP()  ISR(ANA_COMP_vect)


#endif /* _ACMP_H_ */

