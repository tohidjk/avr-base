/*
 * Eeprom memory
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
 *          EECR: eeprom control register
 *   /-------+-------\
 *   7 6 5 4   3 2 1 0
 *             ^ ^ ^ ^
 *             | | | +--- EERE: eeprom read enable
 *             | | +----- EEWE: eeprom write enable
 *             | +------- EEMWE: eeprom master write enable
 *             +--------- EERIE: eeprom ready interrupt enable
 *   
 *   EEAR=EEARL+EEARH: eeprom address register
 *   EEDR: eeprom data register
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

