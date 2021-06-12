/*
 * Reset sources
 * Copyright (C) 2013-2021 Tohid Jafarzadeh <tohid.jk@gmail.com>
 * License GNU GPLv2
 * 2021-06-12 BETA
 */

/**
 * Registers:
 * 
 *       MCUCSR: MCU control and status register
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^   ^ ^ ^ ^ ^
 *   | |   | | | | +--- PORF: power-on reset flag
 *   | |   | | | +----- EXTRF: external reset flag
 *   | |   | | +------- BORF: brown-out reset flag
 *   | |   | +--------- WDRF: watchdog reset flag
 *   | |   +----------- JTRF: JTAG reset flag
 *   | +--------------- ISC2: external interrupt 2 sense control
 *   +----------------- JTD: JTAG interface disable
 */

/*
 * Example:
 * 
 * #include <avr/io.h>
 * #include "reset.h"
 * 
 * int main(void) {
 *   PORTB = 0;
 *   DDRB = ~0;
 * 
 * #if !RESET_WDRF
 *   #warning "RESET_WDRF"
 * #endif
 * 
 *   if (reset_check() == RESET_WDRF)
 *     PORTB = 1;
 *   reset_clear();
 * 
 *   for (;;);
 * 
 *   return 0;
 * }
 */


#ifndef _RESET_H_
#define _RESET_H_ 1


#include "util.h"

/* MCUCSR=JTD|ISC2||JTRF|WDRF|BORF|EXTRF|PORF */


/* power-on reset flag */
#ifdef PORF
	#define RESET_PORF  b1(PORF)
#else /* !PORF */
	#define RESET_PORF  (0)
#endif /* PORF */

/* external reset flag */
#ifdef EXTRF
	#define RESET_EXTRF  b1(EXTRF)
#else /* !EXTRF */
	#define RESET_EXTRF  (0)
#endif /* EXTRF */

/* brown-out reset flag */
#ifdef BORF
	#define RESET_BORF  b1(BORF)
#else /* !BORF */
	#define RESET_BORF  (0)
#endif /* BORF */

/* watchdog reset flag */
#ifdef WDRF
	#define RESET_WDRF  b1(WDRF)
#else /* !WDRF */
	#define RESET_WDRF  (0)
#endif /* WDRF */

/* JTAG reset flag */
#ifdef JTRF
	#define RESET_JTRF  b1(JTRF)
#else /* !JTRF */
	#define RESET_JTRF  (0)
#endif /* JTRF */


/* reset source check macro routines */
#define reset_check()  mis(MCUCSR, RESET_PORF|RESET_EXTRF|RESET_BORF|RESET_WDRF|RESET_JTRF)  /* check reset source */
#define reset_clear()  cmi(MCUCSR, RESET_PORF|RESET_EXTRF|RESET_BORF|RESET_WDRF|RESET_JTRF)  /* clear reset value for next use */


#endif /* _RESET_H_ */

