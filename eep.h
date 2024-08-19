/*
 * Eeprom memory
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */

/*
 * Registers:
 * 
 *   EECR: eeprom control register
 *     0 -> EERE: read enable
 *     1 -> EEWE: write enable
 *     2 -> EEMWE: master write enable
 *     3 -> EERIE: ready interrupt enable
 * 
 *   EEAR=EEARL+EEARH: eeprom address register
 *   EEDR: eeprom data register
 */

/* EECR=||||EERIE|EEMWE|EEWE|EERE */


#ifndef _EEP_H_
#define _EEP_H_ 1


#include "util.h"


/* eeprom interrupts (eep_signal) */
#define EEP_INT_RDY  b1(EERIE)  /* ready */


/* eeprom macros */
#define EEPMEM           __attribute__((section(".eeprom")))  /* set eeprom section for variables */
#define eep_signal(sgn)  smi(EECR, sgn)                 /* enable signals */
#define eep_addr(adr)    out(EEAR, adr)                 /* set address */
#define eep_data(dta)    out(EEDR, dta)                 /* set data value */
#define eep_data_get()   in(EEDR)                       /* readed value */
#define eep_read()       sbi(EECR, EERE)                /* read data from address */
#define eep_write()      smi(EECR, b1(EEMWE)|b1(EEWE))  /* write data to address */
#define eep_wait()       wait_set_bit(EECR, EEWE)       /* wait to write enable */

#define eep_write_byte(adr, vlu)  {eep_wait(); eep_addr((uint16_t)adr); eep_data(vlu); eep_write();}
#define eep_read_byte(adr)        ({eep_wait(); eep_addr((uint16_t)adr); eep_read(); eep_data_get();})


/* eeprom ready ISR */
#define ISR_EEP_RDY()  ISR(EE_RDY_vect)


#ifdef _EEP_H_TEST_

#include <util/delay.h>

EEPMEM uint8_t i = 0;

int main(void) {
    PORTB = 0;
    DDRB = ~0;

    for (;;) {
        PORTB = eep_read_byte(&i);
        eep_write_byte(&i, PORTB+1);
        _delay_ms(500);
    }

    return 0;
}

#endif /* _EEP_H_TEST_ */


#endif /* _EEP_H_ */

