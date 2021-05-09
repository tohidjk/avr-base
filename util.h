/*
 * Utils
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


#ifndef _UTIL_H_
#define _UTIL_H_ 1


#include <avr/io.h>
#include <avr/interrupt.h>


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


#endif /* _UTIL_H_ */

