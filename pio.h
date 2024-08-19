/*
 * Parallel input/ouput ports
 * Copyright 2013-2022 tohid.jk
 * License GNU GPLv2
 * 2022-03-04 beta
 */

/*
 * Registers:
 * 
 *   PORTx: port x output data register
 *   DDRx: port x data direction register
 *   PINx: port x input data register
 */


#ifndef _PIO_H_
#define _PIO_H_ 1


#include "util.h"


/* for PORT */
#define PIO_CLEAR     0
#define PIO_OFF       0
#define PIO_LOW       0
#define PIO_PULLDOWN  0
#define PIO_DOWN      0
#define PIO_SET       1
#define PIO_ON        1
#define PIO_HIGH      1
#define PIO_PULLUP    1
#define PIO_UP        1

/* for DDR */
#define PIO_INPUT     0
#define PIO_IN        0
#define PIO_OUTPUT    1
#define PIO_OUT       1

/* for PORT and DDR */
#define PIO_INVERT    -1
#define PIO_REVERSE   -1
#define PIO_INVERSE   -1
#define PIO_TOGGLE    -1


/* PIO macros */
#define PIOB(reg, bit, x)  {if((x)==0) cbi(reg,bit); else if((x)==1) sbi(reg,bit); else if((x)==-1) ibi(reg,bit);}  /* change bit in register or variable */
#define PORT(prt, bit, x)  PIOB(PORT ## prt, bit, x)  /* change bit in data register */
#define DDR(prt, bit, x)   PIOB(DDR ## prt, bit, x)   /* change bit in data direction register */
#define PIN(pin, bit)      ((PIN ## pin) & b1(bit))   /* get bit in input pins register */


#ifdef _PIO_H_TEST_

int main(void) {
    DDR(B, 0, PIO_INPUT);
    DDR(B, 1, PIO_INPUT);
    DDR(B, 2, PIO_INPUT);
    DDR(B, 3, PIO_INPUT);
    DDR(B, 4, PIO_OUTPUT);
    DDR(B, 5, PIO_OUTPUT);
    DDR(B, 6, PIO_OUTPUT);
    DDR(B, 7, PIO_OUTPUT);

    PORT(B, 0, PIO_PULLDOWN);
    PORT(B, 1, PIO_PULLDOWN);
    PORT(B, 2, PIO_PULLUP);
    PORT(B, 3, PIO_PULLUP);
    PORT(B, 4, PIO_LOW);
    PORT(B, 5, PIO_LOW);
    PORT(B, 6, PIO_HIGH);
    PORT(B, 7, PIO_HIGH);

    for (;;) {
        if (PIN(B, 0)) PORT(B, 4, PIO_INVERT);
        if (PIN(B, 1)) PORT(B, 5, PIO_INVERT);
        if (!PIN(B, 2)) PORT(B, 6, PIO_INVERT);
        if (!PIN(B, 3)) PORT(B, 7, PIO_INVERT);
    }

    return 0;
}

#endif /* _PIO_H_TEST_*/


#endif /* _PIO_H_ */

