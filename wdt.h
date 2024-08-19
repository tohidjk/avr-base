/*
 * Watchdog timer
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */

/*
 * Registers:
 * 
 *   WDTCR: WDT control register
 *     0,1,2 -> WDP0,1,2: prescaler
 *     3 -> WDE: enable
 *     4 -> WDTOE: turn-off enable
 */

/* WDTCR=|||WDTOE|WDE|WDP2|WDP1|WDP0 */


#ifndef _WDT_H_
#define _WDT_H_ 1


#include "util.h"


#ifndef WDTOE
#define WDTOE WDCE
#endif


/* WDT clock prescaler (F_WDOSC: 1MHz) (wdt_set) */
#define WDT_CK_DIV16K    (b0(WDP2)|b0(WDP1)|b0(WDP0))  /* F_WDOSC/16K */
#define WDT_CK_DIV32K    (b0(WDP2)|b0(WDP1)|b1(WDP0))  /* F_WDOSC/32K */
#define WDT_CK_DIV64K    (b0(WDP2)|b1(WDP1)|b0(WDP0))  /* F_WDOSC/64K */
#define WDT_CK_DIV128K   (b0(WDP2)|b1(WDP1)|b1(WDP0))  /* F_WDOSC/128K */
#define WDT_CK_DIV256K   (b1(WDP2)|b0(WDP1)|b0(WDP0))  /* F_WDOSC/256K */
#define WDT_CK_DIV512K   (b1(WDP2)|b0(WDP1)|b1(WDP0))  /* F_WDOSC/512K */
#define WDT_CK_DIV1024K  (b1(WDP2)|b1(WDP1)|b0(WDP0))  /* F_WDOSC/1024K */
#define WDT_CK_DIV2048K  (b1(WDP2)|b1(WDP1)|b1(WDP0))  /* F_WDOSC/2048K */
#define WDT_CK_15MS   WDT_CK_DIV16K    /* timeout -> vcc=3v: 17.1ms & vcc=5v: 16.3ms */
#define WDT_CK_30MS   WDT_CK_DIV32K    /* timeout -> vcc=3v: 34.3ms & vcc=5v: 32.5ms */
#define WDT_CK_60MS   WDT_CK_DIV64K    /* timeout -> vcc=3v: 68.5ms & vcc=5v: 65.0ms */
#define WDT_CK_120MS  WDT_CK_DIV128K   /* timeout -> vcc=3v: 0.14s & vcc=5v: 0.13s */
#define WDT_CK_250MS  WDT_CK_DIV256K   /* timeout -> vcc=3v: 0.27s & vcc=5v: 0.26s */
#define WDT_CK_500MS  WDT_CK_DIV512K   /* timeout -> vcc=3v: 0.55s & vcc=5v: 0.52s */
#define WDT_CK_1S     WDT_CK_DIV1024K  /* timeout -> vcc=3v: 1.1s & vcc=5v: 1.0s */
#define WDT_CK_2S     WDT_CK_DIV2048K  /* timeout -> vcc=3v: 2.2s & vcc=5v: 2.1s */


/* WDT macros */
#define wdt_set(cnt)  out(WDTCR, cnt)                        /* setup */
#define wdt_en()      {cbi(WDTCR, WDTOE); sbi(WDTCR, WDE);}  /* enable */
#define wdt_di()      {sbi(WDTCR, WDTOE); cbi(WDTCR, WDE);}  /* disable */
#define wdt_reset()   {__asm__ __volatile__ ("wdr");}        /* reset counted value */


#ifdef _WDT_H_TEST_

#include <util/delay.h>

int main(void) {
    PORTB = 0;
    DDRB = ~0;

    wdt_set(WDT_CK_2S);
    wdt_en();

    for (PORTB = 1; PORTB; PORTB <<= 1) {
        wdt_reset();
        _delay_ms(500);
    }

    PORTB = ~0;
    for (;;);

    return 0;
}

#endif /* _WDT_H_TEST_ */


#endif /* _WDT_H_ */

