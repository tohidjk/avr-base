/*
 * Store program memory (boot loader)
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */

/*
 * Registers:
 * 
 *   SPMCR: SPM control register
 *     0 -> SPMEN: enable
 *     1 -> PGERS: page erase
 *     2 -> PGWRT: page write
 *     3 -> BLBSET: boot lock bit set
 *     4 -> RWWSRE: read while write section read enable
 *     6 -> RWWSB: read while write section busy
 *     7 -> SPMIE: interrupt enable
 */

/* SPMCR=SPMIE|RWWSB||RWWSRE|BLBSET|PGWRT|PGERS|SPMEN */


#ifndef _SPM_H_
#define _SPM_H_ 1


#include "util.h"


/* SPM interrupts (spm_signal) */
#define SPM_INT_RDY  b1(SPMIE)  /* ready */


/* SPM macros */
#define BOOTMEM           __attribute__((section(".bootloader")))  /* set bootloader section for code */
#define spm_en()          sbi(SPMCR, SPMEN)                 /* enable */
#define spm_di()          cbi(SPMCR, SPMEN)                 /* disable */
#define spm_boot_lock(x)  {__asm__ __volatile__ (::"t"(x):"r0"); sbi(SPMCR, BLBSET);}  /* set boot lock */
#define spm_signal(sgn)   smi(SPMCR, sgn)                   /* enable signals */
#define spm_addr(adr)     {__asm__ __volatile__ (::"z"(adr):"r30","r31");}  /* set address */
#define spm_erase()       sbi(SPMCR, PGERS)                 /* erase address */
#define spm_write()       sbi(SPMCR, PGWRT)                 /* write address */
#define spm_wait()        wait_clear_bit(SPMCR, SPMEN)      /* wait to end */
#define spm_rww_wait()    wait_clear_bit(SPMCR, RWWSB)      /* wait to RWW end */
#define spm_rww_en()      smi(SPMCR, b1(RWWSB)|b1(RWWSRE))  /* RWW enable */


/* SPM ready ISR */
#define ISR_SPM_RDY()  ISR(SPM_RDY_vect)


#endif /* _SPM_H_ */

