// based on
// https://github.com/arduino/Arduino/blob/ide-1.5.x/libraries/Servo/src/avr/Servo.cpp
// https://github.com/arduino/Arduino/blob/ide-1.5.x/libraries/Servo/src/avr/ServoTimers.h

// see also
// http://www.robotshop.com/letsmakerobots/arduino-101-timers-and-interrupts
// http://playground.arduino.cc/Code/Timer1


// from Arduino Servo.h:
/*
  Servo.h - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
  Copyright (c) 2009 Michael Margolis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

//************************************************************************
// arduino-timer-api edits and additions:
// Anton Moiseev, 2017
//************************************************************************


#ifdef ARDUINO_ARCH_AVR

#include "timer_setup.h"

#include <avr/interrupt.h>

/*
 * Defines for 16 bit timers used with  Servo library
 *
 * If _useTimerX is defined then TimerX is a 16 bit timer on the current board
 * timer16_Sequence_t enumerates the sequence that the timers should be allocated
 * _Nbr_16timers indicates how many 16 bit timers are available.
 */
/**
 * AVR Only definitions
 * --------------------
 */

// Say which 16 bit timers can be used and in what order
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define _useTimer5
#define _useTimer1
#define _useTimer3
#define _useTimer4
typedef enum { _timer5, _timer1, _timer3, _timer4, _Nbr_16timers } timer16_Sequence_t;

const int _TIMER1 = _timer1;
const int _TIMER3 = _timer3;
const int _TIMER4 = _timer4;
const int _TIMER5 = _timer5;

const int TIMER_DEFAULT = _timer5; //TIMER5;

#elif defined(__AVR_ATmega32U4__)
#define _useTimer1
typedef enum { _timer1, _Nbr_16timers } timer16_Sequence_t;

const int _TIMER1 = _timer1;
const int _TIMER3 = -1;
const int _TIMER4 = -1;
const int _TIMER5 = -1;

const int TIMER_DEFAULT = _timer1; //TIMER1;

#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
#define _useTimer3
#define _useTimer1
typedef enum { _timer3, _timer1, _Nbr_16timers } timer16_Sequence_t;

const int _TIMER1 = _timer1;
const int _TIMER3 = _timer3;
const int _TIMER4 = -1;
const int _TIMER5 = -1;

const int TIMER_DEFAULT = _timer3; //TIMER3;

#elif defined(__AVR_ATmega128__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega2561__)
#define _useTimer3
#define _useTimer1
typedef enum { _timer3, _timer1, _Nbr_16timers } timer16_Sequence_t;

const int _TIMER1 = _timer1;
const int _TIMER3 = _timer3;
const int _TIMER4 = -1;
const int _TIMER5 = -1;

const int TIMER_DEFAULT = _timer3; //TIMER3;

#else  // everything else
#define _useTimer1
typedef enum { _timer1, _Nbr_16timers } timer16_Sequence_t;

const int _TIMER1 = _timer1;
const int _TIMER3 = -1;
const int _TIMER4 = -1;
const int _TIMER5 = -1;

const int TIMER_DEFAULT = _timer1; //TIMER1;

#endif

// no Timer2 at all
const int _TIMER2 = -1;

// no 32-bit timers at all
const int _TIMER2_32BIT = -1;
const int _TIMER4_32BIT = -1;

// Define timer prescaler options:
// CS12 CS11 CS10
const int TIMER_PRESCALER_1_1    = 1; // 001
const int TIMER_PRESCALER_1_8    = 2; // 010
const int TIMER_PRESCALER_1_64   = 3; // 011
const int TIMER_PRESCALER_1_256  = 4; // 100
const int TIMER_PRESCALER_1_1024 = 5; // 101

// set missing prescalers to 1 (no prescaling)
const int TIMER_PRESCALER_1_2    = 1; // 001
const int TIMER_PRESCALER_1_4    = 1; // 001
const int TIMER_PRESCALER_1_16   = 1; // 001
const int TIMER_PRESCALER_1_32   = 1; // 001


#ifndef WIRING // Wiring pre-defines signal handlers so don't define any if compiling for the Wiring platform
// Interrupt handlers for Arduino
#if defined(_useTimer1)
SIGNAL (TIMER1_COMPA_vect) {
    timer_handle_interrupts(_timer1);
}
#endif

#if defined(_useTimer3)
SIGNAL (TIMER3_COMPA_vect) {
    timer_handle_interrupts(_timer3);
}
#endif

#if defined(_useTimer4)
SIGNAL (TIMER4_COMPA_vect) {
    timer_handle_interrupts(_timer4);
}
#endif

#if defined(_useTimer5)
SIGNAL (TIMER5_COMPA_vect) {
    timer_handle_interrupts(_timer5);
}
#endif

#elif defined WIRING
// Interrupt handlers for Wiring
#if defined(_useTimer1)
void Timer1Service() {
    timer_handle_interrupts(_timer1);
}
#endif
#if defined(_useTimer3)
void Timer3Service() {
    timer_handle_interrupts(_timer3);
}
#endif
#endif

// Typical freqs

/**
 * freq: 1MHz = 1000000 ops/sec
 * period: 1sec/1000000 = 1us
 */
void timer_init_ISR_1MHz(int timer) {
    // to set timer clock period to 1us (1000000 operations per second == 1MHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=2:
    // 16000000/8/500000 = 2000000/1000000 = 2
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 2-1);
}

/**
 * freq: 500KHz = 500000 ops/sec
 * period: 1sec/500000 = 2us
 */
void timer_init_ISR_500KHz(int timer) {
    // to set timer clock period to 2us (500000 operations per second == 500KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=4:
    // 16000000/8/500000 = 2000000/500000 = 4
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 4-1);
}

/**
 * freq: 200KHz = 200000 ops/sec
 * period: 1sec/200000 = 5us
 */
void timer_init_ISR_200KHz(int timer) {
    // to set timer clock period to 5us (200000 operations per second == 200KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=10:
    // 16000000/8/200000 = 2000000/200000 = 10
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 10-1);
}

/**
 * freq: 100KHz = 100000 ops/sec
 * period: 1sec/100000 = 10us
 */
void timer_init_ISR_100KHz(int timer) {
    // to set timer clock period to 10us (100000 operations per second == 100KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=20:
    // 16000000/8/100000 = 2000000/100000 = 20
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 20-1);
}

/**
 * freq: 50KHz = 50000 ops/sec
 * period: 1sec/50000 = 20us
 */
void timer_init_ISR_50KHz(int timer) {
    // to set timer clock period to 20us (50000 operations per second == 50KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=40:
    // 16000000/8/50000 = 2000000/50000 = 40
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 40-1);
}

/**
 * freq: 20KHz = 20000 ops/sec
 * period: 1sec/20000 = 50us
 */
void timer_init_ISR_20KHz(int timer) {
    // to set timer clock period to 50us (20000 operations per second == 20KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=100:
    // 16000000/8/20000 = 2000000/20000 = 100
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 100-1);
}

/**
 * freq: 10KHz = 10000 ops/sec
 * period: 1sec/10000 = 100us
 */
void timer_init_ISR_10KHz(int timer) {
    // to set timer clock period to 100us (10000 operations per second == 10KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=200:
    // 16000000/8/10000 = 2000000/10000 = 200
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 200-1);
}

/**
 * freq: 5KHz = 5000 ops/sec
 * period: 1sec/5000 = 200us
 */
void timer_init_ISR_5KHz(int timer) {
    // to set timer clock period to 200us (5000 operations per second == 5KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=400:
    // 16000000/8/5000 = 2000000/5000 = 400
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 400-1);
}

/**
 * freq: 2KHz = 2000 ops/sec
 * period: 1sec/2000 = 500us
 */
void timer_init_ISR_2KHz(int timer) {
    // to set timer clock period to 500us (2000 operations per second == 2KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=1000:
    // 16000000/8/2000 = 2000000/2000 = 1000
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 1000-1);
}

/**
 * freq: 1KHz = 1000 ops/sec
 * period: 1sec/1000 = 1ms
 */
void timer_init_ISR_1KHz(int timer) {
    // to set timer clock period to 1ms (1000 operations per second == 1KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=2000:
    // 16000000/8/1000 = 2000000/1000 = 2000
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 2000-1);
}

/**
 * freq: 500Hz = 500 ops/sec
 * period: 1sec/500 = 2ms
 */
void timer_init_ISR_500Hz(int timer) {
    // to set timer clock period to 2ms (500 operations per second == 500Hz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=4000:
    // 16000000/8/500 = 2000000/500 = 4000
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 4000-1);
}

/**
 * freq: 200Hz = 200 ops/sec
 * period: 1sec/200 = 5ms
 */
void timer_init_ISR_200Hz(int timer) {
    // to set timer clock period to 5ms (200 operations per second == 200Hz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=10000:
    // 16000000/8/200 = 2000000/200 = 10000
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 10000-1);
}

/**
 * freq: 100Hz = 100 ops/sec
 * period: 1sec/100 = 10ms
 */
void timer_init_ISR_100Hz(int timer) {
    // to set timer clock period to 10ms (100 operations per second == 100Hz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=20000:
    // 16000000/8/100 = 2000000/100 = 20000
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 20000-1);
}

/**
 * freq: 50Hz = 50 ops/sec
 * period: 1sec/50 = 20ms
 */
void timer_init_ISR_50Hz(int timer) {
    // to set timer clock period to 20ms (50 operations per second == 50Hz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=40000:
    // 16000000/8/50 = 2000000/50 = 40000
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 40000-1);
}

/**
 * freq: 20Hz = 20 ops/sec
 * period: 1sec/20 = 50ms
 */
void timer_init_ISR_20Hz(int timer) {
    // to set timer clock period to 50ms (20 operations per second == 20Hz) on 16MHz CPU
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=3125:
    // 16000000/256/20 = 62500/20 = 3125
    timer_init_ISR(timer, TIMER_PRESCALER_1_256, 3125-1);
}

/**
 * freq: 10Hz = 10 ops/sec
 * period: 1sec/10 = 100ms
 */
void timer_init_ISR_10Hz(int timer) {
    // to set timer clock period to 100ms (10 operations per second == 10Hz) on 16MHz CPU
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=6250:
    // 16000000/256/10 = 62500/10 = 6250
    timer_init_ISR(timer, TIMER_PRESCALER_1_256, 6250-1);
}

/**
 * freq: 5Hz = 5 ops/sec
 * period: 1sec/5 = 200ms
 */
void timer_init_ISR_5Hz(int timer) {
    // to set timer clock period to 200ms (5 operations per second == 5Hz) on 16MHz CPU
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=12500:
    // 16000000/256/5 = 62500/5 = 12500
    timer_init_ISR(timer, TIMER_PRESCALER_1_256, 12500-1);
}

/**
 * freq: 2Hz = 2 ops/sec
 * period: 1sec/2 = 500ms
 */
void timer_init_ISR_2Hz(int timer) {
    // to set timer clock period to 500ms (2 operations per second == 2Hz) on 16MHz CPU
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=31250:
    // 16000000/256/2 = 62500/2 = 31250
    timer_init_ISR(timer, TIMER_PRESCALER_1_256, 31250-1);
}

/**
 * freq: 1Hz = 1 ops/sec
 * period: 1sec
 */
void timer_init_ISR_1Hz(int timer) {
    // to set timer clock period to 1s (1 operation per second == 1Hz) on 16MHz CPU
    // use prescaler 1:1024 (TIMER_PRESCALER_1_1024) and adjustment=15625:
    // 16000000/1024/1 = 15625/1 = 15625
    timer_init_ISR(timer, TIMER_PRESCALER_1_1024, 15625-1);
}

// Core funcs

/**
 * Init ISR (Interrupt service routine) for the timer and start timer.
 * 
 * http://www.robotshop.com/letsmakerobots/arduino-101-timers-and-interrupts
 * 1. CPU frequency 16Mhz for Arduino
 * 2. maximum timer counter value (256 for 8bit, 65536 for 16bit timer)
 * 3. Divide CPU frequency through the choosen prescaler (16000000 / 256 = 62500)
 * 4. Divide result through the desired frequency (62500 / 2Hz = 31250)
 * 5. Verify the result against the maximum timer counter value (31250 < 65536 success).
 *    If fail, choose bigger prescaler.
 * 
 * Example: to set timer clock period to 20ms (50 operations per second == 50Hz) on 16MHz CPU
 * use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=40000:
 * 16000000/8/50=40000
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
 *   adjustment divider after timer prescaled - timer compare match value
 */
void timer_init_ISR(int timer, int prescaler, unsigned int adjustment) {
#if defined (_useTimer1)
    if(timer == _timer1) {
        // CS12 CS11 CS10
        // 1/1:    001
        // 1/8:    010
        // 1/64:   011
        // 1/256:  100
        // 1/1024: 101
        unsigned char prescalerBits = 0;
        if(prescaler == TIMER_PRESCALER_1_1) {
            prescalerBits = _BV(CS10);
        } else if(prescaler == TIMER_PRESCALER_1_8) {
            prescalerBits = _BV(CS11);
        } else if(prescaler == TIMER_PRESCALER_1_64) {
            prescalerBits = _BV(CS11) | _BV(CS10);
        } else if(prescaler == TIMER_PRESCALER_1_256) {
            prescalerBits = _BV(CS12);
        } else if(prescaler == TIMER_PRESCALER_1_1024) {
            prescalerBits = _BV(CS12) | _BV(CS10);
        } else {
            // let it be no prescaler by default:
            prescalerBits = _BV(CS10);
        }
        
        TCCR1A = 0;             // normal counting mode
        TCCR1B = 0;             // timer mode:
        TCCR1B |= _BV(WGM12);   // CTC mode
        TCCR1B |= prescalerBits;// set prescaler
        OCR1A = adjustment;     // compare match register
        TCNT1 = 0;              // clear the timer count
#if defined(__AVR_ATmega8__)|| defined(__AVR_ATmega128__)
        TIFR |= _BV(OCF1A);      // clear any pending interrupts;
        TIMSK |=  _BV(OCIE1A) ;  // enable the output compare interrupt
#else
        // here if not ATmega8 or ATmega128
        TIFR1 |= _BV(OCF1A);     // clear any pending interrupts;
        TIMSK1 |=  _BV(OCIE1A) ; // enable the output compare interrupt
#endif
#if defined(WIRING)
        timerAttach(TIMER1OUTCOMPAREA_INT, Timer1Service);
#endif
    }
#endif

#if defined (_useTimer3)
    if(timer == _timer3) {
        // CS32 CS31 CS30
        // 1/1:    001
        // 1/8:    010
        // 1/64:   011
        // 1/256:  100
        // 1/1024: 101
        unsigned char prescalerBits = 0;
        if(prescaler == TIMER_PRESCALER_1_1) {
            prescalerBits = _BV(CS30);
        } else if(prescaler == TIMER_PRESCALER_1_8) {
            prescalerBits = _BV(CS31);
        } else if(prescaler == TIMER_PRESCALER_1_64) {
            prescalerBits = _BV(CS31) | _BV(CS30);
        } else if(prescaler == TIMER_PRESCALER_1_256) {
            prescalerBits = _BV(CS32);
        } else if(prescaler == TIMER_PRESCALER_1_1024) {
            prescalerBits = _BV(CS32) | _BV(CS30);
        } else {
            // let it be no prescaler by default:
            prescalerBits = _BV(CS30);
        }
        
        TCCR3A = 0;             // normal counting mode
        TCCR3B = 0;             // timer mode:
        TCCR3B |= _BV(WGM32);   // CTC mode
        TCCR3B |= prescalerBits;// set prescaler
        OCR3A = adjustment;     // compare match register
        TCNT3 = 0;              // clear the timer count
#if defined(__AVR_ATmega128__)
        TIFR |= _BV(OCF3A);     // clear any pending interrupts;
        ETIMSK |= _BV(OCIE3A);  // enable the output compare interrupt
#else
        TIFR3 = _BV(OCF3A);     // clear any pending interrupts;
        TIMSK3 =  _BV(OCIE3A) ; // enable the output compare interrupt
#endif
#if defined(WIRING)
        timerAttach(TIMER3OUTCOMPAREA_INT, Timer3Service);  // for Wiring platform only
#endif
    }
#endif

#if defined (_useTimer4)
    if(timer == _timer4) {
        // CS42 CS41 CS40
        // 1/1:    001
        // 1/8:    010
        // 1/64:   011
        // 1/256:  100
        // 1/1024: 101
        unsigned char prescalerBits = 0;
        if(prescaler == TIMER_PRESCALER_1_1) {
            prescalerBits = _BV(CS40);
        } else if(prescaler == TIMER_PRESCALER_1_8) {
            prescalerBits = _BV(CS41);
        } else if(prescaler == TIMER_PRESCALER_1_64) {
            prescalerBits = _BV(CS41) | _BV(CS40);
        } else if(prescaler == TIMER_PRESCALER_1_256) {
            prescalerBits = _BV(CS42);
        } else if(prescaler == TIMER_PRESCALER_1_1024) {
            prescalerBits = _BV(CS42) | _BV(CS40);
        } else {
            // let it be no prescaler by default:
            prescalerBits = _BV(CS40);
        }
        
        TCCR4A = 0;             // normal counting mode
        TCCR4B = 0;             // timer mode:
        TCCR4B |= _BV(WGM42);   // CTC mode
        TCCR4B |= prescalerBits;// set prescaler
        OCR4A = adjustment;     // compare match register
        TCNT4 = 0;              // clear the timer count
        TIFR4 = _BV(OCF4A);     // clear any pending interrupts;
        TIMSK4 =  _BV(OCIE4A) ; // enable the output compare interrupt
    }
#endif

#if defined (_useTimer5)
    if(timer == _timer5) {
        // CS52 CS51 CS50
        // 1/1:    001
        // 1/8:    010
        // 1/64:   011
        // 1/256:  100
        // 1/1024: 101
        unsigned char prescalerBits = 0;
        if(prescaler == TIMER_PRESCALER_1_1) {
            prescalerBits = _BV(CS50);
        } else if(prescaler == TIMER_PRESCALER_1_8) {
            prescalerBits = _BV(CS51);
        } else if(prescaler == TIMER_PRESCALER_1_64) {
            prescalerBits = _BV(CS51) | _BV(CS50);
        } else if(prescaler == TIMER_PRESCALER_1_256) {
            prescalerBits = _BV(CS52);
        } else if(prescaler == TIMER_PRESCALER_1_1024) {
            prescalerBits = _BV(CS52) | _BV(CS50);
        } else {
            // let it be no prescaler by default:
            prescalerBits = _BV(CS50);
        }
        
        TCCR5A = 0;             // normal counting mode
        TCCR5B = 0;             // timer mode:
        TCCR5B |= _BV(WGM52);   // CTC mode
        TCCR5B |= prescalerBits;// set prescaler
        OCR5A = adjustment;     // compare match register
        TCNT5 = 0;              // clear the timer count
        TIFR5 = _BV(OCF5A);     // clear any pending interrupts;
        TIMSK5 =  _BV(OCIE5A) ; // enable the output compare interrupt
    }
#endif
}

/**
 * Stop ISR (Interrupt service routine) for the timer.
 * 
 * @param timer
 *   system timer id for started ISR
 */
void timer_stop_ISR(int timer) {
    //disable use of the given timer
#if defined WIRING   // Wiring
    if(timer == _timer1) {
        #if defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
        TIMSK1 &= ~_BV(OCIE1A) ;  // disable timer 1 output compare interrupt
        #else
        TIMSK &= ~_BV(OCIE1A) ;  // disable timer 1 output compare interrupt
        #endif
        timerDetach(TIMER1OUTCOMPAREA_INT);
    } else if(timer == _timer3) {
        #if defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
        TIMSK3 &= ~_BV(OCIE3A);    // disable the timer3 output compare A interrupt
        #else
        ETIMSK &= ~_BV(OCIE3A);    // disable the timer3 output compare A interrupt
        #endif
        timerDetach(TIMER3OUTCOMPAREA_INT);
    }
#else
    //For arduino - in future: call here to a currently undefined function to reset the timer
#endif
}

#endif // ARDUINO_ARCH_AVR

