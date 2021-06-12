/*
 * Analog to digital converter
 * Copyright (C) 2013-2021 Tohid Jafarzadeh <tohid.jk@gmail.com>
 * License GNU GPLv2
 * 2021-06-12 BETA
 */

/**
 * Registers:
 * 
 *       ADCSRA: ADC control and status register
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^ ^ ^ ^ \-+-/
 *   | | | | |   +----- ADPS2,1,0: prescaler select
 *   | | | | +--------- ADIE: interrupt enable
 *   | | | +----------- ADIF: interrupt flag
 *   | | +------------- m32,m16=ADATE: auto trigger enable - m8=ADFR: free runing
 *   | +--------------- ADSC: start conversion
 *   +----------------- ADEN: enable
 * 
 *        ADMUX: ADC multiplexer selection register
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   \+/ ^ \---+---/
 *    |  |     +------- MUX4,3,2,1,0: analog channel and gain selection
 *    |  +------------- ADLAR: left adjust result
 *    +---------------- REFS1,0: reference selection
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
 *   ADCW=ADCH+ADCL: data registers
 */

/*
 * Example:
 * 
 * #include <avr/io.h>
 * #include "adc.h"
 * 
 * int main(void) {
 *   PORTB = 0;
 *   DDRB = ~0;
 * 
 *   adc_set(ADC_CK_DIV64 | ADC_VREF_AREF | ADC_AUTO_TRIGGER | ADC_FREE_RUN | ADC_START);
 *   adc_signal(ADC_INT_COMPLETE);
 *   adc_input(ADC0);
 *   adc_en();
 *   sei();
 * 
 *   for (;;) {
 *     adc_wait();
 *     PORTB = (adc_data() >> 2);
 *   }
 * 
 *   return 0;
 * }
 * 
 * ISR_ADC() {}
 */


#ifndef _ADC_H_
#define _ADC_H_ 1


#include "util.h"

/* ADCSRA=ADEN|ADSC|(ADATE|ADFR)|ADIF|ADIE|ADPS2|ADPS1|ADPS0 */
/* ADMUX=REFS1|REFS0|ADLAR|MUX4|MUX3|MUX2|MUX1|MUX0 */
/* SFIOR=ADTS2|ADTS1|ADTS0||ACME|PUD|PSR2|PSR10 */


/* analog to digital converter clock prescaler (max speed:1MHz) (adc_set) */
#define ADC_CK_DIV2    (b0(ADPS2)|b0(ADPS1)|b1(ADPS0))  /* F_CPU/2 */
#define ADC_CK_DIV4    (b0(ADPS2)|b1(ADPS1)|b0(ADPS0))  /* F_CPU/4 */
#define ADC_CK_DIV8    (b0(ADPS2)|b1(ADPS1)|b1(ADPS0))  /* F_CPU/8 */
#define ADC_CK_DIV16   (b1(ADPS2)|b0(ADPS1)|b0(ADPS0))  /* F_CPU/16 */
#define ADC_CK_DIV32   (b1(ADPS2)|b0(ADPS1)|b1(ADPS0))  /* F_CPU/32 */
#define ADC_CK_DIV64   (b1(ADPS2)|b1(ADPS1)|b0(ADPS0))  /* F_CPU/64 */
#define ADC_CK_DIV128  (b1(ADPS2)|b1(ADPS1)|b1(ADPS0))  /* F_CPU/128 */

/* analog to digital converter voltage refrence (adc_set) */
#define ADC_VREF_AREF    ((b0(REFS1)|b0(REFS0))<<8)  /* voltage refrence on AREF pin */
#define ADC_VREF_AVCC    ((b0(REFS1)|b1(REFS0))<<8)  /* voltage refrence on AVCC pin */
#define ADC_VREF_INT256  ((b1(REFS1)|b1(REFS0))<<8)  /* voltage refrence on internal 2.56v */

/* analog to digital converter options (adc_set) */
#define ADC_DATA_RIGHT_ADJUST  (b0(ADLAR)<<8)  /* data result right adjust */
#define ADC_DATA_LEFT_ADJUST   (b1(ADLAR)<<8)  /* data result left adjust */
#define ADC_START              b1(ADSC)        /* start convertion */
#ifdef ADATE
#define ADC_AUTO_TRIGGER       b1(ADATE)       /* auto trigger enable */
#endif /* ADATE */

/* analog to digital converter auto trigger modes (adc_set) */
#ifdef ADATE
#define ADC_FREE_RUN             ((b0(ADTS2)|b0(ADTS1)|b0(ADTS0))<<16)  /* free running */
#define ADC_TRIGGER_ACMP         ((b0(ADTS2)|b0(ADTS1)|b1(ADTS0))<<16)  /* analog comparator */
#define ADC_TRIGGER_IRQ0         ((b0(ADTS2)|b1(ADTS1)|b0(ADTS0))<<16)  /* external IRQ0 */
#define ADC_TRIGGER_TIMER0_CMP   ((b0(ADTS2)|b1(ADTS1)|b1(ADTS0))<<16)  /* timer/counter0 compare match */
#define ADC_TRIGGER_TIMER0_OVF   ((b1(ADTS2)|b0(ADTS1)|b0(ADTS0))<<16)  /* timer/counter0 overflow */
#define ADC_TRIGGER_TIMER1_CMPB  ((b1(ADTS2)|b0(ADTS1)|b1(ADTS0))<<16)  /* timer/counter1 compare match B */
#define ADC_TRIGGER_TIMER1_OVF   ((b1(ADTS2)|b1(ADTS1)|b0(ADTS0))<<16)  /* timer/counter1 overflow */
#define ADC_TRIGGER_TIMER1_CAPT  ((b1(ADTS2)|b1(ADTS1)|b1(ADTS0))<<16)  /* timer/counter1 capture event */
#endif /* ADATE */
#ifdef ADFR
#define ADC_FREE_RUN             b1(ADFR)   /* free running */
#endif /* ADFR */

/* analog to digital converter chanels and gain (adc_input) */
#define ADC0  (b0(MUX3)|b0(MUX2)|b0(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* chanel 0 - not differential - not gain */
#define ADC1  (b0(MUX3)|b0(MUX2)|b0(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* chanel 1 - not differential - not gain */
#define ADC2  (b0(MUX3)|b0(MUX2)|b1(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* chanel 2 - not differential - not gain */
#define ADC3  (b0(MUX3)|b0(MUX2)|b1(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* chanel 3 - not differential - not gain */
#define ADC4  (b0(MUX3)|b1(MUX2)|b0(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* chanel 4 - not differential - not gain */
#define ADC5  (b0(MUX3)|b1(MUX2)|b0(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* chanel 5 - not differential - not gain */
#define ADC6  (b0(MUX3)|b1(MUX2)|b1(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* chanel 6 - not differential - not gain */
#define ADC7  (b0(MUX3)|b1(MUX2)|b1(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* chanel 7 - not differential - not gain */
#define ADC_DIF00_10   (b1(MUX3)|b0(MUX2)|b0(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* positive: chanel 0 - negative: chanel 0 - gain: 10x */
#define ADC_DIF10_10   (b1(MUX3)|b0(MUX2)|b0(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* positive: chanel 1 - negative: chanel 0 - gain: 10x */
#define ADC_DIF00_200  (b1(MUX3)|b0(MUX2)|b1(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* positive: chanel 0 - negative: chanel 0 - gain: 200x */
#define ADC_DIF10_200  (b1(MUX3)|b0(MUX2)|b1(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* positive: chanel 1 - negative: chanel 0 - gain: 200x */
#define ADC_DIF22_10   (b1(MUX3)|b1(MUX2)|b0(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* positive: chanel 2 - negative: chanel 2 - gain: 10x */
#define ADC_DIF32_10   (b1(MUX3)|b1(MUX2)|b0(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* positive: chanel 3 - negative: chanel 2 - gain: 10x */
#define ADC_DIF22_200  (b1(MUX3)|b1(MUX2)|b1(MUX1)|b0(MUX0)/*|b0(MUX4)*/)  /* positive: chanel 2 - negative: chanel 2 - gain: 200x */
#define ADC_DIF32_200  (b1(MUX3)|b1(MUX2)|b1(MUX1)|b1(MUX0)/*|b0(MUX4)*/)  /* positive: chanel 3 - negative: chanel 2 - gain: 200x */
#ifdef MUX4
#define ADC_DIF01  (b1(MUX4)|b0(MUX3)|b0(MUX2)|b0(MUX1)|b0(MUX0))  /* positive: chanel 0 - negative: chanel 1 - gain: 1x */
#define ADC_DIF11  (b1(MUX4)|b0(MUX3)|b0(MUX2)|b0(MUX1)|b1(MUX0))  /* positive: chanel 1 - negative: chanel 1 - gain: 1x */
#define ADC_DIF21  (b1(MUX4)|b0(MUX3)|b0(MUX2)|b1(MUX1)|b0(MUX0))  /* positive: chanel 2 - negative: chanel 1 - gain: 1x */
#define ADC_DIF31  (b1(MUX4)|b0(MUX3)|b0(MUX2)|b1(MUX1)|b1(MUX0))  /* positive: chanel 3 - negative: chanel 1 - gain: 1x */
#define ADC_DIF41  (b1(MUX4)|b0(MUX3)|b1(MUX2)|b0(MUX1)|b0(MUX0))  /* positive: chanel 4 - negative: chanel 1 - gain: 1x */
#define ADC_DIF51  (b1(MUX4)|b0(MUX3)|b1(MUX2)|b0(MUX1)|b1(MUX0))  /* positive: chanel 5 - negative: chanel 1 - gain: 1x */
#define ADC_DIF61  (b1(MUX4)|b0(MUX3)|b1(MUX2)|b1(MUX1)|b0(MUX0))  /* positive: chanel 6 - negative: chanel 1 - gain: 1x */
#define ADC_DIF71  (b1(MUX4)|b0(MUX3)|b1(MUX2)|b1(MUX1)|b1(MUX0))  /* positive: chanel 7 - negative: chanel 1 - gain: 1x */
#define ADC_DIF02  (b1(MUX4)|b1(MUX3)|b0(MUX2)|b0(MUX1)|b0(MUX0))  /* positive: chanel 0 - negative: chanel 2 - gain: 1x */
#define ADC_DIF12  (b1(MUX4)|b1(MUX3)|b0(MUX2)|b0(MUX1)|b1(MUX0))  /* positive: chanel 1 - negative: chanel 2 - gain: 1x */
#define ADC_DIF22  (b1(MUX4)|b1(MUX3)|b0(MUX2)|b1(MUX1)|b0(MUX0))  /* positive: chanel 2 - negative: chanel 2 - gain: 1x */
#define ADC_DIF32  (b1(MUX4)|b1(MUX3)|b0(MUX2)|b1(MUX1)|b1(MUX0))  /* positive: chanel 3 - negative: chanel 2 - gain: 1x */
#define ADC_DIF42  (b1(MUX4)|b1(MUX3)|b1(MUX2)|b0(MUX1)|b0(MUX0))  /* positive: chanel 4 - negative: chanel 2 - gain: 1x */
#define ADC_DIF52  (b1(MUX4)|b1(MUX3)|b1(MUX2)|b0(MUX1)|b1(MUX0))  /* positive: chanel 5 - negative: chanel 2 - gain: 1x */
#define ADC_122V   (b1(MUX4)|b1(MUX3)|b1(MUX2)|b1(MUX1)|b0(MUX0))  /* voltage refrence:1.22v - not differential - not gain */
#define ADC_AGND   (b1(MUX4)|b1(MUX3)|b1(MUX2)|b1(MUX1)|b1(MUX0))  /* AGND pin:0v - not differential - not gain */
#endif /* MUX4 */

/* analog to digital converter interrupts (adc_signal) */
#define ADC_INT_COMPLETE  b1(ADIE)  /* adc complete */


/* analog to digital converter macro routines */
#define adc_set(cnt)     {out(ADCSRA, cnt); out(ADMUX, (cnt)>>8); smi(SFIOR, (cnt)>>16);}  /* set adc */
#define adc_input(cnl)   {cmi(ADMUX, b1(4)|b1(MUX3)|b1(MUX2)|b1(MUX1)|b1(MUX0)); smi(ADMUX, cnl);}  /* adc set chanel and gain (4 replace MUX4 for reject problem) */
#define adc_signal(sgn)  smi(ADCSRA, sgn)            /* adc enable signals */
#define adc_en()         sbi(ADCSRA, ADEN)           /* adc enable */
#define adc_di()         cbi(ADCSRA, ADEN)           /* adc disable */
#define adc_wait()       wait_set_bit(ADCSRA, ADIF)  /* wait to adc conversion complete signal */
#define adc_data()       in(ADCW)                    /* read adc right adjust 10bit result */
#define adc_data_left()  (in(ADCW)>>6)               /* read adc left adjust 10bit result */


/* analog to digital conversion complete interrupt service routine */
#define ISR_ADC()  ISR(ADC_vect)


#endif /* _ADC_H_ */

