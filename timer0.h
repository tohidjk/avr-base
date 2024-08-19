/*
 * Timer/counter0
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */

/*
 * Registers:
 * 
 *   TCCR0: timer/counter0 control register
 *     0,1,2 -> CS00,1,2: clock select
 *     3 -> WGM01: waveform generation mode
 *     4,5 -> COM00,1: compare match output mode
 *     6 -> WGM00: waveform generation mode
 *     7 -> FOC0: force output compare
 * 
 *   TIFR: timer/counter interrupt flag register
 *     0 -> TOV0: timer/counter0 overflow flag
 *     1 -> OCF0: timer/counter0 output compare match flag
 *     2 -> TOV1: timer/counter1 overflow flag
 *     3 -> OCF1B: timer/counter1 output compare match B flag
 *     4 -> OCF1A: timer/counter1 output compare match A flag
 *     5 -> ICF1: timer/counter1 input capture flag
 *     6 -> TOV2: timer/counter2 overflow flag
 *     7 -> OCF2: timer/counter2 output compare match flag
 * 
 *   TIMSK: timer/counter interrupt mask register
 *     0 -> TOIE0: timer/counter0 overflow interrupt enable
 *     1 -> OCIE0: timer/counter0 output compare match interrupt enable
 *     2 -> TOIE1: timer/counter1 overflow interrupt enable
 *     3 -> OCIE1B: timer/counter1 output compare match B interrupt enable
 *     4 -> OCIE1A: timer/counter1 output compare match A interrupt enable
 *     5 -> TICIE1: timer/counter1 input capture interrupt enable
 *     6 -> TOIE2: timer/counter2 overflow interrupt enable
 *     7 -> OCIE2: timer/counter2 output compare match interrupt enable
 * 
 *   TCNT0: timer/counter0 register
 *   OCR0: timer/counter0 output compare register
 */

/* TCCR0=FOC0|WGM00|COM01|COM00|WGM01|CS02|CS01|CS00 */
/* TIFR=OCF2|TOV2|ICF1|OCF1A|OCF1B|TOV1|OCF0|TOV0 */
/* TIMSK=OCIE2|TOIE2|TICIE1|OCIE1A|OCIE1B|TOIE1|OCIE0|TOIE0 */


#ifndef _TIMER0_H_
#define _TIMER0_H_ 1


#include "util.h"


/* TC0 clock prescaler (timer0_set) */
#define TIMER0_CK_STOP     (b0(CS02)|b0(CS01)|b0(CS00))  /* clock stoped */
#define TIMER0_CK_DIV1     (b0(CS02)|b0(CS01)|b1(CS00))  /* F_CPU/1 */
#define TIMER0_CK_DIV8     (b0(CS02)|b1(CS01)|b0(CS00))  /* F_CPU/8 */
#define TIMER0_CK_DIV64    (b0(CS02)|b1(CS01)|b1(CS00))  /* F_CPU/64 */
#define TIMER0_CK_DIV256   (b1(CS02)|b0(CS01)|b0(CS00))  /* F_CPU/256 */
#define TIMER0_CK_DIV1024  (b1(CS02)|b0(CS01)|b1(CS00))  /* F_CPU/1024 */
#define TIMER0_CK_T0_FALL  (b1(CS02)|b1(CS01)|b0(CS00))  /* T0 pin falling edge */
#define TIMER0_CK_T0_RISE  (b1(CS02)|b1(CS01)|b1(CS00))  /* T0 pin rising edge */

/* TC0 function mode (timer0_set) */
#ifdef OCR0
#define TIMER0_MODE_NORMAL             (b0(WGM00)|b0(WGM01))  /* normal - top: 0xFF */
#define TIMER0_MODE_PHASE_CORRECT_PWM  (b1(WGM00)|b0(WGM01))  /* phase correct PWM generator - top: 0xFF */
#define TIMER0_MODE_CTC                (b0(WGM00)|b1(WGM01))  /* clear timer on compare match - top: OCR0 */
#define TIMER0_MODE_FAST_PWM           (b1(WGM00)|b1(WGM01))  /* fast PWM generator - top: 0xFF */
#endif /* OCR0 */

/* TC0 OC0 output status (timer0_set) */
#ifdef OCR0
#define TIMER0_OC0_NC      (b0(COM01)|b0(COM00))  /* disconnected */
#define TIMER0_OC0_TOGGLE  (b0(COM01)|b1(COM00))  /* toggle on compare */
#define TIMER0_OC0_CLEAR   (b1(COM01)|b0(COM00))  /* clear on compare */
#define TIMER0_OC0_SET     (b1(COM01)|b1(COM00))  /* set on compare */
#endif /* OCR0 */

/* TC0 interrupts (timer0_signal) */
#ifdef OCR0
#define TIMER0_INT_CMP  b1(OCIE0)  /* compare match */
#endif /* OCR0 */
#define TIMER0_INT_OVF  b1(TOIE0)  /* overflow */


/* TC0 macros */
#define timer0_set(cnt)         out(TCCR0, cnt)           /* setup */
#define timer0_value(vlu)       out(TCNT0, vlu)           /* set counted value */
#define timer0_signal(sgn)      smi(TIMSK, sgn)           /* enable signals */
#define timer0_ovf_wait()       wait_set_bit(TIFR, TOV0)  /* wait ot overflow signal */
#define timer0_di()             timer0_set(TIMER0_CK_STOP) /* disable */
#define timer0_value_get()      in(TCNT0)                 /* read counted value */
#ifdef OCR0
#define timer0_compare(cmp)     out(OCR0, cmp)            /* set compare match value */
#define timer0_cmp_wait()       wait_set_bit(TIFR, OCF0)  /* wait ot compare match signal */
#define timer0_force_out_cmp()  sbi(TCCR0, FOC0)          /* force change OC0 - not suport in PWM modes */
#endif /* OCR0 */


#ifdef OCR0
/* TC0 compare match ISR */
#define ISR_TIMER0_CMP()  ISR(TIMER0_COMP_vect)
#endif /* OCR0 */
/* TC0 overflow ISR */
#define ISR_TIMER0_OVF()  ISR(TIMER0_OVF_vect)


#ifdef _TIMER0_H_TEST_

int main(void) {
    PORTB = 0;
    DDRB = ~0;

    timer0_set(TIMER0_CK_T0_FALL);
    timer0_value(0);

    for (;;) {
        PORTB = timer0_value_get();
    }

    return 0;
}

#endif /* _TIMER0_H_TEST_ */


#endif /* _TIMER0_H_ */

