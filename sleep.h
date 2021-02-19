/*
 * Sleep mode
 * 
 * Copyright (C) 2013-2021 Tohid Jafarzadeh <tohid.jk@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 16.02.2021
 */

/**
 * Registers:
 *   
 *         MCUCR: MCU control register
 *   /-------+-------\
 *   7 6 5 4   3 2 1 0
 *   ^ \-+-/   \+/ \+/
 *   |   |      |   +---- ISC01,0: external interrupt 0 sense control
 *   |   |      +-------- ISC11,0: external interrupt 1 sense control
 *   |   +--------------- SM2,1,0: sleep mode
 *   +------------------- SE: sleep enable
 */

/*
 * Example:
 * 
 * #include <avr/io.h>
 * #include "sleep.h"
 * 
 * int main(void) {
 *   PORTB = 0;
 *   DDRB = ~0;
 *   
 *   sleep_set(SLEEP_MODE_PWDOWN);
 *   sleep_en();
 *   
 *   PORTB = 1;
 *   sleep_one();
 *   
 *   PORTB = 2;
 *   for (;;) {
 *     sleep();
 *   }
 *   
 *   return 0;
 * }
 */


#ifndef _SLEEP_H_
#define _SLEEP_H_ 1


#include "util.h"

/* MCUCR=SE|SM2|SM1|SM0|ISC11|ISC10|ISC01|ISC00 */


/* sleep modes (sleep_set) */
#define SLEEP_MODE_IDLE       (b0(SM2)|b0(SM1)|b0(SM0))  /* idle mode */
#define SLEEP_MODE_ADC        (b0(SM2)|b0(SM1)|b1(SM0))  /* adc noise reduction mode */
#define SLEEP_MODE_PWDOWN     (b0(SM2)|b1(SM1)|b0(SM0))  /* power down mode */
#define SLEEP_MODE_PWSAVE     (b0(SM2)|b1(SM1)|b1(SM0))  /* power save mode */
#define SLEEP_MODE_STANDBY    (b1(SM2)|b1(SM1)|b0(SM0))  /* standby mode */
#ifndef __AVR_ATmega8__
#define SLEEP_MODE_EXSTANDBY  (b1(SM2)|b1(SM1)|b1(SM0))  /* extended standby mode */
#endif /* __AVR_ATmega8__ */


/* sleep macro routines */
#define sleep_set(mod)  {cmi(MCUCR, b1(SM2)|b1(SM1)|b1(SM0)); smi(MCUCR, mod);}  /* set sleep */
#define sleep_en()      sbi(MCUCR, SE)                      /* sleep enable */
#define sleep_di()      cbi(MCUCR, SE)                      /* sleep disable */
#define sleep()         {__asm__ __volatile__ ("sleep");}   /* get to sleep */
#define sleep_one()     {sleep_en(); sleep(); sleep_di();}  /* sleep one stag */


#endif /* _SLEEP_H_ */

