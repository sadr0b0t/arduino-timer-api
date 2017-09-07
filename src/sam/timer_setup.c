// based on
// https://github.com/arduino/Arduino/blob/ide-1.5.x/libraries/Servo/src/sam/Servo.cpp
// https://github.com/arduino/Arduino/blob/ide-1.5.x/libraries/Servo/src/sam/ServoTimers.h

// see also
// http://www.atmel.com/Images/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf
// https://store.arduino.cc/usa/arduino-due
// https://www.arduino.cc/en/Guide/ArduinoDue


// from Arduino sam/ServoTimer.h:
/*
  Copyright (c) 2013 Arduino LLC. All right reserved.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

//************************************************************************
// arduino-timer-api edits and additions:
// Anton Moiseev, 2017
//************************************************************************


#ifdef ARDUINO_ARCH_SAM

#include "timer_setup.h"
#include <Arduino.h>

/**
 * SAM Only definitions
 * --------------------
 */

// For SAM3X:
#define _useTimer1
#define _useTimer2
#define _useTimer3
#define _useTimer4
#define _useTimer5
#define _useTimer6
#define _useTimer7
#define _useTimer8
#define _useTimer9

/*
  TC0, chan 0 => TC0_Handler
  TC0, chan 1 => TC1_Handler
  TC0, chan 2 => TC2_Handler
  TC1, chan 0 => TC3_Handler
  TC1, chan 1 => TC4_Handler
  TC1, chan 2 => TC5_Handler
  TC2, chan 0 => TC6_Handler
  TC2, chan 1 => TC7_Handler
  TC2, chan 2 => TC8_Handler
 */

#if defined (_useTimer1)
#define TC_FOR_TIMER1       TC0
#define CHANNEL_FOR_TIMER1  0
#define ID_TC_FOR_TIMER1    ID_TC0
#define IRQn_FOR_TIMER1     TC0_IRQn
#define HANDLER_FOR_TIMER1  TC0_Handler
#endif
#if defined (_useTimer2)
#define TC_FOR_TIMER2       TC0
#define CHANNEL_FOR_TIMER2  1
#define ID_TC_FOR_TIMER2    ID_TC1
#define IRQn_FOR_TIMER2     TC1_IRQn
#define HANDLER_FOR_TIMER2  TC1_Handler
#endif
#if defined (_useTimer3)
#define TC_FOR_TIMER3       TC0
#define CHANNEL_FOR_TIMER3  2
#define ID_TC_FOR_TIMER3    ID_TC2
#define IRQn_FOR_TIMER3     TC2_IRQn
#define HANDLER_FOR_TIMER3  TC2_Handler
#endif
#if defined (_useTimer4)
#define TC_FOR_TIMER4       TC1
#define CHANNEL_FOR_TIMER4  0
#define ID_TC_FOR_TIMER4    ID_TC3
#define IRQn_FOR_TIMER4     TC3_IRQn
#define HANDLER_FOR_TIMER4  TC3_Handler
#endif
#if defined (_useTimer5)
#define TC_FOR_TIMER5       TC1
#define CHANNEL_FOR_TIMER5  1
#define ID_TC_FOR_TIMER5    ID_TC4
#define IRQn_FOR_TIMER5     TC4_IRQn
#define HANDLER_FOR_TIMER5  TC4_Handler
#endif
#if defined (_useTimer6)
#define TC_FOR_TIMER6       TC1
#define CHANNEL_FOR_TIMER6  2
#define ID_TC_FOR_TIMER6    ID_TC5
#define IRQn_FOR_TIMER6     TC5_IRQn
#define HANDLER_FOR_TIMER6  TC5_Handler
#endif
#if defined (_useTimer7)
#define TC_FOR_TIMER7       TC2
#define CHANNEL_FOR_TIMER7  0
#define ID_TC_FOR_TIMER7    ID_TC6
#define IRQn_FOR_TIMER7     TC6_IRQn
#define HANDLER_FOR_TIMER7  TC6_Handler
#endif
#if defined (_useTimer8)
#define TC_FOR_TIMER8       TC1
#define CHANNEL_FOR_TIMER8  1
#define ID_TC_FOR_TIMER8    ID_TC7
#define IRQn_FOR_TIMER8     TC7_IRQn
#define HANDLER_FOR_TIMER8  TC7_Handler
#endif
#if defined (_useTimer9)
#define TC_FOR_TIMER9       TC1
#define CHANNEL_FOR_TIMER9  2
#define ID_TC_FOR_TIMER9    ID_TC8
#define IRQn_FOR_TIMER9     TC8_IRQn
#define HANDLER_FOR_TIMER9  TC8_Handler
#endif



// define timer prescaler options:
const int TIMER_PRESCALER_1_2    = 1;
const int TIMER_PRESCALER_1_8    = 2;
const int TIMER_PRESCALER_1_32   = 3;
const int TIMER_PRESCALER_1_128  = 4;


typedef enum {
    _timer1, _timer2, _timer3,
    _timer4, _timer5, _timer6,
    _timer7, _timer8, _timer9,
    _Nbr_timers
} timer_Sequence_t ;

const int _TIMER1 = _timer1;
const int _TIMER2 = _timer2;
const int _TIMER3 = _timer3;
const int _TIMER4 = _timer4;
const int _TIMER5 = _timer5;
const int _TIMER6 = _timer6;
const int _TIMER7 = _timer7;
const int _TIMER8 = _timer8;
const int _TIMER9 = _timer9;

// all timers here are 32-bit
const int _TIMER1_32BIT = _timer1;
const int _TIMER2_32BIT = _timer2;
const int _TIMER3_32BIT = _timer3;
const int _TIMER4_32BIT = _timer4;
const int _TIMER5_32BIT = _timer5;
const int _TIMER6_32BIT = _timer6;
const int _TIMER7_32BIT = _timer7;
const int _TIMER8_32BIT = _timer8;
const int _TIMER9_32BIT = _timer9;

// default timer
const int TIMER_DEFAULT = _timer3; // let it be TIMER3

//------------------------------------------------------------------------------
/// Interrupt handler for timers
//------------------------------------------------------------------------------

static void _HANDLER_FOR_TIMER(int timer, Tc *tc, uint8_t channel) {
    // clear interrupt
    tc->TC_CHANNEL[channel].TC_SR; // TC - timer counter
    // reset the timer
    tc->TC_CHANNEL[channel].TC_CCR |= TC_CCR_SWTRG;

    timer_handle_interrupts(timer);
}

#if defined (_useTimer1)
void HANDLER_FOR_TIMER1(void) {
    _HANDLER_FOR_TIMER(_TIMER1, TC_FOR_TIMER1, CHANNEL_FOR_TIMER1);
}
#endif
#if defined (_useTimer2)
void HANDLER_FOR_TIMER2(void) {
    _HANDLER_FOR_TIMER(_TIMER2, TC_FOR_TIMER2, CHANNEL_FOR_TIMER2);
}
#endif
#if defined (_useTimer3)
void HANDLER_FOR_TIMER3(void) {
    _HANDLER_FOR_TIMER(_TIMER3, TC_FOR_TIMER3, CHANNEL_FOR_TIMER3);
}
#endif
#if defined (_useTimer4)
void HANDLER_FOR_TIMER4(void) {
    _HANDLER_FOR_TIMER(_TIMER4, TC_FOR_TIMER4, CHANNEL_FOR_TIMER4);
}
#endif
#if defined (_useTimer5)
void HANDLER_FOR_TIMER5(void) {
    _HANDLER_FOR_TIMER(_TIMER5, TC_FOR_TIMER5, CHANNEL_FOR_TIMER5);
}
#endif
#if defined (_useTimer6)
void HANDLER_FOR_TIMER6(void) {
    _HANDLER_FOR_TIMER(_TIMER6, TC_FOR_TIMER6, CHANNEL_FOR_TIMER6);
}
#endif
#if defined (_useTimer7)
void HANDLER_FOR_TIMER7(void) {
    _HANDLER_FOR_TIMER(_TIMER7, TC_FOR_TIMER7, CHANNEL_FOR_TIMER7);
}
#endif
#if defined (_useTimer8)
void HANDLER_FOR_TIMER8(void) {
    _HANDLER_FOR_TIMER(_TIMER8, TC_FOR_TIMER8, CHANNEL_FOR_TIMER8);
}
#endif
#if defined (_useTimer9)
void HANDLER_FOR_TIMER9(void) {
    _HANDLER_FOR_TIMER(_TIMER9, TC_FOR_TIMER9, CHANNEL_FOR_TIMER9);
}
#endif


// Core funcs


static void _initISR(Tc *tc, uint32_t channel, uint32_t id, uint32_t tclock, uint32_t adjustment, IRQn_Type irqn) {
    pmc_enable_periph_clk(id);
    TC_Configure(tc, channel,
        tclock |                     // timer clock (prescaler)
        TC_CMR_WAVE |                // Waveform mode
        TC_CMR_WAVSEL_UP_RC );       // Counter running up and reset when equals to RC

    // timer clock adjustment
    TC_SetRA(tc, channel, adjustment);

    /* Configure and enable interrupt */
    NVIC_EnableIRQ(irqn);
    // TC_IER_CPAS: RA Compare
    tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPAS;

    // Enables the timer clock and performs a software reset to start the counting
    TC_Start(tc, channel);
}

/**
 * Init ISR (Interrupt service routine) for the timer and start timer.
 * 
 * Example: to set timer clock period to 20ms (50 operations per second == 50Hz) on 84MHz CPU
 * use prescaler 1:128 (TIMER_PRESCALER_1_128) and adjustment=13125-1:
 * 84000000/128/50=13125, minus 1 cause count from zero.
 * 
 * Timer interrupt handler timer_handle_interrupts would be called every 20ms (50 times per second)
 * in this case.
 * 
 * @param timer
 *   system timer id: use TIMER_DEFAULT for default timer
 *   or TIMER1, TIMER2, TIMER3, TIMER4, TIMER5, TIMER6, TIMER7, TIMER8, TIMER9 for specific timer
 * @param prescaler
 *   timer prescaler (2, 8, 32, 128),
 *   use constants: PRESCALER_1_2, PRESCALER_1_8, PRESCALER_1_32, PRESCALER_1_128
 * @param adjustment
 *   adjustment divider after timer prescaled - timer compare match value
 */
void timer_init_ISR(int timer, int prescaler, unsigned int adjustment) {

    uint32_t tclock;
    if(prescaler == TIMER_PRESCALER_1_2) {
        tclock = TC_CMR_TCCLKS_TIMER_CLOCK1; // MCK/2
    } else if(prescaler == TIMER_PRESCALER_1_8) {
        tclock = TC_CMR_TCCLKS_TIMER_CLOCK2; // MCK/8
    } else if(prescaler == TIMER_PRESCALER_1_32) {
        tclock = TC_CMR_TCCLKS_TIMER_CLOCK3; // MCK/32
    } else if(prescaler == TIMER_PRESCALER_1_128) {
        tclock = TC_CMR_TCCLKS_TIMER_CLOCK4; // MCK/128
    } else {
        // set wrong prescaler as 1/2 (min prescaling),
        // the timer would work, but result would be wrong anyway
        tclock = TIMER_PRESCALER_1_2;
    }

#if defined (_useTimer1)
    if (timer == _TIMER1)
        _initISR(TC_FOR_TIMER1, CHANNEL_FOR_TIMER1, ID_TC_FOR_TIMER1, tclock, adjustment, IRQn_FOR_TIMER1);
#endif
#if defined (_useTimer2)
    if (timer == _TIMER2)
        _initISR(TC_FOR_TIMER2, CHANNEL_FOR_TIMER2, ID_TC_FOR_TIMER2, tclock, adjustment, IRQn_FOR_TIMER2);
#endif
#if defined (_useTimer3)
    if (timer == _TIMER3)
        _initISR(TC_FOR_TIMER3, CHANNEL_FOR_TIMER3, ID_TC_FOR_TIMER3, tclock, adjustment, IRQn_FOR_TIMER3);
#endif
#if defined (_useTimer4)
    if (timer == _TIMER4)
        _initISR(TC_FOR_TIMER4, CHANNEL_FOR_TIMER4, ID_TC_FOR_TIMER4, tclock, adjustment, IRQn_FOR_TIMER4);
#endif
#if defined (_useTimer5)
    if (timer == _TIMER5)
        _initISR(TC_FOR_TIMER5, CHANNEL_FOR_TIMER5, ID_TC_FOR_TIMER5, tclock, adjustment, IRQn_FOR_TIMER5);
#endif
#if defined (_useTimer6)
    if (timer == _TIMER6)
        _initISR(TC_FOR_TIMER6, CHANNEL_FOR_TIMER6, ID_TC_FOR_TIMER6, tclock, adjustment, IRQn_FOR_TIMER6);
#endif
#if defined (_useTimer7)
    if (timer == _TIMER7)
        _initISR(TC_FOR_TIMER7, CHANNEL_FOR_TIMER7, ID_TC_FOR_TIMER7, tclock, adjustment, IRQn_FOR_TIMER7);
#endif
#if defined (_useTimer8)
    if (timer == _TIMER8)
        _initISR(TC_FOR_TIMER8, CHANNEL_FOR_TIMER8, ID_TC_FOR_TIMER8, tclock, adjustment, IRQn_FOR_TIMER8);
#endif
#if defined (_useTimer9)
    if (timer == _TIMER9)
        _initISR(TC_FOR_TIMER9, CHANNEL_FOR_TIMER9, ID_TC_FOR_TIMER9, tclock, adjustment, IRQn_FOR_TIMER9);
#endif
}


/**
 * Stop ISR (Interrupt service routine) for the timer.
 * 
 * @param timer
 *   system timer id for started ISR
 */
void timer_stop_ISR(int timer) {
#if defined (_useTimer1)
    TC_Stop(TC_FOR_TIMER1, CHANNEL_FOR_TIMER1);
#endif
#if defined (_useTimer2)
    TC_Stop(TC_FOR_TIMER2, CHANNEL_FOR_TIMER2);
#endif
#if defined (_useTimer3)
    TC_Stop(TC_FOR_TIMER3, CHANNEL_FOR_TIMER3);
#endif
#if defined (_useTimer4)
    TC_Stop(TC_FOR_TIMER4, CHANNEL_FOR_TIMER4);
#endif
#if defined (_useTimer5)
    TC_Stop(TC_FOR_TIMER5, CHANNEL_FOR_TIMER5);
#endif
#if defined (_useTimer6)
    TC_Stop(TC_FOR_TIMER6, CHANNEL_FOR_TIMER6);
#endif
#if defined (_useTimer7)
    TC_Stop(TC_FOR_TIMER7, CHANNEL_FOR_TIMER7);
#endif
#if defined (_useTimer8)
    TC_Stop(TC_FOR_TIMER8, CHANNEL_FOR_TIMER8);
#endif
#if defined (_useTimer9)
    TC_Stop(TC_FOR_TIMER9, CHANNEL_FOR_TIMER9);
#endif
}

#endif // ARDUINO_ARCH_SAM

