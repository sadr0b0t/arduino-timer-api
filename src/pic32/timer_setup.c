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


// core funcs


/**
 * Init ISR (Interrupt service routine) for the timer and start timer.
 * 
 * Example: to set timer clock period to 20ms (50 operations per second == 50Hz) on 80MHz CPU
 * use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=25000-1:
 * 80000000/64/50=25000, minus 1 cause count from zero.
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
 *   use constants: PRESCALER_1, PRESCALER_2, PRESCALER_4, PRESCALER_8,
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

