/*
 * Sleep mode
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */

/*
 * Registers:
 * 
 *   MCUCR: MCU control register
 *     0,1 -> ISC00,1: external interrupt 0 sense control
 *     2,3 -> ISC10,1: external interrupt 1 sense control
 *     4,5,6 -> SM0,1,2: sleep mode
 *     7 -> SE: sleep enable
 */

/* MCUCR=SE|SM2|SM1|SM0|ISC11|ISC10|ISC01|ISC00 */


#ifndef _SLEEP_H_
#define _SLEEP_H_ 1


#include "util.h"


/* sleep modes (sleep_set) */
#define SLEEP_IDLE       (b0(SM2)|b0(SM1)|b0(SM0))  /* idle */
#define SLEEP_ADC        (b0(SM2)|b0(SM1)|b1(SM0))  /* adc noise reduction */
#define SLEEP_PWDOWN     (b0(SM2)|b1(SM1)|b0(SM0))  /* power down */
#define SLEEP_PWSAVE     (b0(SM2)|b1(SM1)|b1(SM0))  /* power save */
#define SLEEP_STANDBY    (b1(SM2)|b1(SM1)|b0(SM0))  /* standby */
#ifndef __AVR_ATmega8__
#define SLEEP_EXSTANDBY  (b1(SM2)|b1(SM1)|b1(SM0))  /* extended standby */
#endif /* __AVR_ATmega8__ */


/* sleep macros */
#define sleep_set(mod)  {cmi(MCUCR, b1(SM2)|b1(SM1)|b1(SM0)); smi(MCUCR, mod);}  /* setup */
#define sleep_en()      sbi(MCUCR, SE)                      /* enable */
#define sleep_di()      cbi(MCUCR, SE)                      /* disable */
#define sleep()         {__asm__ __volatile__ ("sleep");}   /* go to sleep */
#define sleep_one()     {sleep_en(); sleep(); sleep_di();}  /* sleep one stag */


#ifdef _SLEEP_H_TEST_

int main(void) {
    PORTB = 0;
    DDRB = ~0;

    sleep_set(SLEEP_PWDOWN);
    sleep_en();

    PORTB = 1;
    sleep_one();

    PORTB = 2;
    for (;;) {
        sleep();
    }

    return 0;
}

#endif /* _SLEEP_H_TEST_ */


#endif /* _SLEEP_H_ */

