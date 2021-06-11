/*
 * Timer/counter0
 * Copyright (C) 2013-2021 Tohid Jafarzadeh <tohid.jk@gmail.com>
 * License GNU GPLv2
 * 2021-06-11 BETA
 */

/**
 * Registers:
 * 
 *        TCCR0: timer/counter0 control register
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^ \+/ ^ \-+-/
 *   | |  |  |   +----- CS02,1,0: clock select
 *   | |  |  +--------- WGM01: waveform generation mode
 *   | |  +------------ COM01,0: compare match output mode
 *   | +--------------- WGM00: waveform generation mode
 *   +----------------- FOC0: force output compare
 * 
 *        TIFR: timer/counter interrupt flag register
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^ ^ ^ ^ ^ ^ ^
 *   | | | | | | | +--- TOV0: timer/counter0 overflow flag
 *   | | | | | | +----- OCF0: timer/counter0 output compare match flag
 *   | | | | | +------- TOV1: timer/counter1 overflow flag
 *   | | | | +--------- OCF1B: timer/counter1 output compare match B flag
 *   | | | +----------- OCF1A: timer/counter1 output compare match A flag
 *   | | +------------- ICF1: timer/counter1 input capture flag
 *   | +--------------- TOV2: timer/counter2 overflow flag
 *   +----------------- OCF2: timer/counter2 output compare match flag
 * 
 *        TIMSK: timer/counter interrupt mask register
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^ ^ ^ ^ ^ ^ ^
 *   | | | | | | | +--- TOIE0: timer/counter0 overflow interrupt enable
 *   | | | | | | +----- OCIE0: timer/counter0 output compare match interrupt enable
 *   | | | | | +------- TOIE1: timer/counter1 overflow interrupt enable
 *   | | | | +--------- OCIE1B: timer/counter1 output compare match B interrupt enable
 *   | | | +----------- OCIE1A: timer/counter1 output compare match A interrupt enable
 *   | | +------------- TICIE1: timer/counter1 input capture interrupt enable
 *   | +--------------- TOIE2: timer/counter2 overflow interrupt enable
 *   +----------------- OCIE2: timer/counter2 output compare match interrupt enable
 * 
 *   TCNT0: timer/counter0 register
 *   OCR0: timer/counter0 output compare register
 */

/*
 * Example:
 * 
 * #include <avr/io.h>
 * #include "timer0.h"
 * 
 * int main(void) {
 *   PORTB = 0;
 *   DDRB = ~0;
 * 
 *   timer0_set(TIMER0_CK_T0_FALL);
 *   timer0_value(0);
 * 
 *   for (;;) {
 *     PORTB = timer0_value_get();
 *   }
 * 
 *   return 0;
 * }
 */


#ifndef _TIMER0_H_
#define _TIMER0_H_ 1


#include "util.h"

/* TCCR0=FOC0|WGM00|COM01|COM00|WGM01|CS02|CS01|CS00 */
/* TIFR=OCF2|TOV2|ICF1|OCF1A|OCF1B|TOV1|OCF0|TOV0 */
/* TIMSK=OCIE2|TOIE2|TICIE1|OCIE1A|OCIE1B|TOIE1|OCIE0|TOIE0 */


/* timer/counter0 clock prescaler (timer0_set) */
#define TIMER0_CK_STOP     (b0(CS02)|b0(CS01)|b0(CS00))  /* clock stoped */
#define TIMER0_CK_DIV1     (b0(CS02)|b0(CS01)|b1(CS00))  /* F_CPU/1 */
#define TIMER0_CK_DIV8     (b0(CS02)|b1(CS01)|b0(CS00))  /* F_CPU/8 */
#define TIMER0_CK_DIV64    (b0(CS02)|b1(CS01)|b1(CS00))  /* F_CPU/64 */
#define TIMER0_CK_DIV256   (b1(CS02)|b0(CS01)|b0(CS00))  /* F_CPU/256 */
#define TIMER0_CK_DIV1024  (b1(CS02)|b0(CS01)|b1(CS00))  /* F_CPU/1024 */
#define TIMER0_CK_T0_FALL  (b1(CS02)|b1(CS01)|b0(CS00))  /* T0 pin falling edge */
#define TIMER0_CK_T0_RISE  (b1(CS02)|b1(CS01)|b1(CS00))  /* T0 pin rising edge */

/* timer/counter0 function mode (timer0_set) */
#ifdef OCR0
#define TIMER0_MODE_NORMAL             (b0(WGM00)|b0(WGM01))  /* normal timer/counter - top: 0xFF */
#define TIMER0_MODE_PHASE_CORRECT_PWM  (b1(WGM00)|b0(WGM01))  /* phase correct PWM generator - top: 0xFF */
#define TIMER0_MODE_CTC                (b0(WGM00)|b1(WGM01))  /* clear timer on compare match - top: OCR0 */
#define TIMER0_MODE_FAST_PWM           (b1(WGM00)|b1(WGM01))  /* fast PWM generator - top: 0xFF */
#endif /* OCR0 */

/* timer/counter0 OC0 output status (timer0_set) */
#ifdef OCR0
#define TIMER0_OC0_NC      (b0(COM01)|b0(COM00))  /* OC0 output disconnected */
#define TIMER0_OC0_TOGGLE  (b0(COM01)|b1(COM00))  /* OC0 output toggle on compare */
#define TIMER0_OC0_CLEAR   (b1(COM01)|b0(COM00))  /* OC0 output clear on compare */
#define TIMER0_OC0_SET     (b1(COM01)|b1(COM00))  /* OC0 output set on compare */
#endif /* OCR0 */

/* timer/counter0 interrupts (timer0_signal) */
#ifdef OCR0
#define TIMER0_INT_CMP  b1(OCIE0)  /* timer/counter0 compare match */
#endif /* OCR0 */
#define TIMER0_INT_OVF  b1(TOIE0)  /* timer/counter0 overflow */


/* timer/counter0 macro routines */
#define timer0_set(cnt)         out(TCCR0, cnt)           /* set timer/counter0 */
#define timer0_value(vlu)       out(TCNT0, vlu)           /* set timer/counter0 counted value */
#define timer0_signal(sgn)      smi(TIMSK, sgn)           /* timer/counter0 enable signals */
#define timer0_ovf_wait()       wait_set_bit(TIFR, TOV0)  /* wait ot timer/counter0 overflow signal */
#define timer0_di()             timer0_set(TIMER0_CK_STOP)  /* timer/counter0 disable */
#define timer0_value_get()      in(TCNT0)                 /* read timer/counter0 counted value */
#ifdef OCR0
#define timer0_compare(cmp)     out(OCR0, cmp)            /* set timer/counter0 compare match value */
#define timer0_cmp_wait()       wait_set_bit(TIFR, OCF0)  /* wait ot timer/counter0 compare match signal */
#define timer0_force_out_cmp()  sbi(TCCR0, FOC0)          /* force change OC0 - not suport in PWM modes */
#endif /* OCR0 */


#ifdef OCR0
/* timer/counter0 compare match interrupt service routine */
#define ISR_TIMER0_CMP()  ISR(TIMER0_COMP_vect)
#endif /* OCR0 */
/* timer/counter0 overflow interrupt service routine */
#define ISR_TIMER0_OVF()  ISR(TIMER0_OVF_vect)


#endif /* _TIMER0_H_ */

