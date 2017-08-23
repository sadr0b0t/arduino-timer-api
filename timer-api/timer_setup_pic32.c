// based on
// https://github.com/chipKIT32/chipKIT-core/blob/master/pic32/libraries/Servo/utility/int.c

// also see
// PIC32 family reference manual Section 14. Timers
// http://umassamherstm5.org/tech-tutorials/pic32-tutorials/ubw32-tutorials/3-interrupts

// from ChipKIT-core int.c:
/************************************************************************/
/*																		*/
/*	int.c	--	Handles timer interrupts for PIC32      				*/
/*																		*/
/************************************************************************/
/*	Author:		Michelle Yu                                             */
/*                          											*/
/*	Copyright 2011, Digilent Inc.										*/
/*  This library is free software; you can redistribute it and/or       */
/*  modify it under the terms of the GNU Lesser General Public          */
/*  License as published by the Free Software Foundation; either        */
/*  version 2.1 of the License, or (at your option) any later version.  */
/*                                                                      */
/*  This library is distributed in the hope that it will be useful,     */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of      */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU   */
/*  Lesser General Public License for more details.                     */
/*                                                                      */
/*  You should have received a copy of the GNU Lesser General Public    */
/*  License along with this library; if not, write to the Free Software */
/*  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA           */
/*  02110-1301 USA                                                      */
/************************************************************************/
/*  File Description:													*/
/*	This file declares functions that handle timer interrupts for       */
/*  chipKIT PIC32 boards.												*/
//************************************************************************
//*	Edit History
//************************************************************************
//*	Aug 18,	2011	<MichelleY> file header comment block reformatted
//*	Sep  5,	2011	<GeneA> Added include of p32xxxx.h and plib.h to fix
//*						compile errors intoduced when include of plib.h was removed
//*						HardwareSerial.h
//* Feb  7, 2013	<GeneApperson> Removed dependency on Microchip plib library
//************************************************************************

//************************************************************************
// arduino-timer-api edits and additions:
// Anton Moiseev, 2017
//************************************************************************

#ifdef __PIC32__

#define OPT_SYSTEM_INTERNAL
#define OPT_BOARD_INTERNAL
#include <p32xxxx.h>
#include <sys/attribs.h>
#include <pins_arduino.h>

#include "timer_setup.h"

// Define timer ids
const int TIMER1 = -1;
const int TIMER3 = 0;
const int TIMER4 = 1;
const int TIMER5 = 2;

const int TIMER_DEFAULT = 1; // TIMER4;

// Define timer prescaler values: 3 bits in TxCON<6:4> register 
// give 8 different values for prescaler:
const int TIMER_PRESCALER_1_1    = 0b000; // 0000 0000
const int TIMER_PRESCALER_1_2    = 0b001; // 0001 0000
const int TIMER_PRESCALER_1_4    = 0b010; // 0010 0000
const int TIMER_PRESCALER_1_8    = 0b011; // 0011 0000
const int TIMER_PRESCALER_1_16   = 0b100; // 0100 0000
const int TIMER_PRESCALER_1_32   = 0b101; // 0101 0000
const int TIMER_PRESCALER_1_64   = 0b110; // 0110 0000
const int TIMER_PRESCALER_1_256  = 0b111; // 0111 0000

// set missing prescalers to 1 (no prescaling)
const int TIMER_PRESCALER_1_1024 = 0x0000; // 0000 0000


// M00BUG: This is hard coded for specific registers for interrupt priority
// flag bits and enable bits. This code happens to work correctly for all
// currently existing PIC32 devices, but this needs to be rewritten to be
// more generic.

//void __USER_ISR T3_IntHandler (void) {
void __attribute__((interrupt(),nomips16)) T3_IntHandler (void){
    timer_handle_interrupts(TIMER3);
    IFS0bits.T3IF = 0; // Clear timer interrupt status flag
}

//void __USER_ISR T4_IntHandler (void) {
void __attribute__((interrupt(),nomips16)) T4_IntHandler (void){
    timer_handle_interrupts(TIMER4);
    IFS0bits.T4IF = 0; // Clear timer interrupt status flag
}

//void __USER_ISR T5_IntHandler (void) {
void __attribute__((interrupt(),nomips16)) T5_IntHandler (void){
    timer_handle_interrupts(TIMER5);
    IFS0bits.T5IF = 0; // Clear timer interrupt status flag
}

// Typical freqs

/**
 * freq: 100KHz = 100000 ops/sec
 * period: 1sec/100000 = 10us
 */
void timer_init_ISR_100KHz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 10us (100000 operations per second == 100KHz) on 80MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=100:
    // 80000000/8/100000 = 10000000/100000 = 100
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 100-1);
#endif
}

/**
 * freq: 50KHz = 50000 ops/sec
 * period: 1sec/50000 = 20us
 */
void timer_init_ISR_50KHz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 20us (50000 operations per second == 50KHz) on 80MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=200:
    // 80000000/8/50000 = 10000000/50000 = 200
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 200-1);
#endif
}

/**
 * freq: 20KHz = 20000 ops/sec
 * period: 1sec/20000 = 50us
 */
void timer_init_ISR_20KHz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 50us (20000 operations per second == 20KHz) on 80MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=500:
    // 80000000/8/20000 = 10000000/20000 = 500
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 500-1);
#endif
}

/**
 * freq: 10KHz = 10000 ops/sec
 * period: 1sec/10000 = 100us
 */
void timer_init_ISR_10KHz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 100us (10000 operations per second == 10KHz) on 80MHz CPU
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=125:
    // 80000000/64/10000 = 1250000/10000 = 125
    timer_init_ISR(timer, TIMER_PRESCALER_1_64, 125-1);
#endif
}

/**
 * freq: 5KHz = 5000 ops/sec
 * period: 1sec/5000 = 200us
 */
void timer_init_ISR_5KHz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 200us (5000 operations per second == 5KHz) on 80MHz CPU
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=250:
    // 80000000/64/5000 = 1250000/5000 = 250
    timer_init_ISR(timer, TIMER_PRESCALER_1_64, 250-1);
#endif
}

/**
 * freq: 2KHz = 2000 ops/sec
 * period: 1sec/2000 = 500us
 */
void timer_init_ISR_2KHz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 500us (2000 operations per second == 2KHz) on 80MHz CPU
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=625:
    // 80000000/64/2000 = 1250000/2000 = 625
    timer_init_ISR(timer, TIMER_PRESCALER_1_64, 625-1);
#endif
}

/**
 * freq: 1KHz = 1000 ops/sec
 * period: 1sec/1000 = 1ms
 */
void timer_init_ISR_1KHz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 1ms (1000 operations per second == 1KHz) on 80MHz CPU
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=1250:
    // 80000000/64/1000 = 1250000/1000 = 1250
    timer_init_ISR(timer, TIMER_PRESCALER_1_64, 1250-1);
#endif
}

/**
 * freq: 100Hz = 100 ops/sec
 * period: 1sec/100 = 10ms
 */
void timer_init_ISR_100Hz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 10ms (100 operations per second == 100Hz) on 80MHz CPU
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=12500:
    // 80000000/64/100 = 1250000/100 = 12500
    timer_init_ISR(timer, TIMER_PRESCALER_1_64, 12500-1);
#endif
}

/**
 * freq: 50Hz = 50 ops/sec
 * period: 1sec/50 = 20ms
 */
void timer_init_ISR_50Hz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO

    // 40000000 / 32 / 25000 = 50 => 20ms
    //uint8_t     tckps   = 0b101;    // set prescalar 1:32
    //uint16_t    prx     = 0x61A8;   // 25000
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO

    // 200000000 / PB3(usually == 2) / 64 / 31250 = 50 => 20ms
    //uint8_t     tckps   = 0b110;    // set prescalar 1:64
    //uint16_t    prx     = F_CPU / (PB3DIV + 1) / 64 / 50;
#else
    // 80MHz
    // to set timer clock period to 20ms (50 operations per second == 50Hz) on 80MHz CPU
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=25000:
    // 80000000/64/50 = 1250000/50 = 25000
    timer_init_ISR(timer, TIMER_PRESCALER_1_64, 25000-1);
#endif
}

/**
 * freq: 20Hz = 20 ops/sec
 * period: 1sec/20 = 50ms
 */
void timer_init_ISR_20Hz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 50ms (20 operations per second == 20Hz) on 80MHz CPU
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=15625:
    // 80000000/256/20 = 312500/20 = 15625
    timer_init_ISR(timer, TIMER_PRESCALER_1_256, 15625-1);
#endif
}

/**
 * freq: 10Hz = 10 ops/sec
 * period: 1sec/10 = 100ms
 */
void timer_init_ISR_10Hz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 100ms (10 operations per second == 10Hz) on 80MHz CPU
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=31250:
    // 80000000/256/10 = 312500/10 = 31250
    timer_init_ISR(timer, TIMER_PRESCALER_1_256, 31250-1);
#endif
}

/**
 * freq: 5Hz = 5 ops/sec
 * period: 1sec/5 = 200ms
 */
void timer_init_ISR_5Hz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 200ms (5 operations per second == 5Hz) on 80MHz CPU
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=156250:
    // 80000000/256/5 = 312500/5 = 62500
    timer_init_ISR(timer, TIMER_PRESCALER_1_256, 62500-1);
#endif
}

/**
 * freq: 2Hz = 2 ops/sec
 * period: 1sec/2 = 500ms
 */
void timer_init_ISR_2Hz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 500ms (2 operations per second == 2Hz) on 80MHz CPU
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=156250:
    // 80000000/256/2 = 312500/2 = 156250 > 2^16 (won't work with 16-bit timer)
    //timer_init_ISR(timer, TIMER_PRESCALER_1_256, 156250-1);
    // TODO: use 32-bit timer
#endif
}

/**
 * freq: 1Hz = 1 ops/sec
 * period: 1sec
 */
void timer_init_ISR_1Hz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO

    // 40000000 / 32 / 25000 = 50 => 20ms
    //uint8_t     tckps   = 0b101;    // set prescalar 1:32
    //uint16_t    prx     = 0x61A8;   // 25000
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO

    // 200000000 / PB3(usually == 2) / 64 / 31250 = 50 => 20ms
    //uint8_t     tckps   = 0b110;    // set prescalar 1:64
    //uint16_t    prx     = F_CPU / (PB3DIV + 1) / 64 / 50;
#else
    // 80MHz
    // to set timer clock period to 1s (1 operation per second == 1Hz) on 80MHz CPU
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=312500:
    // 80000000/256/1 = 312500/1 = 312500 > 2^16 (won't work with 16-bit timer)
    //timer_init_ISR(timer, TIMER_PRESCALER_1_256, 312500-1);
    // TODO: use 32-bit timer
#endif
}

// core funcs


/**
 * Init ISR (Interrupt service routine) for the timer and start timer.
 * 
 * Example: to set timer clock period to 20ms (50 operations per second == 50Hz) on 80MHz CPU
 * use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=25000:
 * 80000000/64/50=25000
 * 
 * Timer interrupt handler timer_handle_interrupts would be called every 20ms (50 times per second)
 * in this case.
 * 
 * @param timer
 *   system timer id: use TIMER_DEFAULT for default timer
 *   or TIMER3, TIMER4 or TIMER5 for specific timer
 * @param prescaler
 *   timer prescaler (1, 2, 4, 8, 16, 32, 64, 256),
 *   use constants: PRESCALER_1, PRESCALER_2, PRESCALER_8,
 *   PRESCALER_16, PRESCALER_32, PRESCALER_64, PRESCALER_256
 * @param adjustment
 *   adjustment divider after timer prescaled - timer compare match value.
 */
void timer_init_ISR(int timer, int prescaler, int adjustment) {
    // See PIC32 family reference manual Section 14. Timers

    if(timer == TIMER3) {
        // set the vector up
        setIntVector(_TIMER_3_VECTOR, T3_IntHandler);

        // set timer clock period
        T3CONbits.TCKPS = prescaler; // set prescaler
        TMR3 = 0;                    // clear timer register
        PR3 = adjustment;            // period register
           
        IFS0bits.T3IF = 0; // clear interrupt flag
        IEC0bits.T3IE = 1; // enable interrupt
     
        IPC3CLR = 0x0000001F;
        IPC3SET = (_T3_IPL_IPC << 2) | _T3_SPL_IPC;
       
        T3CONbits.ON = 1; // start Timer3
    }
    if(timer == TIMER4) {
        // set the vector up
        setIntVector(_TIMER_4_VECTOR, T4_IntHandler);
        
        // set timer clock period
        T4CONbits.TCKPS = prescaler; // set prescaler
        TMR4 = 0;                    // clear timer register
        PR4 = adjustment;            // period register
           
        IFS0bits.T4IF = 0; // clear interrupt flag
        IEC0bits.T4IE = 1; // enable interrupt
         
        IPC4CLR = 0x0000001F;
        IPC4SET = (_T4_IPL_IPC << 2) | _T4_SPL_IPC;   
        
        T4CONbits.ON = 1; // start Timer4
    }
    if(timer == TIMER5) {
        // set the vector up
        setIntVector(_TIMER_5_VECTOR, T5_IntHandler);

        // set timer clock period
        T5CONbits.TCKPS = prescaler; // set prescaler
        TMR5 = 0;                    // clear timer register
        PR5 = adjustment;            // period register
           
        IFS0bits.T5IF = 0; // clear interrupt flag
        IEC0bits.T5IE = 1; // enable interrupt
     
        IPC5CLR = 0x0000001F;
        IPC5SET = (_T5_IPL_IPC << 2) | _T5_SPL_IPC;
       
        T5CONbits.ON = 1; // start Timer5
    }
}

/**
 * Stop ISR (Interrupt service routine) for the timer.
 * 
 * @param timer
 *   system timer id for started ISR
 */
void timer_stop_ISR(int timer) {
    //disable use of the given timer
    if (timer == TIMER3) {
        IEC0CLR = 0x1000; // disable T3 interrupt 
    } else if (timer == TIMER4) {
        IEC0CLR = 0x10000; // disable T4 interrupt 
    } else if (timer == TIMER5) {
        IEC0CLR = 0x100000; // disable T5 interrupt 
    }
}

#endif // __PIC32__

