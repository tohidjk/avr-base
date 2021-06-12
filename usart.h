/*
 * Universal synchronous and asynchronous serial receiver and transmitter
 * Copyright (C) 2013-2021 Tohid Jafarzadeh <tohid.jk@gmail.com>
 * License GNU GPLv2
 * 2021-06-12 BETA
 */

/**
 * Registers:
 * 
 *        UCSRB: USART control and status register B
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^ ^ ^ ^ ^ ^ ^
 *   | | | | | | | +--- TXB8: transmitt data bit 8
 *   | | | | | | +----- RXB8: receive data bit 8
 *   | | | | | +------- UCSZ2: character size
 *   | | | | +--------- TXEN: transmitter enable
 *   | | | +----------- RXEN: receiver enable
 *   | | +------------- UDRIE: data register empty interrupt enable
 *   | +--------------- TXCIE: transmitt complete interrupt enable
 *   +----------------- RXCIE: receive complete interrupt enable
 * 
 *        UCSRA: USART control and status register A
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^ ^ ^ ^ ^ ^ ^
 *   | | | | | | | +--- MPCM: multi-processor communication mode
 *   | | | | | | +----- U2X: double the transmission speed
 *   | | | | | +------- PE: parity error
 *   | | | | +--------- DOR: data over-run
 *   | | | +----------- FE: framing error
 *   | | +------------- UDRE: data register empty
 *   | +--------------- TXC: transmitt complete
 *   +----------------- RXC: receive complete
 * 
 *        UCSRC: USART control and status register C
 *   /------+------\
 *   7 6 5 4 3 2 1 0
 *   ^ ^ \+/ ^ \+/ ^
 *   | |  |  |  |  +--- UCPOL: clock polarity
 *   | |  |  |  +------ UCSZ1,0: character size
 *   | |  |  +--------- USBS: stop bit select
 *   | |  +------------ UPM1,0: parity mode
 *   | +--------------- UMSEL: mode select
 *   +----------------- URSEL: register select
 * 
 *   UBRR=UBRRL+UBRRH: baud rate registers
 *   UDR: data register
 */

/*
 * Example:
 * 
 * #include <avr/io.h>
 * #include "usart.h"
 * 
 * int main(void) {
 *   unsigned char i = 0;
 * 
 *   PORTB = 0;
 *   DDRB = ~0;
 * 
 *   usart_set(USART_RX | USART_TX | USART_REG_SELECT | USART_DATA_8BIT);
 *   usart_baud(9600);
 *   usart_signal(USART_INT_RXC);
 *   sei();
 * 
 *   for (;;) {
 *     usart_empty_wait();
 *     usart_tx_data(i++);
 *   }
 * 
 *   return 0;
 * }
 * 
 * ISR_USART_RXC() {
 *   if (!usart_error())
 *     PORTB = usart_rx_data();
 * }
 */


#ifndef _USART_H_
#define _USART_H_ 1


#include "util.h"

/* UCSRB=RXCIE|TXCIE|UDRIE|RXEN|TXEN|UCSZ2|RXB8|TXB8 */
/* UCSRA=RXC|TXC|UDRE|FE|DOR|PE|U2X|MPCM */
/* UCSRC=URSEL|UMSEL|UPM1|UPM0|USBS|UCSZ1|UCSZ0|UCPOL */


/* USART options (usart_set) */
#define USART_BAUD_DOUBLE  (b1(U2X)<<8)     /* double the USART transmission speed (Asynchronous) */
#define USART_MULTI_PROC   (b1(MPCM)<<8)    /* multi-processor communication mode */
#define USART_RX           b1(RXEN)         /* receiver enable */
#define USART_TX           b1(TXEN)         /* transmitter enable */
#define USART_SYNC         (b1(UMSEL)<<16)  /* USART synchronous mode select */
#define USART_CK_POLARITY  (b1(UCPOL)<<16)  /* clock polarity (synchronous) */
#define USART_REG_SELECT   (b1(URSEL)<<16)  /* register select for use UCSRC */

/* USART parity modes (usart_set) */
#define USART_PARITY_NONE  ((b0(UPM1)|b0(UPM0))<<16)  /* none parity */
#define USART_PARITY_EVEN  ((b1(UPM1)|b0(UPM0))<<16)  /* even parity */
#define USART_PARITY_ODD   ((b1(UPM1)|b1(UPM0))<<16)  /* odd parity */

/* USART stop bit size modes (usart_set) */
#define USART_STOP_1BIT  (b0(USBS)<<16)  /* 1 stop bit */
#define USART_STOP_2BIT  (b1(USBS)<<16)  /* 2 stop bit */

/* USART data bit size modes (usart_set) */
#define USART_DATA_5BIT  (b0(UCSZ2)|((b0(UCSZ1)|b0(UCSZ0))<<16))  /* 5bit character size */
#define USART_DATA_6BIT  (b0(UCSZ2)|((b0(UCSZ1)|b1(UCSZ0))<<16))  /* 6bit character size */
#define USART_DATA_7BIT  (b0(UCSZ2)|((b1(UCSZ1)|b0(UCSZ0))<<16))  /* 7bit character size */
#define USART_DATA_8BIT  (b0(UCSZ2)|((b1(UCSZ1)|b1(UCSZ0))<<16))  /* 8bit character size */
#define USART_DATA_9BIT  (b1(UCSZ2)|((b1(UCSZ1)|b1(UCSZ0))<<16))  /* 9bit character size */

/* USART standard baud rates (usart_baud) */
/* 110,300,600,1200,2400,4800,9600,14400,19200,28800,38400,
 * 56000,57600,76800,115200,230400,250000,500000,1000000 */
#define USART_BAUD_9600    9600
#define USART_BAUD_19200   19200
#define USART_BAUD_38400   38400
#define USART_BAUD_57600   57600
#define USART_BAUD_115200  115200

/* USART interrupts (usart_signal) */
#define USART_INT_RXC    b1(RXCIE)  /* USART receive complete interrupt */
#define USART_INT_EMPTY  b1(UDRIE)  /* USART data register empty interrupt */
#define USART_INT_TXC    b1(TXCIE)  /* USART transmitt complete interrupt */


/* USART macro routines */
#define usart_set(cnt)        {out(UCSRB, cnt); out(UCSRA, (cnt)>>8); out(UCSRC, (cnt)>>16);}  /* set USART */
#define usart_baud(bdr)       out(UBRR, (F_CPU+(bdr)*8)/((bdr)*16)-1)  /* for Asynchronous and synchronous (master) */
// #define usart_setbaud()       out(UBRR, UBRR_VALUE)  /* <util/setbaud.h> for Asynchronous and synchronous (master) */
#define usart_signal(sgn)     smi(UCSRB, sgn)                /* USART enable signals */
#define usart_tx_data(dta)    {out(UDR, dta); bis(dta, 0x100)? sbi(UCSRB, TXB8): cbi(UCSRB, TXB8);}  /* send data */
#define usart_rx_data()       (in(UDR)|(bis(UCSRB, RXB8)? 0x100: 0))  /* read received data */
#define usart_frame_error()   bis(UCSRA, FE)                 /* check USART receive framing error */
#define usart_data_overrun()  bis(UCSRA, DOR)                /* check USART receive data over run */
#define usart_parity_error()  bis(UCSRA, PE)                 /* check USART receive parity error */
#define usart_error()         mis(UCSRA, b1(FE)|b1(DOR)|b1(PE))  /* check USART receive framing error and data over run and parity error */
#define usart_empty_wait()    wait_set_bit(UCSRA, UDRE)      /* wait to USART data register empty */
#define usart_rx_wait()       wait_set_bit(UCSRA, RXC)       /* wait to USART receive complete */
#define usart_tx_wait()       wait_set_bit(UCSRA, TXC)       /* wait to USART transmitt complete */
#define usart_di()            cmi(UCSRB, b1(TXEN)|b1(RXEN))  /* USART disable */


/* USART receive complete interrupt service routine */
#define ISR_USART_RXC()    ISR(USART_RXC_vect)
/* USART data register empty interrupt service routine */
#define ISR_USART_EMPTY()  ISR(USART_UDRE_vect)
/* USART transmit complete interrupt service routine */
#define ISR_USART_TXC()    ISR(USART_TXC_vect)


#endif /* _USART_H_ */

