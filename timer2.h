/*
 * Timer/counter2
 * Copyright (C) 2013-2021 Tohid Jafarzadeh <tohid.jk@gmail.com>
 * License GNU GPLv2
 * 16.02.2021
 */

/**
 * Registers:
 *   
 *          ASSR: asynchronous status register
 *   /-------+-------\
 *   7 6 5 4   3 2 1 0
 *             ^ ^ ^ ^
 *             | | | +--- TCR2UB: timer/counter2 control register update busy
 *             | | +----- OCR2UB: timer/counter2 output compare register update busy
 *             | +------- TCN2UB : timer/counter2 update busy
 *             +--------- AS2: Timer/counter2 asynchronous clock select
 *   
 *         TCCR2: timer/counter2 control register
 *   /-------+-------\
 *   7 6 5 4   3 2 1 0
 *   ^ ^ \+/   ^ \-+-/
 *   | |  |    |   +----- CS22,1,0: clock select
 *   | |  |    +--------- WGM21: waveform generation mode
 *   | |  +-------------- COM21,0: compare output mode
 *   | +----------------- WGM20: waveform generation mode
 *   +------------------- FOC2: force output compare
 *   
 *          TIFR: timer/counter interrupt flag register
 *   /-------+-------\
 *   7 6 5 4   3 2 1 0
 *   ^ ^ ^ ^   ^ ^ ^ ^
 *   | | | |   | | | +--- TOV0: timer/counter0 overflow flag
 *   | | | |   | | +----- OCF0: timer/counter0 output compare match flag
 *   | | | |   | +------- TOV1: timer/counter1 overflow flag
 *   | | | |   +--------- OCF1B: timer/counter1 output compare match B flag
 *   | | | +------------- OCF1A: timer/counter1 output compare match A flag
 *   | | +--------------- ICF1: timer/counter1 input capture flag
 *   | +----------------- TOV2: timer/counter2 overflow flag
 *   +------------------- OCF2: timer/counter2 output compare match flag
 *   
 *         TIMSK: timer/counter interrupt mask register
 *   /-------+-------\
 *   7 6 5 4   3 2 1 0
 *   ^ ^ ^ ^   ^ ^ ^ ^
 *   | | | |   | | | +--- TOIE0: timer/counter0 overflow interrupt enable
 *   | | | |   | | +----- OCIE0: timer/counter0 output compare match interrupt enable
 *   | | | |   | +------- TOIE1: timer/counter1 overflow interrupt enable
 *   | | | |   +--------- OCIE1B: timer/counter1 output compare match B interrupt enable
 *   | | | +------------- OCIE1A: timer/counter1 output compare match A interrupt enable
 *   | | +--------------- TICIE1: timer/counter1 input capture interrupt enable
 *   | +----------------- TOIE2: timer/counter2 overflow interrupt enable
 *   +------------------- OCIE2: timer/counter2 output compare match interrupt enable
 *   
 *   TCNT2: timer/counter2 register
 *   OCR2: timer/counter2 output compare match register
 */

/*
 * Example:
 * 
 * #include <avr/io.h>
 * #include "timer2.h"
 * 
 * unsigned char i = 0;
 * 
 * int main(void) {
 *   PORTB = 0;
 *   DDRB = ~0;
 *   
 *   timer2_rtc_mode_en();
 *   timer2_set(TIMER2_CK_DIV128);
 *   timer2_value(0);
 *   timer2_compare(0);
 *   timer2_signal(TIMER2_INT_OVF);
 *   sei();
 *   
 *   for (;;) {
 *     PORTB = i;
 *   }
 *   
 *   return 0;
 * }
 * 
 * ISR_TIMER2_OVF() {
 *   i++;
 * }
 */


#ifndef _TIMER2_H_
#define _TIMER2_H_ 1


#include "util.h"

/* ASSR=||||AS2|TCN2UB|OCR2UB|TCR2UB */
/* TCCR2=FOC2|WGM20|COM21|COM20|WGM21|CS22|CS21|CS20 */
/* TIFR=OCF2|TOV2|ICF1|OCF1A|OCF1B|TOV1|OCF0|TOV0 */
/* TIMSK=OCIE2|TOIE2|TICIE1|OCIE1A|OCIE1B|TOIE1|OCIE0|TOIE0 */


/* timer/counter2 clock prescaler (timer2_set) */
#define TIMER2_CK_STOP     (b0(CS22)|b0(CS21)|b0(CS20))  /* clock stoped */
#define TIMER2_CK_DIV1     (b0(CS22)|b0(CS21)|b1(CS20))  /* F_CPU/1 or F_RTC/1 */
#define TIMER2_CK_DIV8     (b0(CS22)|b1(CS21)|b0(CS20))  /* F_CPU/8 or F_RTC/8 */
#define TIMER2_CK_DIV32    (b0(CS22)|b1(CS21)|b1(CS20))  /* F_CPU/32 or F_RTC/32 */
#define TIMER2_CK_DIV64    (b1(CS22)|b0(CS21)|b0(CS20))  /* F_CPU/64 or F_RTC/64 */
#define TIMER2_CK_DIV128   (b1(CS22)|b0(CS21)|b1(CS20))  /* F_CPU/128 or F_RTC/128 */
#define TIMER2_CK_DIV256   (b1(CS22)|b1(CS21)|b0(CS20))  /* F_CPU/256 or F_RTC/256 */
#define TIMER2_CK_DIV1024  (b1(CS22)|b1(CS21)|b1(CS20))  /* F_CPU/1024 or F_RTC/1024 */

/* timer/counter2 function mode (timer2_set) */
#define TIMER2_MODE_NORMAL             (b0(WGM20)|b0(WGM21))  /* normal timer/counter - top: 0xFF */
#define TIMER2_MODE_PHASE_CORRECT_PWM  (b1(WGM20)|b0(WGM21))  /* phase correct PWM generator - top: 0xFF */
#define TIMER2_MODE_CTC                (b0(WGM20)|b1(WGM21))  /* clear timer on compare match - top: OCR2 */
#define TIMER2_MODE_FAST_PWM           (b1(WGM20)|b1(WGM21))  /* fast PWM generator - top: 0xFF */

/* timer/counter2 OC2 output status (timer2_set) */
#define TIMER2_OC2_DISCONNECT  (b0(COM21)|b0(COM20))  /* OC2 output disconnected */
#define TIMER2_OC2_TOGGLE      (b0(COM21)|b1(COM20))  /* OC2 output toggle on compare */
#define TIMER2_OC2_CLEAR       (b1(COM21)|b0(COM20))  /* OC2 output clear on compare */
#define TIMER2_OC2_SET         (b1(COM21)|b1(COM20))  /* OC2 output set on compare */

/* timer/counter2 interrupts (timer2_signal) */
#define TIMER2_INT_CMP  b1(OCIE2)  /* timer/counter2 compare match */
#define TIMER2_INT_OVF  b1(TOIE2)  /* timer/counter2 overflow */


/* timer/counter2 macro routines */
#define timer2_rtc_mode_en()    sbi(ASSR, AS2)            /* enable Asynchronous RTC mode */
#define timer2_rtc_mode_di()    cbi(ASSR, AS2)            /* disable Asynchronous RTC mode */
#define timer2_set(cnt)         out(TCCR2, cnt)           /* set timer/counter2 */
#define timer2_value(vlu)       out(TCNT2, vlu)           /* set timer/counter2 counted value */
#define timer2_signal(sgn)      smi(TIMSK, sgn)           /* timer/counter2 enable signals */
#define timer2_compare(cmp)     out(OCR2, cmp)            /* set timer/counter2 compare match value */
#define timer2_cmp_wait()       wait_set_bit(TIFR, OCF2)  /* wait to timer/counter2 compare match signal */
#define timer2_ovf_wait()       wait_set_bit(TIFR, TOV2)  /* wait to timer/counter2 overflow signal */
#define timer2_di()             timer2_set(TIMER2_CK_STOP)  /* timer/counter2 disable */
#define timer2_force_out_cmp()  sbi(TCCR2, FOC2)            /* force change OC2 - not suport in PWM modes */
#define timer2_update_wait()    wait_set_mask(ASSR, b1(TCN2UB)|b1(OCR2UB)|b1(TCR2UB))  /* wait to timer/counter2 registers update busy */
#define timer2_value_get()      in(TCNT2)                   /* read timer/counter2 counted value */


/* timer/counter2 compare match interrupt service routine */
#define ISR_TIMER2_CMP()  ISR(TIMER2_COMP_vect)
/* timer/counter2 overflow interrupt service routine */
#define ISR_TIMER2_OVF()  ISR(TIMER2_OVF_vect)


#endif /* _TIMER2_H_ */

