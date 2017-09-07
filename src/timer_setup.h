#ifndef TIMER_SETUP_H
#define TIMER_SETUP_H

// Define timer ids
extern const int _TIMER1;
extern const int _TIMER2;
extern const int _TIMER3;
extern const int _TIMER4;
extern const int _TIMER5;
extern const int _TIMER6;
extern const int _TIMER7;
extern const int _TIMER8;
extern const int _TIMER9;

// 32-bit timers
extern const int _TIMER1_32BIT;
extern const int _TIMER2_32BIT;
extern const int _TIMER3_32BIT;
extern const int _TIMER4_32BIT;
extern const int _TIMER5_32BIT;
extern const int _TIMER6_32BIT;
extern const int _TIMER7_32BIT;
extern const int _TIMER8_32BIT;
extern const int _TIMER9_32BIT;

// Default timer
extern const int TIMER_DEFAULT;

// Define timer prescaler options
extern const int TIMER_PRESCALER_1_1;
extern const int TIMER_PRESCALER_1_2;
extern const int TIMER_PRESCALER_1_4;
extern const int TIMER_PRESCALER_1_8;
extern const int TIMER_PRESCALER_1_16;
extern const int TIMER_PRESCALER_1_32;
extern const int TIMER_PRESCALER_1_64;
extern const int TIMER_PRESCALER_1_128;
extern const int TIMER_PRESCALER_1_256;
extern const int TIMER_PRESCALER_1_1024;

// Typical freqs

/**
 * freq: 1MHz = 1000000 ops/sec
 * period: 1sec/1000000 = 1us
 */
void timer_init_ISR_1MHz(int timer);

/**
 * freq: 500KHz = 500000 ops/sec
 * period: 1sec/500000 = 2us
 */
void timer_init_ISR_500KHz(int timer);

/**
 * freq: 200KHz = 200000 ops/sec
 * period: 1sec/200000 = 5us
 */
void timer_init_ISR_200KHz(int timer);

/**
 * freq: 100KHz = 100000 ops/sec
 * period: 1sec/100000 = 10us
 */
void timer_init_ISR_100KHz(int timer);

/**
 * freq: 50KHz = 50000 ops/sec
 * period: 1sec/50000 = 20us
 */
void timer_init_ISR_50KHz(int timer);

/**
 * freq: 20KHz = 20000 ops/sec
 * period: 1sec/20000 = 50us
 */
void timer_init_ISR_20KHz(int timer);


/**
 * freq: 10KHz = 10000 ops/sec
 * period: 1sec/10000 = 100us
 */
void timer_init_ISR_10KHz(int timer);

/**
 * freq: 5KHz = 5000 ops/sec
 * period: 1sec/5000 = 200us
 */
void timer_init_ISR_5KHz(int timer);

/**
 * freq: 2KHz = 2000 ops/sec
 * period: 1sec/2000 = 500us
 */
void timer_init_ISR_2KHz(int timer);

/**
 * freq: 1KHz = 1000 ops/sec
 * period: 1sec/1000 = 1ms
 */
void timer_init_ISR_1KHz(int timer);

/**
 * freq: 500Hz = 500 ops/sec
 * period: 1sec/500 = 2ms
 */
void timer_init_ISR_500Hz(int timer);

/**
 * freq: 200Hz = 200 ops/sec
 * period: 1sec/200 = 5ms
 */
void timer_init_ISR_200Hz(int timer);

/**
 * freq: 100Hz = 100 ops/sec
 * period: 1sec/100 = 10ms
 */
void timer_init_ISR_100Hz(int timer);

/**
 * freq: 50Hz = 50 ops/sec
 * period: 1sec/50 = 20ms
 */
void timer_init_ISR_50Hz(int timer);

/**
 * freq: 20Hz = 20 ops/sec
 * period: 1sec/20 = 50ms
 */
void timer_init_ISR_20Hz(int timer);

/**
 * freq: 10Hz = 10 ops/sec
 * period: 1sec/10 = 100ms
 */
void timer_init_ISR_10Hz(int timer);

/**
 * freq: 5Hz = 5 ops/sec
 * period: 1sec/5 = 200ms
 */
void timer_init_ISR_5Hz(int timer);

/**
 * freq: 2Hz = 2 ops/sec
 * period: 1sec/2 = 500ms
 */
void timer_init_ISR_2Hz(int timer);

/**
 * freq: 1Hz = 1 ops/sec
 * period: 1sec
 */
void timer_init_ISR_1Hz(int timer);

// Core funcs

/**
 * Init ISR (Interrupt service routine) for the timer and start timer.
 * 
 * General algorithm
 * http://www.robotshop.com/letsmakerobots/arduino-101-timers-and-interrupts
 * 1. CPU frequency 16Mhz for Arduino
 * 2. maximum timer counter value (256 for 8bit, 65536 for 16bit timer)
 * 3. Divide CPU frequency through the choosen prescaler (16000000 / 256 = 62500)
 * 4. Divide result through the desired frequency (62500 / 2Hz = 31250)
 * 5. Verify the result against the maximum timer counter value (31250 < 65536 success).
 *    If fail, choose bigger prescaler.
 * 
 * Example: to set timer clock period to 20ms (50 operations per second == 50Hz)
 * 
 * 1) on 16MHz CPU (AVR Arduino)
 *   use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=40000-1:
 *   16000000/8/50=40000, minus 1 cause count from zero.
 * 
 * 2) on 80MHz CPU (PIC32MX ChipKIT)
 *   use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=25000-1:
 *   80000000/64/50=25000, minus 1 cause count from zero.
 *
 * 3) on 84MHz CPU (SAM Arduino Due)
 *   use prescaler 1:128 (TIMER_PRESCALER_1_128) and adjustment=13125-1:
 *   80000000/128/50=13125, minus 1 cause count from zero.
 * 
 * Timer interrupt handler timer_handle_interrupts would be called every 20ms
 * (50 times per second == 50Hz freq) in this case.
 * 
 * @param timer
 *   system timer id: use TIMER_DEFAULT for default timer
 *   or _TIMER1...TIMER9, _TIMER1_32BIT..._TIMER9_32BIT for specific timer.
 *   note: availability of specific timer depends on the platform.
 * @param prescaler
 *   timer prescaler (1, 2, 4, 8, 16, 32, 64, 128, 256, 1024),
 *   use constants: PRESCALER_1_1, PRESCALER_1_2, PRESCALER_1_8,
 *   PRESCALER_1_16, PRESCALER_1_32, PRESCALER_1_64, PRESCALER_1_128
 *   PRESCALER_1_256, PRESCALER_1_1024
 *   note: availability of specific prescaler depends on the platform.
 * @param adjustment
 *   adjustment divider after timer prescaled - timer compare match value.
 */
void timer_init_ISR(int timer, int prescaler, unsigned int adjustment);

/**
 * Stop ISR (Interrupt service routine) for the timer.
 * 
 * @param timer
 *     system timer id for started ISR
 */
void timer_stop_ISR(int timer);

/**
 * Timer ISR (Interrupt service routine) handler.
 * Implementation must be provided in module with user code.
 * 
 * @param timer
 *     system timer id for started ISR 
 */
void timer_handle_interrupts(int timer);

#endif


