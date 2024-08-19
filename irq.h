/*
 * External interrupt requestes
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */

/*
 * Registers:
 * 
 *   MCUCSR: MCU control and status register
 *     0 -> PORF: power-on reset flag
 *     1 -> EXTRF: external reset flag
 *     2 -> BORF: brown-out reset flag
 *     3 -> WDRF: watchdog reset flag
 *     4 -> JTRF: JTAG reset flag
 *     6 -> ISC2: external interrupt 2 sense control
 *     7 -> JTD: JTAG interface disable
 * 
 *   MCUCR: MCU control register
 *     0,1 -> ISC00,1: external interrupt 0 sense control
 *     2,3 -> ISC10,1: external interrupt 1 sense control
 *     4,5,6 -> SM0,1,2: sleep mode
 *     7 -> SE: sleep enable
 * 
 *   GIFR: general interrupt flag register
 *     5 -> INTF2: external interrupt 2 flag
 *     6 -> INTF0: external interrupt 0 flag
 *     7 -> INTF1: external interrupt 1 flag
 * 
 *   GICR: general interrupt control register
 *     0 -> IVCE: interrupt vector change enable
 *     1 -> IVSEL: interrupt vector select
 *     5 -> INT2: external interrupt request 2 enable
 *     6 -> INT0: external interrupt request 0 enable
 *     7 -> INT1: external interrupt request 1 enable
 */

/* MCUCSR=JTD|ISC2||JTRF|WDRF|BORF|EXTRF|PORF */
/* MCUCR=SE|SM2|SM1|SM0|ISC11|ISC10|ISC01|ISC00 */
/* GIFR=INTF1|INTF0|INTF2||||| */
/* GICR=INT1|INT0|INT2||||IVSEL|IVCE */


#ifndef _IRQ_H_
#define _IRQ_H_ 1


#include "util.h"


/* IRQ select (irq_set) */
#define IRQ_INT0  b1(INT0)  /* external interrupt 0 */
#define IRQ_INT1  b1(INT1)  /* external interrupt 1 */
#ifdef INT2
#define IRQ_INT2  b1(INT2)  /* external interrupt 2 */
#endif /* INT2 */

/* IRQ0 modes (int0_set) */
#define IRQ_INT0_MODE_LOW   (b0(ISC01)|b0(ISC00))  /* low level */
#define IRQ_INT0_MODE_ANY   (b0(ISC01)|b1(ISC00))  /* any change */
#define IRQ_INT0_MODE_FALL  (b1(ISC01)|b0(ISC00))  /* falling edge */
#define IRQ_INT0_MODE_RISE  (b1(ISC01)|b1(ISC00))  /* rising edge */

/* IRQ1 modes (int1_set) */
#define IRQ_INT1_MODE_LOW   (b0(ISC11)|b0(ISC10))  /* low level */
#define IRQ_INT1_MODE_ANY   (b0(ISC11)|b1(ISC10))  /* any change */
#define IRQ_INT1_MODE_FALL  (b1(ISC11)|b0(ISC10))  /* falling edge */
#define IRQ_INT1_MODE_RISE  (b1(ISC11)|b1(ISC10))  /* rising edge */

/* IRQ2 modes (int2_set) */
#ifdef INT2
#define IRQ_INT2_MODE_FALL  b0(ISC2)  /* falling edge */
#define IRQ_INT2_MODE_RISE  b1(ISC2)  /* rising edge */
#endif /* INT2 */


/* IRQ macros */
#define irq_app()          {sbi(GICR, IVCE); cbi(GICR, IVSEL);}  /* move interrupts to application flash section */
#define irq_boot()         {sbi(GICR, IVCE); sbi(GICR, IVSEL);}  /* move interrupts to boot flash section */
#define irq_set(sgn)       smi(GICR, sgn)             /* enable signals */
#define irq_int0_set(mod)  smi(MCUCR, mod)            /* set IRQ0 mode */
#define irq_int0_wait()    wait_set_bit(GIFR, INTF0)  /* wait to IRQ0 signal */
#define irq_int1_set(mod)  smi(MCUCR, mod)            /* set IRQ1 mode */
#define irq_int1_wait()    wait_set_bit(GIFR, INTF1)  /* wait to IRQ1 signal */
#ifdef INT2
#define irq_int2_set(mod)  smi(MCUCSR, mod)           /* set IRQ2 mode */
#define irq_int2_wait()    wait_set_bit(GIFR, INTF2)  /* wait to IRQ2 signal */
#endif /* INT2 */


/* IRQ0 */
#define ISR_INT0()  ISR(INT0_vect)
/* IRQ1 */
#define ISR_INT1()  ISR(INT1_vect)
#ifdef INT2
/* IRQ2 */
#define ISR_INT2()  ISR(INT2_vect)
#endif /* INT2 */


#ifdef _IRQ_H_TEST_

int main(void) {
    PORTB = 0;
    DDRB = ~0;

    irq_set(IRQ_INT0 | IRQ_INT1);
    irq_int0_set(IRQ_INT0_MODE_FALL);
    irq_int1_set(IRQ_INT1_MODE_RISE);
    sei();

    for (;;) {
        irq_int1_wait();
        PORTB ^= 2;
    }

    return 0;
}

ISR_INT0() {
    PORTB ^= 1;
}

ISR_INT1() {}

#endif /* _IRQ_H_TEST_ */


#endif /* _IRQ_H_ */

