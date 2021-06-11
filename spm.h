/*
 * Store program memory (boot loader)
 * Copyright (C) 2013-2021 Tohid Jafarzadeh <tohid.jk@gmail.com>
 * License GNU GPLv2
 * 2021-06-11 BETA
 */

/**
 * Registers:
 * 
 *        SPMCR: store program memory control register
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^   ^ ^ ^ ^ ^
 *   | |   | | | | +--- SPMEN: enable
 *   | |   | | | +----- PGERS: page erase
 *   | |   | | +------- PGWRT: page write
 *   | |   | +--------- BLBSET: boot lock bit set
 *   | |   +----------- RWWSRE: read while write section read enable
 *   | +--------------- RWWSB: read while write section busy
 *   +----------------- SPMIE: interrupt enable
 */


#ifndef _SPM_H_
#define _SPM_H_ 1


#include "util.h"

/* SPMCR=SPMIE|RWWSB||RWWSRE|BLBSET|PGWRT|PGERS|SPMEN */


/* store program memory interrupts (spm_signal) */
#define SPM_INT_RDY  b1(SPMIE)  /* store program memory ready */


/* store program memory macro routines */
#define BOOTMEM           __attribute__((section(".bootloader")))  /* set bootloader section for code */
#define spm_en()          sbi(SPMCR, SPMEN)                 /* SPM enable */
#define spm_di()          cbi(SPMCR, SPMEN)                 /* SPM disable */
#define spm_boot_lock(x)  {__asm__ __volatile__ (::"t"(x):"r0"); sbi(SPMCR, BLBSET);}  /* set boot lock */
#define spm_signal(sgn)   smi(SPMCR, sgn)                   /* SPM enable signals */
#define spm_addr(adr)     {__asm__ __volatile__ (::"z"(adr):"r30","r31");}  /* SPM set address */
#define spm_erase()       sbi(SPMCR, PGERS)                 /* SPM erase address */
#define spm_write()       sbi(SPMCR, PGWRT)                 /* SPM write address */
#define spm_wait()        wait_clear_bit(SPMCR, SPMEN)      /* SPM wait to end */
#define spm_rww_wait()    wait_clear_bit(SPMCR, RWWSB)      /* SPM wait to RWW end */
#define spm_rww_en()      smi(SPMCR, b1(RWWSB)|b1(RWWSRE))  /* SPM RWW enable */


/* store program memory ready interrupt service routine */
#define ISR_SPM_RDY()  ISR(SPM_RDY_vect)


#endif /* _SPM_H_ */

