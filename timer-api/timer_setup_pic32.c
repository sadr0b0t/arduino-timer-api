// based on
// https://github.com/chipKIT32/chipKIT-core/blob/master/pic32/libraries/Servo/utility/int.h
// https://github.com/chipKIT32/chipKIT-core/blob/master/pic32/libraries/Servo/utility/int.c

// see also
// - PIC32 family reference manual Section 14. Timers
// - http://umassamherstm5.org/tech-tutorials/pic32-tutorials/ubw32-tutorials/3-interrupts
// - Добавить варианты 32-битных таймеров на PIC32/ChipKIT
// https://github.com/sadr0b0t/arduino-timer-api/issues/1

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
const int _TIMER1 = 0;
const int _TIMER2 = 1;
const int _TIMER3 = 2;
const int _TIMER4 = 3;
const int _TIMER5 = 4;

// 32-bit timers
const int _TIMER2_32BIT = 5;
const int _TIMER4_32BIT = 6;

const int TIMER_DEFAULT = 6; // TIMER4_32BIT;

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


//void __USER_ISR T1_IntHandler (void) {
void __attribute__((interrupt(),nomips16)) T1_IntHandler (void) {
    timer_handle_interrupts(_TIMER1);
    IFS0bits.T1IF = 0; // Clear timer interrupt status flag
}


//void __USER_ISR T2_IntHandler (void) {
void __attribute__((interrupt(),nomips16)) T2_IntHandler (void) {
    timer_handle_interrupts(_TIMER2);
    IFS0bits.T2IF = 0; // Clear timer interrupt status flag
}

//void __USER_ISR T3_IntHandler (void) {
void __attribute__((interrupt(),nomips16)) T3_IntHandler (void) {
    timer_handle_interrupts(_TIMER3);
    IFS0bits.T3IF = 0; // Clear timer interrupt status flag
}

//void __USER_ISR T4_IntHandler (void) {
void __attribute__((interrupt(),nomips16)) T4_IntHandler (void) {
    timer_handle_interrupts(_TIMER4);
    IFS0bits.T4IF = 0; // Clear timer interrupt status flag
}

//void __USER_ISR T5_IntHandler (void) {
void __attribute__((interrupt(),nomips16)) T5_IntHandler (void) {
    timer_handle_interrupts(_TIMER5);
    IFS0bits.T5IF = 0; // Clear timer interrupt status flag
}


//void __USER_ISR T23_IntHandler (void) {
void __attribute__((interrupt(),nomips16)) T23_IntHandler (void) {
    timer_handle_interrupts(_TIMER2_32BIT);
    // Timer2(32bit) == Timer2(16bit)+Timer3(16bit),
    // Timer2 is a master timer, but interrupt flags come from Timer3
    IFS0bits.T3IF = 0; // Clear timer interrupt status flag
}

//void __USER_ISR T45_IntHandler (void) {
void __attribute__((interrupt(),nomips16)) T45_IntHandler (void) {
    timer_handle_interrupts(_TIMER4_32BIT);
    // Timer4(32bit) == Timer4(16bit)+Timer5(16bit),
    // Timer4 is a master timer, but interrupt flags come from Timer5
    IFS0bits.T5IF = 0; // Clear timer interrupt status flag
}

// Typical freqs

/**
 * freq: 1MHz = 1000000 ops/sec
 * period: 1sec/1000000 = 1us
 */
void timer_init_ISR_1MHz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 1us (1000000 operations per second == 1MHz) on 80MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=10:
    // 80000000/8/1000000 = 10000000/1000000 = 10
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 10-1);
#endif
}

/**
 * freq: 500KHz = 500000 ops/sec
 * period: 1sec/500000 = 2us
 */
void timer_init_ISR_500KHz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 2us (500000 operations per second == 500KHz) on 80MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=20:
    // 80000000/8/500000 = 10000000/500000 = 20
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 20-1);
#endif
}

/**
 * freq: 200KHz = 200000 ops/sec
 * period: 1sec/200000 = 5us
 */
void timer_init_ISR_200KHz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 5us (200000 operations per second == 200KHz) on 80MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=50:
    // 80000000/8/200000 = 10000000/200000 = 50
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 50-1);
#endif
}

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
 * freq: 500Hz = 500 ops/sec
 * period: 1sec/500 = 2ms
 */
void timer_init_ISR_500Hz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 2ms (500 operations per second == 500Hz) on 80MHz CPU
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=2500:
    // 80000000/64/500 = 1250000/500 = 2500
    timer_init_ISR(timer, TIMER_PRESCALER_1_64, 2500-1);
#endif
}

/**
 * freq: 200Hz = 200 ops/sec
 * period: 1sec/200 = 5ms
 */
void timer_init_ISR_200Hz(int timer) {
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
    // 40MHz
    // TODO
#elif defined(__PIC32MZXX__)
    // 200MHz
    // TODO
#else
    // 80MHz
    // to set timer clock period to 5ms (200 operations per second == 200Hz) on 80MHz CPU
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=6250:
    // 80000000/64/200 = 1250000/200 = 6250
    timer_init_ISR(timer, TIMER_PRESCALER_1_64, 6250-1);
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
    // 80000000/256/2 = 312500/2 = 156250 > 2^16 > 2^16 = 65536
    // (will only work with 32-bit timer, won't work with 16-bit timer)
    timer_init_ISR(timer, TIMER_PRESCALER_1_256, 156250-1);
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
    // 80000000/256/1 = 312500/1 = 312500 > 2^16 = 65536
    // (will only work with 32-bit timer, won't work with 16-bit timer)
    timer_init_ISR(timer, TIMER_PRESCALER_1_256, 312500-1);
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
 *   or _TIMER1, _TIMER2, _TIMER3, _TIMER4, TIMER5,
 *   _TIMER2_32BIT or _TIMER4_32BIT for specific timer.
 * @param prescaler
 *   timer prescaler (1, 2, 4, 8, 16, 32, 64, 256),
 *   use constants: PRESCALER_1, PRESCALER_2, PRESCALER_8,
 *   PRESCALER_16, PRESCALER_32, PRESCALER_64, PRESCALER_256
 * @param adjustment
 *   adjustment divider after timer prescaled - timer compare match value.
 */
void timer_init_ISR(int timer, int prescaler, unsigned int adjustment) {
    // See PIC32 family reference manual Section 14. Timers
    
    if(timer == _TIMER1) {
        // set the vector up
        setIntVector(_TIMER_1_VECTOR, T1_IntHandler);
        
        // set timer clock period
        T1CONbits.TCKPS = prescaler; // set prescaler
        TMR1 = 0;                    // clear timer register
        PR1 = adjustment;            // period register
        
        // configure interrupt
        IFS0bits.T1IF = 0; // clear interrupt flag
        IEC0bits.T1IE = 1; // enable interrupt
        
        IPC1CLR = 0x0000001F;
        IPC1SET = (_T1_IPL_IPC << 2) | _T1_SPL_IPC;
        
        // start timer
        T1CONbits.ON = 1;
    }
    
    if(timer == _TIMER2) {
        // set the vector up
        setIntVector(_TIMER_2_VECTOR, T2_IntHandler);
        
        // set timer clock period
        T2CONbits.TCKPS = prescaler; // set prescaler
        TMR2 = 0;                    // clear timer register
        PR2 = adjustment;            // period register
        
        // configure interrupt
        IFS0bits.T2IF = 0; // clear interrupt flag
        IEC0bits.T2IE = 1; // enable interrupt
        
        IPC2CLR = 0x0000001F;
        IPC2SET = (_T2_IPL_IPC << 2) | _T2_SPL_IPC;
        
        // start timer
        T2CONbits.ON = 1;
    }
    
    if(timer == _TIMER3) {
        // set the vector up
        setIntVector(_TIMER_3_VECTOR, T3_IntHandler);
        
        // set timer clock period
        T3CONbits.TCKPS = prescaler; // set prescaler
        TMR3 = 0;                    // clear timer register
        PR3 = adjustment;            // period register
        
        // configure interrupt
        IFS0bits.T3IF = 0; // clear interrupt flag
        IEC0bits.T3IE = 1; // enable interrupt
        
        IPC3CLR = 0x0000001F;
        IPC3SET = (_T3_IPL_IPC << 2) | _T3_SPL_IPC;
        
        // start timer
        T3CONbits.ON = 1;
    }
    if(timer == _TIMER4) {
        // set the vector up
        setIntVector(_TIMER_4_VECTOR, T4_IntHandler);
        
        // set timer clock period
        T4CONbits.TCKPS = prescaler; // set prescaler
        TMR4 = 0;                    // clear timer register
        PR4 = adjustment;            // period register
        
        // configure interrupt
        IFS0bits.T4IF = 0; // clear interrupt flag
        IEC0bits.T4IE = 1; // enable interrupt
        
        IPC4CLR = 0x0000001F;
        IPC4SET = (_T4_IPL_IPC << 2) | _T4_SPL_IPC;
        
        // start timer
        T4CONbits.ON = 1;
    }
    if(timer == _TIMER5) {
        // set the vector up
        setIntVector(_TIMER_5_VECTOR, T5_IntHandler);
        
        // set timer clock period
        T5CONbits.TCKPS = prescaler; // set prescaler
        TMR5 = 0;                    // clear timer register
        PR5 = adjustment;            // period register
        
        // configure interrupt
        IFS0bits.T5IF = 0; // clear interrupt flag
        IEC0bits.T5IE = 1; // enable interrupt
        
        IPC5CLR = 0x0000001F;
        IPC5SET = (_T5_IPL_IPC << 2) | _T5_SPL_IPC;
        
        // start timer
        T5CONbits.ON = 1;
    }
    
    if(timer == _TIMER2_32BIT) {
        // Timer2(32bit) == Timer2(16bit)+Timer3(16bit),
        // Timer2 is a master timer, but interrupt flags come from Timer3
    
        // set the vector up
        setIntVector(_TIMER_3_VECTOR, T23_IntHandler);
        
        // clear contents of the T2CON and T3CON registers
        T2CON = 0x0; // stop 16-bit Timer2 and clear control register
        T3CON = 0x0; // stop 16-bit Timer3 and clear control register
        
        // set timer clock period on Timer2
        T2CONbits.T32 = 1;           // set 32-bit mode
        T2CONbits.TCKPS = prescaler; // set prescaler
        TMR2 = 0;                    // clear timer2 and timer3 registers
        PR2 = adjustment;            // period register (32-bit value)
        
        // configure interrupt on Timer5
        IPC3CLR = 0x0000001F;
        IPC3SET = (_T3_IPL_IPC << 2) | _T3_SPL_IPC;
        //IPC3SET = 0x00000004; // Set priority level = 1
        //IPC3SET = 0x00000001; //Set sub-priority level = 1
                              // Can be done in a single operation by assigning
                              // IPC3SET = 0x00000005
        
        IFS0bits.T3IF = 0; // clear the Timer3 interrupt status flag
        IEC0bits.T3IE = 1; // enable Timer3 interrupts
        
        // start Timer2+3
        T2CONbits.ON = 1;  // start Timer2+3
    }
    
    if(timer == _TIMER4_32BIT) {
        // Timer4(32bit) == Timer4(16bit)+Timer5(16bit),
        // Timer4 is a master timer, but interrupt flags come from Timer5
    
        // set the vector up
        setIntVector(_TIMER_5_VECTOR, T45_IntHandler);
        
        // clear contents of the T4CON and T5CON registers
        T4CON = 0x0; // stop 16-bit Timer4 and clear control register
        T5CON = 0x0; // stop 16-bit Timer5 and clear control register
        
        // set timer clock period on Timer4
        T4CONbits.T32 = 1;           // set 32-bit mode
        T4CONbits.TCKPS = prescaler; // set prescaler
        TMR4 = 0;                    // clear timer4 and timer5 registers
        PR4 = adjustment;            // period register (32-bit value)
        
        // configure interrupt on Timer5
        IPC5CLR = 0x0000001F;
        IPC5SET = (_T5_IPL_IPC << 2) | _T5_SPL_IPC;
        //IPC5SET = 0x00000004; // Set priority level = 1
        //IPC5SET = 0x00000001; //Set sub-priority level = 1
                              // Can be done in a single operation by assigning
                              // IPC5SET = 0x00000005
        
        IFS0bits.T5IF = 0; // clear the Timer5 interrupt status flag
        IEC0bits.T5IE = 1; // enable Timer5 interrupts
        
        // start Timer4+5
        T4CONbits.ON = 1;  // start Timer4+5
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
    if (timer == _TIMER1) {
        IEC0CLR = 0x10; // disable T1 interrupt
    } else if (timer == _TIMER2) {
        IEC0CLR = 0x100; // disable T2 interrupt
    } else if (timer == _TIMER3 || timer == _TIMER2_32BIT) {
        IEC0CLR = 0x1000; // disable T3 interrupt
    } else if (timer == _TIMER4) {
        IEC0CLR = 0x10000; // disable T4 interrupt
    } else if (timer == _TIMER5 || timer == _TIMER4_32BIT) {
        IEC0CLR = 0x100000; // disable T5 interrupt
    }
}

#endif // __PIC32__

