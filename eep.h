/*
 * Eeprom memory
 * Copyright (C) 2013-2021 Tohid Jafarzadeh <tohid.jk@gmail.com>
 * License GNU GPLv2
 * 2021-06-12 BETA
 */

/**
 * Registers:
 * 
 *        EECR: eeprom control register
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *           ^ ^ ^ ^
 *           | | | +--- EERE: read enable
 *           | | +----- EEWE: write enable
 *           | +------- EEMWE: master write enable
 *           +--------- EERIE: ready interrupt enable
 * 
 *   EEAR=EEARL+EEARH: address register
 *   EEDR: data register
 */

/*
 * Example:
 * 
 * #include <avr/io.h>
 * #include <util/delay.h>
 * #include "eep.h"
 * 
 * EEPMEM unsigned char i = 0;
 * 
 * int main(void) {
 *   PORTB = 0;
 *   DDRB = ~0;
 * 
 *   for (;;) {
 *     PORTB = eep_read_byte(&i);
 *     eep_write_byte(&i, PORTB+1);
 *     _delay_ms(500);
 *   }
 * 
 *   return 0;
 * }
 */


#ifndef _EEP_H_
#define _EEP_H_ 1


#include "util.h"

/* EECR=||||EERIE|EEMWE|EEWE|EERE */


/* eeprom interrupts (eep_signal) */
#define EEP_INT_RDY  b1(EERIE)  /* eeprom ready interrupt */


/* eeprom macro routines */
#define EEPMEM           __attribute__((section(".eeprom")))  /* set eeprom section for variables */
#define eep_signal(sgn)  smi(EECR, sgn)                 /* eeprom enable signals */
#define eep_addr(adr)    out(EEAR, adr)                 /* eeprom set address */
#define eep_data(dta)    out(EEDR, dta)                 /* eeprom set data value */
#define eep_data_get()   in(EEDR)                       /* eeprom readed value */
#define eep_read()       sbi(EECR, EERE)                /* eeprom read data from address */
#define eep_write()      smi(EECR, b1(EEMWE)|b1(EEWE))  /* eeprom write data to address */
#define eep_wait()       wait_set_bit(EECR, EEWE)       /* wait to eeprom write enable */

#define eep_write_byte(adr, vlu)  {eep_wait(); eep_addr(adr); eep_data(vlu); eep_write();}
#define eep_read_byte(adr)        ({eep_wait(); eep_addr(adr); eep_read(); eep_data_get();})


/* eeprom ready interrupt service routine */
#define ISR_EEP_RDY()  ISR(EE_RDY_vect)


#endif /* _EEP_H_ */

