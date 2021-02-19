/*
 * External interrupt requestes
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
 *   
 *         MCUCR: MCU control register
 *   /-------+-------\
 *   7 6 5 4   3 2 1 0
 *   ^ \-+-/   \+/ \+/
 *   |   |      |   +---- ISC01,0: external interrupt 0 sense control
 *   |   |      +-------- ISC11,0: external interrupt 1 sense control
 *   |   +--------------- SM2,1,0: sleep mode
 *   +------------------- SE: sleep enable
 *   
 *          GIFR: general interrupt flag register
 *   /-------+-------\
 *   7 6 5 4   3 2 1 0
 *   ^ ^ ^
 *   | | +--------------- INTF2: external interrupt 2 flag
 *   | +----------------- INTF0: external interrupt 0 flag
 *   +------------------- INTF1: external interrupt 1 flag
 *   
 *          GICR: general interrupt control register
 *   /-------+-------\
 *   7 6 5 4   3 2 1 0
 *   ^ ^ ^         ^ ^
 *   | | |         | +--- IVCE: interrupt vector change enable
 *   | | |         +----- IVSEL: interrupt vector select
 *   | | +--------------- INT2: external interrupt request 2 enable
 *   | +----------------- INT0: external interrupt request 0 enable
 *   +------------------- INT1: external interrupt request 1 enable
 */

/*
 * Example:
 * 
 * #include <avr/io.h>
 * #include "irq.h"
 * 
 * int main(void) {
 *   PORTB = 0;
 *   DDRB = ~0;
 *   
 *   irq_set(IRQ_INT0 | IRQ_INT1);
 *   int0_set(INT0_MODE_FALL);
 *   int1_set(INT1_MODE_RISE);
 *   sei();
 *   
 *   for (;;) {
 *     int1_wait();
 *     PORTB ^= 2;
 *   }
 *   
 *   return 0;
 * }
 * 
 * ISR_INT0() {
 *   PORTB ^= 1;
 * }
 * 
 * ISR_INT1() {}
 */


#ifndef _IRQ_H_
#define _IRQ_H_ 1


#include "util.h"

/* MCUCSR=JTD|ISC2||JTRF|WDRF|BORF|EXTRF|PORF */
/* MCUCR=SE|SM2|SM1|SM0|ISC11|ISC10|ISC01|ISC00 */
/* GIFR=INTF1|INTF0|INTF2||||| */
/* GICR=INT1|INT0|INT2||||IVSEL|IVCE */


/* external interrupt requestes (irq_set) */
#define IRQ_INT0  b1(INT0)  /* external interrupt 0 */
#define IRQ_INT1  b1(INT1)  /* external interrupt 1 */
#ifdef INT2
#define IRQ_INT2  b1(INT2)  /* external interrupt 2 */
#endif /* INT2 */

/* external interrupt request 0 modes (int0_set) */
#define INT0_MODE_LOW   (b0(ISC01)|b0(ISC00))  /* low level */
#define INT0_MODE_ANY   (b0(ISC01)|b1(ISC00))  /* any change */
#define INT0_MODE_FALL  (b1(ISC01)|b0(ISC00))  /* falling edge */
#define INT0_MODE_RISE  (b1(ISC01)|b1(ISC00))  /* rising edge */

/* external interrupt request 1 modes (int1_set) */
#define INT1_MODE_LOW   (b0(ISC11)|b0(ISC10))  /* low level */
#define INT1_MODE_ANY   (b0(ISC11)|b1(ISC10))  /* any change */
#define INT1_MODE_FALL  (b1(ISC11)|b0(ISC10))  /* falling edge */
#define INT1_MODE_RISE  (b1(ISC11)|b1(ISC10))  /* rising edge */

/* external interrupt request 2 modes (int2_set) */
#ifdef INT2
#define INT2_MODE_FALL  b0(ISC2)  /* falling edge */
#define INT2_MODE_RISE  b1(ISC2)  /* rising edge */
#endif /* INT2 */


/* interrupt request macro routines */
#define irq_app()      {sbi(GICR, IVCE); cbi(GICR, IVSEL);}  /* move interrupts to application flash section */
#define irq_boot()     {sbi(GICR, IVCE); sbi(GICR, IVSEL);}  /* move interrupts to boot flash section */
#define irq_set(sgn)   smi(GICR, sgn)             /* external interrupt request enable signals */
#define int0_set(mod)  smi(MCUCR, mod)            /* set external interrupt 0 mode */
#define int1_set(mod)  smi(MCUCR, mod)            /* set external interrupt 1 mode */
#define int0_wait()    wait_set_bit(GIFR, INTF0)  /* wait to external interrupt 0 signal */
#define int1_wait()    wait_set_bit(GIFR, INTF1)  /* wait to external interrupt 1 signal */
#ifdef INT2
#define int2_set(mod)  smi(MCUCSR, mod)           /* set external interrupt 2 mode */
#define int2_wait()    wait_set_bit(GIFR, INTF2)  /* wait to external interrupt 2 signal */
#endif /* INT2 */


/* external interrupt request 0 service routine */
#define ISR_INT0()  ISR(INT0_vect)
/* external interrupt request 1 service routine */
#define ISR_INT1()  ISR(INT1_vect)
#ifdef INT2
/* external interrupt request 2 service routine */
#define ISR_INT2()  ISR(INT2_vect)
#endif /* INT2 */


#endif /* _IRQ_H_ */

