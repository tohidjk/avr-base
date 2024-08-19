/*
 * Analog to digital converter
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */

/*
 * Registers:
 * 
 *   ADCSRA: ADC control and status register
 *     0,1,2 -> ADPS0,1,2: prescaler select
 *     3 -> ADIE: interrupt enable
 *     4 -> ADIF: interrupt flag
 *     5 -> m32,m16=ADATE: auto trigger enable - m8=ADFR: free runing
 *     6 -> ADSC: start conversion
 *     7 -> ADEN: enable
 * 
 *   ADMUX: ADC multiplexer selection register
 *     0,1,2,3,4 -> MUX0,1,2,3,4: analog channel and gain selection
 *     5 -> ADLAR: left adjust result
 *     6,7 -> REFS0,1: reference selection
 * 
 *   SFIOR: special function IO register
 *     0 -> PSR10: prescaler reset timer/counter1&0
 *     1 -> PSR2: prescaler Reset timer/counter2
 *     2 -> PUD: pull-up disable
 *     3 -> ACME: analog comparator multiplexer enable
 *     4 -> m8=ADHSM: ADC high speed mode
 *     5,6,7 -> ADTS0,1,2: ADC auto trigger source
 * 
 *   ADCW=ADCH+ADCL: ADC data registers
 */

/* ADCSRA=ADEN|ADSC|(ADATE|ADFR)|ADIF|ADIE|ADPS2|ADPS1|ADPS0 */
/* ADMUX=REFS1|REFS0|ADLAR|MUX4|MUX3|MUX2|MUX1|MUX0 */
/* SFIOR=ADTS2|ADTS1|ADTS0||ACME|PUD|PSR2|PSR10 */


#ifndef _ADC_H_
#define _ADC_H_ 1


#include "util.h"


/* ADC clock prescaler (max speed:1MHz) (adc_set) */
#define ADC_CK_DIV2    (b0(ADPS2)|b0(ADPS1)|b1(ADPS0))  /* F_CPU/2 */
#define ADC_CK_DIV4    (b0(ADPS2)|b1(ADPS1)|b0(ADPS0))  /* F_CPU/4 */
#define ADC_CK_DIV8    (b0(ADPS2)|b1(ADPS1)|b1(ADPS0))  /* F_CPU/8 */
#define ADC_CK_DIV16   (b1(ADPS2)|b0(ADPS1)|b0(ADPS0))  /* F_CPU/16 */
#define ADC_CK_DIV32   (b1(ADPS2)|b0(ADPS1)|b1(ADPS0))  /* F_CPU/32 */
#define ADC_CK_DIV64   (b1(ADPS2)|b1(ADPS1)|b0(ADPS0))  /* F_CPU/64 */
#define ADC_CK_DIV128  (b1(ADPS2)|b1(ADPS1)|b1(ADPS0))  /* F_CPU/128 */

/* ADC voltage refrence (adc_set) */
#define ADC_VREF_AREF    ((b0(REFS1)|b0(REFS0))<<8)  /* AREF pin */
#define ADC_VREF_AVCC    ((b0(REFS1)|b1(REFS0))<<8)  /* AVCC pin */
#define ADC_VREF_INT256  ((b1(REFS1)|b1(REFS0))<<8)  /* internal 2.56v */

/* ADC options (adc_set) */
#define ADC_DATA_RIGHT_ADJUST  (b0(ADLAR)<<8)  /* data result right adjust */
#define ADC_DATA_LEFT_ADJUST   (b1(ADLAR)<<8)  /* data result left adjust */
#define ADC_START              b1(ADSC)        /* start convertion */

/* ADC auto trigger modes (adc_set) */
#ifdef ADATE
#define ADC_FREE_RUN             (b1(ADATE)|((uint32_t)(b0(ADTS2)|b0(ADTS1)|b0(ADTS0))<<16))  /* free running */
#define ADC_TRIGGER_ACMP         (b1(ADATE)|((uint32_t)(b0(ADTS2)|b0(ADTS1)|b1(ADTS0))<<16))  /* analog comparator */
#define ADC_TRIGGER_IRQ0         (b1(ADATE)|((uint32_t)(b0(ADTS2)|b1(ADTS1)|b0(ADTS0))<<16))  /* external IRQ0 */
#define ADC_TRIGGER_TIMER0_CMP   (b1(ADATE)|((uint32_t)(b0(ADTS2)|b1(ADTS1)|b1(ADTS0))<<16))  /* timer/counter0 compare match */
#define ADC_TRIGGER_TIMER0_OVF   (b1(ADATE)|((uint32_t)(b1(ADTS2)|b0(ADTS1)|b0(ADTS0))<<16))  /* timer/counter0 overflow */
#define ADC_TRIGGER_TIMER1_CMPB  (b1(ADATE)|((uint32_t)(b1(ADTS2)|b0(ADTS1)|b1(ADTS0))<<16))  /* timer/counter1 compare match B */
#define ADC_TRIGGER_TIMER1_OVF   (b1(ADATE)|((uint32_t)(b1(ADTS2)|b1(ADTS1)|b0(ADTS0))<<16))  /* timer/counter1 overflow */
#define ADC_TRIGGER_TIMER1_CAPT  (b1(ADATE)|((uint32_t)(b1(ADTS2)|b1(ADTS1)|b1(ADTS0))<<16))  /* timer/counter1 capture event */
#endif /* ADATE */
#ifdef ADFR
#define ADC_FREE_RUN             b1(ADFR)  /* free running */
#endif /* ADFR */

/* ADC chanels and gain (adc_input) */
#define ADC0  (b0(MUX3)|b0(MUX2)|b0(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* chanel 0 - not differential - not gain */
#define ADC1  (b0(MUX3)|b0(MUX2)|b0(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* chanel 1 - not differential - not gain */
#define ADC2  (b0(MUX3)|b0(MUX2)|b1(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* chanel 2 - not differential - not gain */
#define ADC3  (b0(MUX3)|b0(MUX2)|b1(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* chanel 3 - not differential - not gain */
#define ADC4  (b0(MUX3)|b1(MUX2)|b0(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* chanel 4 - not differential - not gain */
#define ADC5  (b0(MUX3)|b1(MUX2)|b0(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* chanel 5 - not differential - not gain */
#define ADC6  (b0(MUX3)|b1(MUX2)|b1(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* chanel 6 - not differential - not gain */
#define ADC7  (b0(MUX3)|b1(MUX2)|b1(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* chanel 7 - not differential - not gain */
#define ADC_DIF00_10   (b1(MUX3)|b0(MUX2)|b0(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* pos: chanel 0 - neg: chanel 0 - gain: 10x */
#define ADC_DIF10_10   (b1(MUX3)|b0(MUX2)|b0(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* pos: chanel 1 - neg: chanel 0 - gain: 10x */
#define ADC_DIF00_200  (b1(MUX3)|b0(MUX2)|b1(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* pos: chanel 0 - neg: chanel 0 - gain: 200x */
#define ADC_DIF10_200  (b1(MUX3)|b0(MUX2)|b1(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* pos: chanel 1 - neg: chanel 0 - gain: 200x */
#define ADC_DIF22_10   (b1(MUX3)|b1(MUX2)|b0(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* pos: chanel 2 - neg: chanel 2 - gain: 10x */
#define ADC_DIF32_10   (b1(MUX3)|b1(MUX2)|b0(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* pos: chanel 3 - neg: chanel 2 - gain: 10x */
#define ADC_DIF22_200  (b1(MUX3)|b1(MUX2)|b1(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* pos: chanel 2 - neg: chanel 2 - gain: 200x */
#define ADC_DIF32_200  (b1(MUX3)|b1(MUX2)|b1(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* pos: chanel 3 - neg: chanel 2 - gain: 200x */
#ifdef MUX4
#define ADC_DIF01  (b1(MUX4)|b0(MUX3)|b0(MUX2)|b0(MUX1)|b0(MUX0))  /* pos: chanel 0 - neg: chanel 1 - gain: 1x */
#define ADC_DIF11  (b1(MUX4)|b0(MUX3)|b0(MUX2)|b0(MUX1)|b1(MUX0))  /* pos: chanel 1 - neg: chanel 1 - gain: 1x */
#define ADC_DIF21  (b1(MUX4)|b0(MUX3)|b0(MUX2)|b1(MUX1)|b0(MUX0))  /* pos: chanel 2 - neg: chanel 1 - gain: 1x */
#define ADC_DIF31  (b1(MUX4)|b0(MUX3)|b0(MUX2)|b1(MUX1)|b1(MUX0))  /* pos: chanel 3 - neg: chanel 1 - gain: 1x */
#define ADC_DIF41  (b1(MUX4)|b0(MUX3)|b1(MUX2)|b0(MUX1)|b0(MUX0))  /* pos: chanel 4 - neg: chanel 1 - gain: 1x */
#define ADC_DIF51  (b1(MUX4)|b0(MUX3)|b1(MUX2)|b0(MUX1)|b1(MUX0))  /* pos: chanel 5 - neg: chanel 1 - gain: 1x */
#define ADC_DIF61  (b1(MUX4)|b0(MUX3)|b1(MUX2)|b1(MUX1)|b0(MUX0))  /* pos: chanel 6 - neg: chanel 1 - gain: 1x */
#define ADC_DIF71  (b1(MUX4)|b0(MUX3)|b1(MUX2)|b1(MUX1)|b1(MUX0))  /* pos: chanel 7 - neg: chanel 1 - gain: 1x */
#define ADC_DIF02  (b1(MUX4)|b1(MUX3)|b0(MUX2)|b0(MUX1)|b0(MUX0))  /* pos: chanel 0 - neg: chanel 2 - gain: 1x */
#define ADC_DIF12  (b1(MUX4)|b1(MUX3)|b0(MUX2)|b0(MUX1)|b1(MUX0))  /* pos: chanel 1 - neg: chanel 2 - gain: 1x */
#define ADC_DIF22  (b1(MUX4)|b1(MUX3)|b0(MUX2)|b1(MUX1)|b0(MUX0))  /* pos: chanel 2 - neg: chanel 2 - gain: 1x */
#define ADC_DIF32  (b1(MUX4)|b1(MUX3)|b0(MUX2)|b1(MUX1)|b1(MUX0))  /* pos: chanel 3 - neg: chanel 2 - gain: 1x */
#define ADC_DIF42  (b1(MUX4)|b1(MUX3)|b1(MUX2)|b0(MUX1)|b0(MUX0))  /* pos: chanel 4 - neg: chanel 2 - gain: 1x */
#define ADC_DIF52  (b1(MUX4)|b1(MUX3)|b1(MUX2)|b0(MUX1)|b1(MUX0))  /* pos: chanel 5 - neg: chanel 2 - gain: 1x */
#define ADC_122V   (b1(MUX4)|b1(MUX3)|b1(MUX2)|b1(MUX1)|b0(MUX0))  /* voltage refrence:1.22v - not differential - not gain */
#define ADC_AGND   (b1(MUX4)|b1(MUX3)|b1(MUX2)|b1(MUX1)|b1(MUX0))  /* AGND pin:0v - not differential - not gain */
#endif /* MUX4 */

/* ADC interrupts (adc_signal) */
#define ADC_INT_COMPLETE  b1(ADIE)  /* adc complete */


/* ADC macros */
#define adc_set(cnt)     {out(ADCSRA, (cnt)&0xFF); out(ADMUX, ((cnt)>>8)&0xFF); smi(SFIOR, ((uint32_t)(cnt))>>16);}  /* setup */
#define adc_input(cnl)   {cmi(ADMUX, b1(4)|b1(MUX3)|b1(MUX2)|b1(MUX1)|b1(MUX0)); smi(ADMUX, cnl);}  /* set chanel and gain (4 replace MUX4 for reject problem) */
#define adc_signal(sgn)  smi(ADCSRA, sgn)            /* enable signals */
#define adc_en()         sbi(ADCSRA, ADEN)           /* enable */
#define adc_di()         cbi(ADCSRA, ADEN)           /* disable */
#define adc_wait()       wait_set_bit(ADCSRA, ADIF)  /* wait to conversion complete signal */
#define adc_data()       in(ADCW)                    /* read right adjust 10bit result */
#define adc_data_left()  (in(ADCW)>>6)               /* read left adjust 10bit result */


/* ADC complete ISR */
#define ISR_ADC()  ISR(ADC_vect)


#ifdef _ADC_H_TEST_

int main(void) {
    PORTB = 0;
    DDRB = ~0;

    adc_set(ADC_CK_DIV64 | ADC_VREF_AREF | ADC_FREE_RUN | ADC_START);
    adc_signal(ADC_INT_COMPLETE);
    adc_input(ADC0);
    adc_en();
    sei();

    for (;;) {
        adc_wait();
        PORTB = (adc_data() >> 2);
    }

    return 0;
}

ISR_ADC() {}

#endif /* _ADC_H_TEST_ */


#endif /* _ADC_H_ */

