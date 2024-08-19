/*
 * Utils
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */


#ifndef _UTIL_H_
#define _UTIL_H_ 1


#include <stdint.h>
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

