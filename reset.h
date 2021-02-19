/*
 * Reset sources
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
 *         MCUCSR: MCU control and status register
 *   /-------+-------\
 *   7 6 5 4   3 2 1 0
 *   ^ ^   ^   ^ ^ ^ ^
 *   | |   |   | | | +--- PORF: power-on reset flag
 *   | |   |   | | +----- EXTRF: external reset flag
 *   | |   |   | +------- BORF: brown-out reset flag
 *   | |   |   +--------- WDRF: watchdog reset flag
 *   | |   +------------- JTRF: JTAG reset flag
 *   | +----------------- ISC2: external interrupt 2 sense control
 *   +------------------- JTD: JTAG interface disable
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
 * #if !RST_WDRF
 *   #warning "RST_WDRF"
 * #endif
 *   
 *   if (reset_check() == RST_WDRF)
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
#define RST_PORF  b1(PORF)
#else /* !PORF */
#define RST_PORF  (0)
#endif /* PORF */
/* external reset flag */
#ifdef EXTRF
#define RST_EXTRF  b1(EXTRF)
#else /* !EXTRF */
#define RST_EXTRF  (0)
#endif /* EXTRF */
/* brown-out reset flag */
#ifdef BORF
#define RST_BORF  b1(BORF)
#else /* !BORF */
#define RST_BORF  (0)
#endif /* BORF */
/* watchdog reset flag */
#ifdef WDRF
#define RST_WDRF  b1(WDRF)
#else /* !WDRF */
#define RST_WDRF  (0)
#endif /* WDRF */
/* JTAG reset flag */
#ifdef JTRF
#define RST_JTRF  b1(JTRF)
#else /* !JTRF */
#define RST_JTRF  (0)
#endif /* JTRF */


/* reset source check macro routines */
#define reset_check()  mis(MCUCSR, RST_PORF|RST_EXTRF|RST_BORF|RST_WDRF|RST_JTRF)  /* check reset source */
#define reset_clear()  cmi(MCUCSR, RST_PORF|RST_EXTRF|RST_BORF|RST_WDRF|RST_JTRF)  /* clear reset value for next use */


#endif /* _RESET_H_ */

