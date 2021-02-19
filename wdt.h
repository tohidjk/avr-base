/*
 * Watchdog timer
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
 *         WDTCR: watchdog timer control register
 *   /-------+-------\
 *   7 6 5 4   3 2 1 0
 *         ^   ^ \-+-/
 *         |   |   +----- WDP2,1,0: watchdog timer prescaler
 *         |   +--------- WDE: watchdog timer enable
 *         +------------- WDTOE: watchdog timer turn-off enable
 */

/*
 * Example:
 * 
 * #include <avr/io.h>
 * #include <util/delay.h>
 * #include "wdt.h"
 * 
 * int main(void) {
 *   PORTB = 0;
 *   DDRB = ~0;
 *   
 *   wdt_set(WDT_CK_2S);
 *   wdt_en();
 *   
 *   for (PORTB = 1; PORTB; PORTB <<= 1) {
 *     wdr();
 *     _delay_ms(500);
 *   }
 *   
 *   PORTB = ~0;
 *   for (;;);
 *   
 *   return 0;
 * }
 */


#ifndef _WDT_H_
#define _WDT_H_ 1


#include "util.h"

/* WDTCR=|||WDTOE|WDE|WDP2|WDP1|WDP0 */


/* watchdog timer clock prescaler (F_WDOSC: 1MHz) (wdt_set) */
#define WDT_CK_DIV16K    (b0(WDP2)|b0(WDP1)|b0(WDP0))  /* F_WDOSC/16K */
#define WDT_CK_DIV32K    (b0(WDP2)|b0(WDP1)|b1(WDP0))  /* F_WDOSC/32K */
#define WDT_CK_DIV64K    (b0(WDP2)|b1(WDP1)|b0(WDP0))  /* F_WDOSC/64K */
#define WDT_CK_DIV128K   (b0(WDP2)|b1(WDP1)|b1(WDP0))  /* F_WDOSC/128K */
#define WDT_CK_DIV256K   (b1(WDP2)|b0(WDP1)|b0(WDP0))  /* F_WDOSC/256K */
#define WDT_CK_DIV512K   (b1(WDP2)|b0(WDP1)|b1(WDP0))  /* F_WDOSC/512K */
#define WDT_CK_DIV1024K  (b1(WDP2)|b1(WDP1)|b0(WDP0))  /* F_WDOSC/1024K */
#define WDT_CK_DIV2048K  (b1(WDP2)|b1(WDP1)|b1(WDP0))  /* F_WDOSC/2048K */
#define WDT_CK_15MS   WDT_CK_DIV16K    /* typical timeout -> vcc=3v: 17.1ms & vcc=5v: 16.3ms */
#define WDT_CK_30MS   WDT_CK_DIV32K    /* typical timeout -> vcc=3v: 34.3ms & vcc=5v: 32.5ms */
#define WDT_CK_60MS   WDT_CK_DIV64K    /* typical timeout -> vcc=3v: 68.5ms & vcc=5v: 65.0ms */
#define WDT_CK_120MS  WDT_CK_DIV128K   /* typical timeout -> vcc=3v: 0.14s & vcc=5v: 0.13s */
#define WDT_CK_250MS  WDT_CK_DIV256K   /* typical timeout -> vcc=3v: 0.27s & vcc=5v: 0.26s */
#define WDT_CK_500MS  WDT_CK_DIV512K   /* typical timeout -> vcc=3v: 0.55s & vcc=5v: 0.52s */
#define WDT_CK_1S     WDT_CK_DIV1024K  /* typical timeout -> vcc=3v: 1.1s & vcc=5v: 1.0s */
#define WDT_CK_2S     WDT_CK_DIV2048K  /* typical timeout -> vcc=3v: 2.2s & vcc=5v: 2.1s */


/* watchdog timer macro routines */
#define wdt_set(cnt)  out(WDTCR, cnt)                        /* watchdog timer set */
#define wdt_en()      {cbi(WDTCR, WDTOE); sbi(WDTCR, WDE);}  /* watchdog timer enable */
#define wdt_di()      {sbi(WDTCR, WDTOE); cbi(WDTCR, WDE);}  /* watchdog timer disable */
#define wdr()         {__asm__ __volatile__ ("wdr");}        /* watchdog timer reset counted value */


#endif /* _WDT_H_ */

