/*
 * Two wire serial interface
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */

/*
 * Registers:
 * 
 *   TWSR: TWI status register
 *     0,1 -> TWPS0,1: prescaler
 *     3,4,5,6,7 -> TWS3,4,5,6,7: status
 * 
 *   TWAR: TWI slave address register
 *     0 -> TWGCE: general call recognition enable
 *     1,2,3,4,5,6,7 -> TWA0,1,2,3,4,5,6: slave address
 * 
 *   TWCR: TWI control register
 *     0 -> TWIE: interrupt enable
 *     2 -> TWEN: enable
 *     3 -> TWWC: write collition flag
 *     4 -> TWSTO: stop condition
 *     5 -> TWSTA: start condition
 *     6 -> TWEA: acknowledge enable
 *     7 -> TWINT: interrupt flag
 * 
 *   TWBR: TWI bitrate register
 *   TWDR: TWI data register
 */

/* TWSR=TWS7|TWS6|TWS5|TWS4|TWS3||TWPS1|TWPS0 */
/* TWAR=TWA6|TWA5|TWA4|TWA3|TWA2|TWA1|TWA0|TWGCE */
/* TWCR=TWINT|TWEA|TWSTA|TWSTO|TWWC|TWEN||TWIE */


#ifndef _TWI_H_
#define _TWI_H_ 1


#include "util.h"


/* TWI bitrate prescaler (twi_set) */
#define TWI_CK_DIV1   (b0(TWPS1)|b0(TWPS0))  /* bitrate/1 */
#define TWI_CK_DIV4   (b0(TWPS1)|b1(TWPS0))  /* bitrate/4 */
#define TWI_CK_DIV16  (b1(TWPS1)|b0(TWPS0))  /* bitrate/16 */
#define TWI_CK_DIV64  (b1(TWPS1)|b1(TWPS0))  /* bitrate/64 */

/* TWI options (twi_set) */
#define TWI_ACKNOWLEDGE      (b1(TWEA)<<8)   /* enable acknowledge pulse */
#define TWI_START_CONDITION  (b1(TWSTA)<<8)  /* start condition bit (master) */
#define TWI_STOP_CONDITION   (b1(TWSTO)<<8)  /* stop condition bit (master) */

/* TWI interrupts (twi_signal) */
#define TWI_INT  b1(TWIE)

/* general call recognition enable (twi_addr) */
#define TWI_ADDR_GENERAL  b1(TWGCE)

/* TWI macros */
#define twi_set(cnt)         {out(TWSR, (cnt)&0xFF); out(TWCR, (cnt)>>8);}  /* setup */
#define twi_signal(sgn)      smi(TWCR, sgn)             /* enable signals */
#define twi_addr(adr)        out(TWAR, adr)             /* set address register (slave) */
#define twi_bitrate(btr)     out(TWBR, (F_CPU/1000l/(btr)>=16)? ((F_CPU/1000l/(btr)-16)/2): (F_CPU/1000l/(btr)))  /* set bitrate (KHz) (max: 400KHz) (master) */
#define twi_en()             sbi(TWCR, TWEN)            /* enable */
#define twi_di()             cbi(TWCR, TWEN)            /* disable */
#define twi_write_collis()   bis(TWCR, TWWC)            /* check write collition flag */
#define twi_wait()           wait_set_bit(TWCR, TWINT)  /* wait to signal */
#define twi_data(vlu)        out(TWDR, vlu)             /* data buffer write (master) */
#define twi_data_get()       in(TWDR)                   /* data buffer read (slave) */
#define twi_status()         mis(TWSR, b1(TWS7)|b1(TWS6)|b1(TWS5)|b1(TWS4)|b1(TWS3))  /* general status */
#define twi_start()          {cmi(TWCR, b1(TWEA)|b1(TWSTA)|b1(TWSTO)); sbi(TWCR, TWSTA);}  /* send start condition (master) */
#define twi_stop()           {cmi(TWCR, b1(TWEA)|b1(TWSTA)|b1(TWSTO)); smi(TWCR, b1(TWEA)|b1(TWSTO));}  /* send stop condition (master) */


/* TWI ISR */
#define ISR_TWI()  ISR(TWI_vect)


#ifdef _TWI_H_TEST_

int main(void) {
    uint8_t i = 0;

    twi_set(TWI_ACKNOWLEDGE | TWI_STOP_CONDITION);
    twi_bitrate(100);
    twi_addr(TWI_ADDR_GENERAL);
    twi_signal(TWI_INT);
    twi_en();

    for (;;) {
        twi_wait();
        twi_data(i++);
    }

    return 0;
}

ISR_TWI() {}

#endif /* _TWI_H_TEST_*/


#endif /* _TWI_H_ */

