/*
 * Serial peripheral interface
 * Copyright (C) 2013-2021 Tohid Jafarzadeh <tohid.jk@gmail.com>
 * License GNU GPLv2
 * 2021-06-11 BETA
 */

/**
 * Registers:
 * 
 *        SPCR: SPI control register
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^ ^ ^ ^ ^ \+/
 *   | | | | | |  +---- SPR1,0: clock rate
 *   | | | | | +------- CPHA: clock Phase
 *   | | | | +--------- CPOL: clock polarity
 *   | | | +----------- MSTR: master/slave select
 *   | | +------------- DORD: data order
 *   | +--------------- SPE: enable
 *   +----------------- SPIE: interrupt enable
 * 
 *        SPSR: SPI status register
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^           ^
 *   | |           +--- SPI2X: double speed
 *   | +--------------- WCOL: write collision flag
 *   +----------------- SPIF: interrupt flag
 * 
 *   SPDR: data register
 */

/*
 * Example:
 * 
 * #include "spi.h"
 * 
 * int main(void) {
 *   unsigned char i = 0;
 * 
 *   spi_set(SPI_MASTER | SPI_CK_DIV64);
 *   spi_signal(SPI_INT_STC);
 *   spi_en();
 *   sei();
 * 
 *   for (;;) {
 *     spi_wait();
 *     spi_data(i++);
 *   }
 * 
 *   return 0;
 * }
 * 
 * ISR_SPI_STC() {}
 */


#ifndef _SPI_H_
#define _SPI_H_ 1


#include "util.h"

/* SPCR=SPIE|SPE|DORD|MSTR|CPOL|CPHA|SPR1|SPR0 */
/* SPSR=SPIF|WCOL||||||SPI2X */


/* SPI clock prescaler for master (spi_set) */
#define SPI_CK_DIV2    ((b1(SPI2X)<<8)|b0(SPR1)|b0(SPR0))  /* F_CPU/2 */
#define SPI_CK_DIV4    ((b0(SPI2X)<<8)|b0(SPR1)|b0(SPR0))  /* F_CPU/4 */
#define SPI_CK_DIV8    ((b1(SPI2X)<<8)|b0(SPR1)|b1(SPR0))  /* F_CPU/8 */
#define SPI_CK_DIV16   ((b0(SPI2X)<<8)|b0(SPR1)|b1(SPR0))  /* F_CPU/16 */
#define SPI_CK_DIV32   ((b1(SPI2X)<<8)|b1(SPR1)|b0(SPR0))  /* F_CPU/32 */
#define SPI_CK_DIV64   ((b0(SPI2X)<<8)|b1(SPR1)|b0(SPR0))  /* F_CPU/64 */
/*#define SPI_CK_DIV64   ((b1(SPI2X)<<8)|b1(SPR1)|b1(SPR0))*/  /* F_CPU/64 */
#define SPI_CK_DIV128  ((b0(SPI2X)<<8)|b1(SPR1)|b1(SPR0))  /* F_CPU/128 */

/* SPI options (spi_set) */
#define SPI_DATA_ORDER   b1(DORD)  /* data order */
#define SPI_SLAVE        b0(MSTR)  /* slave select */
#define SPI_MASTER       b1(MSTR)  /* master select */
#define SPI_CK_POLARITY  b1(CPOL)  /* clock polarity */
#define SPI_CK_PHASE     b1(CPHA)  /* clock phase */

/* SPI interrupts (spi_signal) */
#define SPI_INT_STC  b1(SPIE)  /* SPI serial transfer complete */


/* SPI macro routines */
#define spi_set(cnt)        {out(SPCR, cnt); out(SPSR, (cnt)>>8);}  /* set SPI */
#define spi_signal(sgn)     smi(SPCR, sgn)            /* SPI enable signals */
#define spi_en()            sbi(SPCR, SPE)            /* SPI enable */
#define spi_di()            cbi(SPCR, SPE)            /* SPI disable */
#define spi_write_collis()  bis(SPSR, WCOL)           /* SPI check write collision flag */
#define spi_wait()          wait_set_bit(SPSR, SPIF)  /* wait to SPI transfer complete signal */
#define spi_data(vlu)       out(SPDR, vlu)            /* SPI data buffer write (master) */
#define spi_data_get()      in(SPDR)                  /* SPI data buffer read (slave) */


/* SPI serial transfer complete interrupt service routine */
#define ISR_SPI_STC()  ISR(SPI_STC_vect)


#endif /* _SPI_H_ */

