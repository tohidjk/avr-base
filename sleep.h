/*
 * Sleep mode
 * Copyright (C) 2013-2021 Tohid Jafarzadeh <tohid.jk@gmail.com>
 * License GNU GPLv2
 * 2021-06-19 BETA
 */

/**
 * Registers:
 * 
 *   MCUCR: MCU control register
 *     0,1 -> ISC00,1: external interrupt 0 sense control
 *     2,3 -> ISC10,1: external interrupt 1 sense control
 *     4,5,6 -> SM0,1,2: sleep mode
 *     7 -> SE: sleep enable
 */


#ifndef _SLEEP_H_
#define _SLEEP_H_ 1


#include "util.h"

/* MCUCR=SE|SM2|SM1|SM0|ISC11|ISC10|ISC01|ISC00 */


/* sleep modes (sleep_set) */
#define SLEEP_IDLE       (b0(SM2)|b0(SM1)|b0(SM0))  /* idle mode */
#define SLEEP_ADC        (b0(SM2)|b0(SM1)|b1(SM0))  /* adc noise reduction mode */
#define SLEEP_PWDOWN     (b0(SM2)|b1(SM1)|b0(SM0))  /* power down mode */
#define SLEEP_PWSAVE     (b0(SM2)|b1(SM1)|b1(SM0))  /* power save mode */
#define SLEEP_STANDBY    (b1(SM2)|b1(SM1)|b0(SM0))  /* standby mode */
#ifndef __AVR_ATmega8__
#define SLEEP_EXSTANDBY  (b1(SM2)|b1(SM1)|b1(SM0))  /* extended standby mode */
#endif /* __AVR_ATmega8__ */


/* sleep macro routines */
#define sleep_set(mod)  {cmi(MCUCR, b1(SM2)|b1(SM1)|b1(SM0)); smi(MCUCR, mod);}  /* set sleep */
#define sleep_en()      sbi(MCUCR, SE)                      /* sleep enable */
#define sleep_di()      cbi(MCUCR, SE)                      /* sleep disable */
#define sleep()         {__asm__ __volatile__ ("sleep");}   /* get to sleep */
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

