/*
 * Utils with other controls and statuss
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
 *          SREG: status register
 *   /-------+-------\
 *   7 6 5 4   3 2 1 0
 *   ^ ^ ^ ^   ^ ^ ^ ^
 *   | | | |   | | | +--- SREG_C: carry flag
 *   | | | |   | | +----- SREG_Z: zero flag
 *   | | | |   | +------- SREG_N: negative flag
 *   | | | |   +--------- SREG_V: two's complement overflow flag
 *   | | | +------------- SREG_S: sign bit
 *   | | +--------------- SREG_H: half carry flag
 *   | +----------------- SREG_T: bit copy storage
 *   +------------------- SREG_I: global interrupt enable
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
 *         SFIOR: special function IO register
 *   /-------+-------\
 *   7 6 5 4   3 2 1 0
 *   \-+-/ ^   ^ ^ ^ ^
 *     |   |   | | | +--- PSR10: prescaler reset timer/counter1&0
 *     |   |   | | +----- PSR2: prescaler Reset timer/counter2
 *     |   |   | +------- PUD: pull-up disable
 *     |   |   +--------- ACME: analog comparator multiplexer enable
 *     |   +------------- m8=ADHSM: ADC high speed mode
 *     +----------------- ADTS2,1,0: ADC auto trigger source
 *   
 *   OSCCAL: oscillator calibration value
 */


#ifndef _UTIL_H_
#define _UTIL_H_ 1


#include <avr/io.h>
#include <avr/interrupt.h>

/* SREG=SREG_C|SREG_Z|SREG_N|SREG_V|SREG_S|SREG_H|SREG_T|SREG_I */
/* MCUCSR=JTD|ISC2||JTRF|WDRF|BORF|EXTRF|PORF */
/* SFIOR=ADTS2|ADTS1|ADTS0|ADHSM|ACME|PUD|PSR2|PSR10 */


#ifdef cbi
#undef cbi
#endif /* cbi */

#ifdef sbi
#undef sbi
#endif /* sbi */

#define b0(bit)        (0 << (bit))          /* bit with 0 value */
#define b1(bit)        (1 << (bit))          /* bit with 1 value */
#define in(reg)        (reg)                 /* read value from io */
#define out(reg, val)  ((reg) = (val))       /* write value to io */
#define cbi(reg, bit)  ((reg) &= ~b1(bit))   /* clear bit in io */
#define sbi(reg, bit)  ((reg) |= b1(bit))    /* set bit in io */
#define ibi(reg, bit)  ((reg) ^= b1(bit))    /* invert bit in io */
#define cmi(reg, msk)  ((reg) &= ~(msk))     /* clear mask in io */
#define smi(reg, msk)  ((reg) |= (msk))      /* set mask in io */
#define imi(reg, msk)  ((reg) ^= (msk))      /* invert mask in io */
#define bic(reg, bit)  (!((reg) & b1(bit)))  /* bit is clear in io  */
#define bis(reg, bit)  ((reg) & b1(bit))     /* bit is set in io  */
#define mic(reg, msk)  (!((reg) & (msk)))    /* mask is clear in io  */
#define mis(reg, msk)  ((reg) & (msk))       /* mask is set in io  */

#define wait_clear_bit(reg, bit)   {while (bis(reg, bit));}  /* wait until bit in io is clear */
#define wait_set_bit(reg, bit)     {while (bic(reg, bit));}  /* wait until bit in io is set */
#define wait_clear_mask(reg, msk)  {while (mis(reg, msk));}  /* wait until mask in io is clear */
#define wait_set_mask(reg, msk)    {while (mic(reg, msk));}  /* wait until mask in io is set */


/* other controls and statuss */
#define osc_calibr(vlu)      out(OSCCAL, vlu)   /* oscillator calibration value (debug) */
#define pullup_en()          cbi(SFIOR, PUD)    /* pull-up enable */
#define pullup_di()          sbi(SFIOR, PUD)    /* pull-up disable */
#define timer01_clk_reset()  sbi(SFIOR, PSR10)  /* timer/counter0,1 prescaler reset */
#define timer2_clk_reset()   sbi(SFIOR, PSR2)   /* timer/counter2 prescaler reset */
#define signal_en()          sbi(SREG, SREG_I)  /* global interrupt enable */
#define signal_di()          cbi(SREG, SREG_I)  /* global interrupt disable */
#ifdef ADHSM
#define adc_high_speed_en()  sbi(SFIOR, ADHSM)  /* adc high speed mode enable */
#define adc_high_speed_di()  cbi(SFIOR, ADHSM)  /* adc high speed mode disable */
#endif /* ADHSM */
#ifdef JTD
#define jtag_en()            cbi(MCUCSR, JTD)   /* JTAG Interface enable */
#define jtag_di()            sbi(MCUCSR, JTD)   /* JTAG Interface disable */
#endif /* JTD */


#endif /* _UTIL_H_ */

