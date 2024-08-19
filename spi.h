/*
 * Serial peripheral interface
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */

/*
 * Registers:
 * 
 *   SPCR: SPI control register
 *     0,1 -> SPR0,1: clock rate
 *     2 -> CPHA: clock Phase
 *     3 -> CPOL: clock polarity
 *     4 -> MSTR: master/slave select
 *     5 -> DORD: data order
 *     6 -> SPE: enable
 *     7 -> SPIE: interrupt enable
 * 
 *   SPSR: SPI status register
 *     0 -> SPI2X: double speed
 *     6 -> WCOL: write collision flag
 *     7 -> SPIF: interrupt flag
 * 
 *   SPDR: SPI data register
 */

/* SPCR=SPIE|SPE|DORD|MSTR|CPOL|CPHA|SPR1|SPR0 */
/* SPSR=SPIF|WCOL||||||SPI2X */


#ifndef _SPI_H_
#define _SPI_H_ 1


#include "util.h"


/* SPI clock prescaler for master (spi_set) */
#define SPI_CK_DIV2    ((b1(SPI2X)<<8)|b0(SPR1)|b0(SPR0))  /* F_CPU/2 */
#define SPI_CK_DIV4    ((b0(SPI2X)<<8)|b0(SPR1)|b0(SPR0))  /* F_CPU/4 */
#define SPI_CK_DIV8    ((b1(SPI2X)<<8)|b0(SPR1)|b1(SPR0))  /* F_CPU/8 */
#define SPI_CK_DIV16   ((b0(SPI2X)<<8)|b0(SPR1)|b1(SPR0))  /* F_CPU/16 */
#define SPI_CK_DIV32   ((b1(SPI2X)<<8)|b1(SPR1)|b0(SPR0))  /* F_CPU/32 */
#define SPI_CK_DIV64   ((b0(SPI2X)<<8)|b1(SPR1)|b0(SPR0))  /* F_CPU/64 */
// #define SPI_CK_DIV64   ((b1(SPI2X)<<8)|b1(SPR1)|b1(SPR0))  /* F_CPU/64 */
#define SPI_CK_DIV128  ((b0(SPI2X)<<8)|b1(SPR1)|b1(SPR0))  /* F_CPU/128 */

/* SPI options (spi_set) */
#define SPI_DATA_ORDER   b1(DORD)  /* data order */
#define SPI_SLAVE        b0(MSTR)  /* slave select */
#define SPI_MASTER       b1(MSTR)  /* master select */
#define SPI_CK_POLARITY  b1(CPOL)  /* clock polarity */
#define SPI_CK_PHASE     b1(CPHA)  /* clock phase */

/* SPI interrupts (spi_signal) */
#define SPI_INT_STC  b1(SPIE)  /* serial transfer complete */


/* SPI macros */
#define spi_set(cnt)        {out(SPCR, (cnt)&0xFF); out(SPSR, (cnt)>>8);}  /* setup */
#define spi_signal(sgn)     smi(SPCR, sgn)            /* enable signals */
#define spi_en()            sbi(SPCR, SPE)            /* enable */
#define spi_di()            cbi(SPCR, SPE)            /* disable */
#define spi_write_collis()  bis(SPSR, WCOL)           /* check write collision flag */
#define spi_wait()          wait_set_bit(SPSR, SPIF)  /* wait to transfer complete signal */
#define spi_data(vlu)       out(SPDR, vlu)            /* data buffer write (master) */
#define spi_data_get()      in(SPDR)                  /* data buffer read (slave) */


/* SPI serial transfer complete ISR */
#define ISR_SPI_STC()  ISR(SPI_STC_vect)


#ifdef _SPI_H_TEST_

int main(void) {
    uint8_t i = 0;

    spi_set(SPI_MASTER | SPI_CK_DIV64);
    spi_signal(SPI_INT_STC);
    spi_en();
    sei();

    for (;;) {
        spi_wait();
        spi_data(i++);
    }

    return 0;
}

ISR_SPI_STC() {}

#endif /* _SPI_H_TEST_ */


#endif /* _SPI_H_ */

