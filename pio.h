/*
 * Parallel input/ouput ports
 * Copyright (C) 2013-2021 Tohid Jafarzadeh <tohid.jk@gmail.com>
 * License GNU GPLv2
 * 2021-06-12 BETA
 */

/**
 * Registers:
 * 
 *   PORTx: port x output data register
 *   DDRx: port x data direction register
 *   PINx: port x input data pins
 */

/*
 * Example:
 * 
 * #include "pio.h"
 * 
 * int main(void) {
 *   DDR(B, 0, INPUT);
 *   DDR(B, 1, INPUT);
 *   DDR(B, 2, INPUT);
 *   DDR(B, 3, INPUT);
 *   DDR(B, 4, OUTPUT);
 *   DDR(B, 5, OUTPUT);
 *   DDR(B, 6, OUTPUT);
 *   DDR(B, 7, OUTPUT);
 * 
 *   PORT(B, 0, PULLDOWN);
 *   PORT(B, 1, PULLDOWN);
 *   PORT(B, 2, PULLUP);
 *   PORT(B, 3, PULLUP);
 *   PORT(B, 4, LOW);
 *   PORT(B, 5, LOW);
 *   PORT(B, 6, HIGH);
 *   PORT(B, 7, HIGH);
 * 
 *   for (;;) {
 *     if (PIN(B, 0)) PORT(B, 4, INVERT);
 *     if (PIN(B, 1)) PORT(B, 5, INVERT);
 *     if (!PIN(B, 2)) PORT(B, 6, INVERT);
 *     if (!PIN(B, 3)) PORT(B, 7, INVERT);
 *   }
 * 
 *   return 0;
 * }
 */


#ifndef _PIO_H_
#define _PIO_H_ 1


#include "util.h"


/* for PORT */
#define CLEAR     0
#define CLR       0
#define OFF       0
#define LOW       0
#define PULLDOWN  0
#define DOWN      0
#define SET       1
#define ST        1
#define ON        1
#define HIGH      1
#define PULLUP    1
#define UP        1
/* for DDR */
#define INPUT     0
#define IN        0
#define OUTPUT    1
#define OUT       1
/* for PORT and DDR */
#define INVERT    -1
#define INV       -1
#define REVERSE   -1
#define REV       -1
#define INVERSE   -1
#define TOGGLE    -1


/* io ports macro routines */
#define IOB(reg, bit, x)   {if((x)==0) cbi(reg,bit); else if((x)==1) sbi(reg,bit); else if((x)==-1) ibi(reg,bit);}  /* change bit in register or variable */
#define PORT(prt, bit, x)  IOB(PORT ## prt, bit, x)  /* change bit in data register */
#define DDR(prt, bit, x)   IOB(DDR ## prt, bit, x)   /* change bit in data direction register */
#define PIN(pin, bit)      ((PIN ## pin) & b1(bit))  /* get bit in input pins register */


#endif /* _PIO_H_ */

