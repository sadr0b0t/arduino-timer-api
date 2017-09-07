
#ifdef __PIC32__

#include "timer_setup.h"

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
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=10-1:
    // 80000000/8/1000000 = 10000000/1000000 = 10,
    // minus 1 cause count from zero.
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
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=20-1:
    // 80000000/8/500000 = 10000000/500000 = 20,
    // minus 1 cause count from zero.
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
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=50-1:
    // 80000000/8/200000 = 10000000/200000 = 50,
    // minus 1 cause count from zero.
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
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=100-1:
    // 80000000/8/100000 = 10000000/100000 = 100,
    // minus 1 cause count from zero.
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
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=200-1:
    // 80000000/8/50000 = 10000000/50000 = 200,
    // minus 1 cause count from zero.
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
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=500-1:
    // 80000000/8/20000 = 10000000/20000 = 500,
    // minus 1 cause count from zero.
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
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=125-1:
    // 80000000/64/10000 = 1250000/10000 = 125,
    // minus 1 cause count from zero.
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
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=250-1:
    // 80000000/64/5000 = 1250000/5000 = 250,
    // minus 1 cause count from zero.
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
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=625-1:
    // 80000000/64/2000 = 1250000/2000 = 625,
    // minus 1 cause count from zero.
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
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=1250-1:
    // 80000000/64/1000 = 1250000/1000 = 1250,
    // minus 1 cause count from zero.
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
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=2500-1:
    // 80000000/64/500 = 1250000/500 = 2500,
    // minus 1 cause count from zero.
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
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=6250-1:
    // 80000000/64/200 = 1250000/200 = 6250,
    // minus 1 cause count from zero.
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
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=12500-1:
    // 80000000/64/100 = 1250000/100 = 12500,
    // minus 1 cause count from zero.
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
    // use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=25000-1:
    // 80000000/64/50 = 1250000/50 = 25000,
    // minus 1 cause count from zero.
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
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=15625-1:
    // 80000000/256/20 = 312500/20 = 15625,
    // minus 1 cause count from zero.
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
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=31250-1:
    // 80000000/256/10 = 312500/10 = 31250,
    // minus 1 cause count from zero.
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
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=156250-1:
    // 80000000/256/5 = 312500/5 = 62500,
    // minus 1 cause count from zero.
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
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=156250-1:
    // 80000000/256/2 = 312500/2 = 156250 > 2^16 > 2^16 = 65536
    // (will only work with 32-bit timer, won't work with 16-bit timer),
    // minus 1 cause count from zero.
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
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=312500-1:
    // 80000000/256/1 = 312500/1 = 312500 > 2^16 = 65536
    // (will only work with 32-bit timer, won't work with 16-bit timer),
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_256, 312500-1);
#endif
}

#endif // __PIC32__

