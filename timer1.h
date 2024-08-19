/*
 * Timer/counter1
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */

/*
 * Registers:
 * 
 *   TCCR1B: timer/counter1 control register B
 *     0,1,2 -> CS10,1,2: clock select
 *     3,4 -> WGM12,3: waveform generation mode
 *     6 -> ICES1: input capture edge select
 *     7 -> ICNC1: input capture noise canceler
 * 
 *    TCCR1A: timer/counter1 control register A
 *      0,1 -> WGM10,1: waveform generation mode
 *      2 -> FOC1B: force output compare B
 *      3 -> FOC1A: force output compare A
 *      4,5 -> COM1B0,1: compare output mode B
 *      6,7 -> COM1A0,1: compare output mode A
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
 *   TCNT1=TCNT1H+TCNT1L: timer/counter1 register
 *   ICR1=ICR1H+ICR1L: timer/counter1 input capture register
 *   OCR1B=OCR1BH+OCR1BL: timer/counter1 output compare B register
 *   OCR1A=OCR1AH+OCR1AL: timer/counter1 output compare A register
 */

/* TCCR1B=ICNC1|ICES1||WGM13|WGM12|CS12|CS11|CS10 */
/* TCCR1A=COM1A1|COM1A0|COM1B1|COM1B0|FOC1A|FOC1B|WGM11|WGM10 */
/* TIFR=OCF2|TOV2|ICF1|OCF1A|OCF1B|TOV1|OCF0|TOV0 */
/* TIMSK=OCIE2|TOIE2|TICIE1|OCIE1A|OCIE1B|TOIE1|OCIE0|TOIE0 */


#ifndef _TIMER1_H_
#define _TIMER1_H_ 1


#include "util.h"


/* TC1 clock prescaler (timer1_set) */
#define TIMER1_CK_STOP     (b0(CS12)|b0(CS11)|b0(CS10))  /* clock stoped */
#define TIMER1_CK_DIV1     (b0(CS12)|b0(CS11)|b1(CS10))  /* F_CPU/1 */
#define TIMER1_CK_DIV8     (b0(CS12)|b1(CS11)|b0(CS10))  /* F_CPU/8 */
#define TIMER1_CK_DIV64    (b0(CS12)|b1(CS11)|b1(CS10))  /* F_CPU/64 */
#define TIMER1_CK_DIV256   (b1(CS12)|b0(CS11)|b0(CS10))  /* F_CPU/256 */
#define TIMER1_CK_DIV1024  (b1(CS12)|b0(CS11)|b1(CS10))  /* F_CPU/1024 */
#define TIMER1_CK_T1_FALL  (b1(CS12)|b1(CS11)|b0(CS10))  /* T1 pin falling edge */
#define TIMER1_CK_T1_RISE  (b1(CS12)|b1(CS11)|b1(CS10))  /* T1 pin rising edge */

/* TC1 function mode (timer1_set) */
#define TIMER1_MODE_NORMAL                       (((b0(WGM11)|b0(WGM10))<<8)|b0(WGM13)|b0(WGM12))  /* normal - top: 0xFFFF */
#define TIMER1_MODE_PHASE_CORRECT_PWM_8BIT       (((b0(WGM11)|b1(WGM10))<<8)|b0(WGM13)|b0(WGM12))  /* 8bit phase correct PWM generator - top: 0x00FF */
#define TIMER1_MODE_PHASE_CORRECT_PWM_9BIT       (((b1(WGM11)|b0(WGM10))<<8)|b0(WGM13)|b0(WGM12))  /* 9bit phase correct PWM generator - top: 0x01FF */
#define TIMER1_MODE_PHASE_CORRECT_PWM_10BIT      (((b1(WGM11)|b1(WGM10))<<8)|b0(WGM13)|b0(WGM12))  /* 10bit phase correct PWM generator - top: 0x03FF */
#define TIMER1_MODE_CTC_CMPA                     (((b0(WGM11)|b0(WGM10))<<8)|b0(WGM13)|b1(WGM12))  /* clear timer on compare match - top: OCR1A */
#define TIMER1_MODE_FAST_PWM_8BIT                (((b0(WGM11)|b1(WGM10))<<8)|b0(WGM13)|b1(WGM12))  /* 8bit fast PWM generator - top: 0x00FF */
#define TIMER1_MODE_FAST_PWM_9BIT                (((b1(WGM11)|b0(WGM10))<<8)|b0(WGM13)|b1(WGM12))  /* 9bit fast PWM generator - top: 0x01FF */
#define TIMER1_MODE_FAST_PWM_10BIT               (((b1(WGM11)|b1(WGM10))<<8)|b0(WGM13)|b1(WGM12))  /* 10bit fast PWM generator - top: 0x03FF */
#define TIMER1_MODE_PHASE_FREQ_CORRECT_PWM_CAPT  (((b0(WGM11)|b0(WGM10))<<8)|b1(WGM13)|b0(WGM12))  /* phase and frequency correct PWM generator - top: ICR1 */
#define TIMER1_MODE_PHASE_FREQ_CORRECT_PWM_CMPA  (((b0(WGM11)|b1(WGM10))<<8)|b1(WGM13)|b0(WGM12))  /* phase and frequency correct PWM generator - top: OCR1A */
#define TIMER1_MODE_PHASE_CORRECT_PWM_CAPT       (((b1(WGM11)|b0(WGM10))<<8)|b1(WGM13)|b0(WGM12))  /* phase correct PWM generator - top: ICR1 */
#define TIMER1_MODE_PHASE_CORRECT_PWM_CMPA       (((b1(WGM11)|b1(WGM10))<<8)|b1(WGM13)|b0(WGM12))  /* phase correct PWM generator - top: OCR1A */
#define TIMER1_MODE_CTC_CAPT                     (((b0(WGM11)|b0(WGM10))<<8)|b1(WGM13)|b1(WGM12))  /* clear timer on compare match - top: ICR1 */
#define TIMER1_MODE_FAST_PWM_CAPT                (((b1(WGM11)|b0(WGM10))<<8)|b1(WGM13)|b1(WGM12))  /* fast PWM generator - top: ICR1 */
#define TIMER1_MODE_FAST_PWM_CMPA                (((b1(WGM11)|b1(WGM10))<<8)|b1(WGM13)|b1(WGM12))  /* fast PWM generator - top: OCR1A */

/* TC1 OC1A and OC1B output status (timer1_set) */
#define TIMER1_OC1A_NC      ((b0(COM1A1)|b0(COM1A0))<<8)  /* A disconnected */
#define TIMER1_OC1A_TOGGLE  ((b0(COM1A1)|b1(COM1A0))<<8)  /* A toggle on compare */
#define TIMER1_OC1A_CLEAR   ((b1(COM1A1)|b0(COM1A0))<<8)  /* A clear on compare */
#define TIMER1_OC1A_SET     ((b1(COM1A1)|b1(COM1A0))<<8)  /* A set on compare */
#define TIMER1_OC1B_NC      ((b0(COM1B1)|b0(COM1B0))<<8)  /* B disconnected */
#define TIMER1_OC1B_TOGGLE  ((b0(COM1B1)|b1(COM1B0))<<8)  /* B toggle on compare */
#define TIMER1_OC1B_CLEAR   ((b1(COM1B1)|b0(COM1B0))<<8)  /* B clear on compare */
#define TIMER1_OC1B_SET     ((b1(COM1B1)|b1(COM1B0))<<8)  /* B set on compare */

/* TC1 input capture options (timer1_set) */
#define TIMER1_CAPTURE_NOISE_CANCELER  b1(ICNC1)  /* noise canceler */
#define TIMER1_CAPTURE_FALLING_EDGE    b0(ICES1)  /* falling edge */
#define TIMER1_CAPTURE_RISING_EDGE     b1(ICES1)  /* rising edge */

/* TC1 interrupts (timer1_signal) */
#define TIMER1_INT_CAPT  b1(TICIE1)  /* capture event */
#define TIMER1_INT_CMPA  b1(OCIE1A)  /* compare match A */
#define TIMER1_INT_CMPB  b1(OCIE1B)  /* compare match B */
#define TIMER1_INT_OVF   b1(TOIE1)   /* Overflow */


/* TC1 macros */
#define timer1_set(cnt)          {out(TCCR1B, (cnt)&0xFF); out(TCCR1A, (cnt)>>8);}  /* setup */
#define timer1_value(vlu)        out(TCNT1, vlu)             /* set counted value */
#define timer1_signal(sgn)       smi(TIMSK, sgn)             /* enable signals */
#define timer1_capture(cpt)      out(ICR1, cpt)              /* set capture value */
#define timer1_compareA(cmp)     out(OCR1A, cmp)             /* set compare match A value */
#define timer1_compareB(cmp)     out(OCR1B, cmp)             /* set compare match B value */
#define timer1_capt_wait()       wait_set_bit(TIFR, ICF1)    /* wait to capture signal */
#define timer1_cmpA_wait()       wait_set_bit(TIFR, OCF1A)   /* wait to compare match A signal */
#define timer1_cmpB_wait()       wait_set_bit(TIFR, OCF1B)   /* wait to compare match B signal */
#define timer1_ovf_wait()        wait_set_bit(TIFR, TOV1)    /* wait to overflow signal */
#define timer1_di()              timer1_set(TIMER1_CK_STOP)  /* disable */
#define timer1_force_out_cmpA()  sbi(TCCR1A, FOC1A)          /* force change OC1A - not suport in PWM modes */
#define timer1_force_out_cmpB()  sbi(TCCR1A, FOC1B)          /* force change OC1BS - not suport in PWM modes */
#define timer1_value_get()       in(TCNT1)                   /* read counted value */


/* TC1 capture event ISR */
#define ISR_TIMER1_CAPT()  ISR(TIMER1_CAPT_vect)
/* TC1 compare match A ISR */
#define ISR_TIMER1_CMPA()  ISR(TIMER1_COMPA_vect)
/* TC1 compare match B ISR */
#define ISR_TIMER1_CMPB()  ISR(TIMER1_COMPB_vect)
/* TC1 overflow ISR */
#define ISR_TIMER1_OVF()   ISR(TIMER1_OVF_vect)


#ifdef _TIMER1_H_TEST_

uint8_t i = 0;

int main(void) {
    PORTB = 0;
    DDRB = ~0;

    timer1_set(TIMER1_CK_DIV1 | TIMER1_MODE_CTC_CMPA);
    timer1_value(0);
    timer1_compareA(1000);
    timer1_compareB(0);
    timer1_capture(0);
    timer1_signal(TIMER1_INT_CMPA);
    sei();

    for (;;) {
        PORTB = i;
    }

    return 0;
}

ISR_TIMER1_CMPA() {
    i++;
}

#endif /* _TIMER1_H_TEST_ */


#endif /* _TIMER1_H_ */

