/*
 * Universal synchronous and asynchronous serial receiver and transmitter
 * Copyright 2013-2021 tohid.jk
 * License GNU GPLv2
 * 2021-06-19 beta
 */

/*
 * Registers:
 * 
 *   UCSRB: USART control and status register B
 *     0 -> TXB8: transmitt data bit 8
 *     1 -> RXB8: receive data bit 8
 *     2 -> UCSZ2: character size
 *     3 -> TXEN: transmitter enable
 *     4 -> RXEN: receiver enable
 *     5 -> UDRIE: data register empty interrupt enable
 *     6 -> TXCIE: transmitt complete interrupt enable
 *     7 -> RXCIE: receive complete interrupt enable
 * 
 *   UCSRA: USART control and status register A
 *     0 -> MPCM: multi-processor communication mode
 *     1 -> U2X: double the transmission speed
 *     2 -> PE: parity error
 *     3 -> DOR: data over-run
 *     4 -> FE: framing error
 *     5 -> UDRE: data register empty
 *     6 -> TXC: transmitt complete
 *     7 -> RXC: receive complete
 * 
 *   UCSRC: USART control and status register C
 *     0 -> UCPOL: clock polarity
 *     1,2 -> UCSZ0,1: character size
 *     3 -> USBS: stop bit select
 *     4,5 -> UPM0,1: parity mode
 *     6 -> UMSEL: mode select
 *     7 -> URSEL: register select
 * 
 *   UBRR=UBRRL+UBRRH: USART baud rate registers
 *   UDR: USART data register
 */

/* UCSRB=RXCIE|TXCIE|UDRIE|RXEN|TXEN|UCSZ2|RXB8|TXB8 */
/* UCSRA=RXC|TXC|UDRE|FE|DOR|PE|U2X|MPCM */
/* UCSRC=URSEL|UMSEL|UPM1|UPM0|USBS|UCSZ1|UCSZ0|UCPOL */


#ifndef _USART_H_
#define _USART_H_ 1


#include "util.h"


/* USART options (usart_set) */
#define USART_BAUD_DOUBLE  (b1(U2X)<<8)   /* double transmission speed (Asynchronous) */
#define USART_MULTI_PROC   (b1(MPCM)<<8)  /* multi-processor communication mode */
#define USART_RX           b1(RXEN)       /* receiver enable */
#define USART_TX           b1(TXEN)       /* transmitter enable */
#define USART_SYNC         ((uint32_t)b1(UMSEL)<<16)  /* synchronous mode select */
#define USART_CK_POLARITY  ((uint32_t)b1(UCPOL)<<16)  /* clock polarity (synchronous) */
#define USART_REG_SELECT   ((uint32_t)b1(URSEL)<<16)  /* register select for use UCSRC */

/* USART parity modes (usart_set) */
#define USART_PARITY_NONE  ((uint32_t)(b0(UPM1)|b0(UPM0))<<16)  /* none */
#define USART_PARITY_EVEN  ((uint32_t)(b1(UPM1)|b0(UPM0))<<16)  /* even */
#define USART_PARITY_ODD   ((uint32_t)(b1(UPM1)|b1(UPM0))<<16)  /* odd */

/* USART stop bit size (usart_set) */
#define USART_STOP_1BIT  ((uint32_t)b0(USBS)<<16)  /* 1bit */
#define USART_STOP_2BIT  ((uint32_t)b1(USBS)<<16)  /* 2bit */

/* USART data bit size (usart_set) */
#define USART_DATA_5BIT  (b0(UCSZ2)|((uint32_t)(b0(UCSZ1)|b0(UCSZ0))<<16))  /* 5bit */
#define USART_DATA_6BIT  (b0(UCSZ2)|((uint32_t)(b0(UCSZ1)|b1(UCSZ0))<<16))  /* 6bit */
#define USART_DATA_7BIT  (b0(UCSZ2)|((uint32_t)(b1(UCSZ1)|b0(UCSZ0))<<16))  /* 7bit */
#define USART_DATA_8BIT  (b0(UCSZ2)|((uint32_t)(b1(UCSZ1)|b1(UCSZ0))<<16))  /* 8bit */
#define USART_DATA_9BIT  (b1(UCSZ2)|((uint32_t)(b1(UCSZ1)|b1(UCSZ0))<<16))  /* 9bit */

/* USART standard baud rates (usart_baud) */
/* 110,300,600,1200,2400,4800,9600,14400,19200,28800,38400,
 * 56000,57600,76800,115200,230400,250000,500000,1000000 */
#define USART_BAUD_9600    9600
#define USART_BAUD_19200   19200
#define USART_BAUD_38400   38400
#define USART_BAUD_57600   57600
#define USART_BAUD_115200  115200

/* USART interrupts (usart_signal) */
#define USART_INT_RXC    b1(RXCIE)  /* receive complete */
#define USART_INT_EMPTY  b1(UDRIE)  /* data register empty */
#define USART_INT_TXC    b1(TXCIE)  /* transmitt complete */


/* USART macros */
#define usart_set(cnt)        {out(UCSRB, (cnt)&0xFF); out(UCSRA, ((cnt)>>8)&0xFF); out(UCSRC, (cnt)>>16);}  /* setup */
#define _usart_ubbr(bdr)      ((F_CPU+((uint32_t)(bdr))*8)/(((uint32_t)(bdr))*16)-1)  /* UBBR value calculate */
#define usart_baud(bdr)       {out(UBRRL, _usart_ubbr(bdr)&0xFF); out(UBRRH, _usart_ubbr(bdr)>>8);}  /* for Asynchronous and synchronous (master) */
// #define usart_setbaud()       {out(UBRRL, UBRRL_VALUE); out(UBRRH, UBRRH_VALUE);}  /* <util/setbaud.h> for Asynchronous and synchronous (master) */
#define usart_signal(sgn)     smi(UCSRB, sgn)                /* enable signals */
#define usart_tx_data(dta)    {out(UDR, dta); bis(dta, 8)? sbi(UCSRB, TXB8): cbi(UCSRB, TXB8);}  /* send data */
#define usart_rx_data()       (in(UDR)|(bis(UCSRB, RXB8)? 0x100: 0))  /* read received data */
#define usart_frame_error()   bis(UCSRA, FE)                 /* check receive framing error */
#define usart_data_overrun()  bis(UCSRA, DOR)                /* check receive data over run */
#define usart_parity_error()  bis(UCSRA, PE)                 /* check receive parity error */
#define usart_error()         mis(UCSRA, b1(FE)|b1(DOR)|b1(PE))  /* check receive framing error and data over run and parity error */
#define usart_empty_wait()    wait_set_bit(UCSRA, UDRE)      /* wait to data register empty */
#define usart_rx_wait()       wait_set_bit(UCSRA, RXC)       /* wait to receive complete */
#define usart_tx_wait()       wait_set_bit(UCSRA, TXC)       /* wait to transmitt complete */
#define usart_di()            cmi(UCSRB, b1(TXEN)|b1(RXEN))  /* disable */


/* USART receive complete ISR */
#define ISR_USART_RXC()    ISR(USART_RXC_vect)
/* USART data register empty ISR */
#define ISR_USART_EMPTY()  ISR(USART_UDRE_vect)
/* USART transmit complete ISR */
#define ISR_USART_TXC()    ISR(USART_TXC_vect)


#ifdef _USART_H_TEST_

int main(void) {
    uint8_t i = 0;

    PORTB = 0;
    DDRB = ~0;

    usart_set(USART_RX | USART_TX | USART_REG_SELECT | USART_DATA_8BIT);
    usart_baud(9600);
    usart_signal(USART_INT_RXC);
    sei();

    for (;;) {
        usart_empty_wait();
        usart_tx_data(i++);
    }

    return 0;
}

ISR_USART_RXC() {
    if (!usart_error())
        PORTB = usart_rx_data();
}

#endif /* _USART_H_TEST_ */


#endif /* _USART_H_ */

