
#ifdef ARDUINO_ARCH_AVR

#include "timer_setup.h"

// Typical freqs

/**
 * freq: 1MHz = 1000000 ops/sec
 * period: 1sec/1000000 = 1us
 */
void timer_init_ISR_1MHz(int timer) {
    // to set timer clock period to 1us (1000000 operations per second == 1MHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=2-1:
    // 16000000/8/500000 = 2000000/1000000 = 2-1,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 2-1);
}

/**
 * freq: 500KHz = 500000 ops/sec
 * period: 1sec/500000 = 2us
 */
void timer_init_ISR_500KHz(int timer) {
    // to set timer clock period to 2us (500000 operations per second == 500KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=4-1:
    // 16000000/8/500000 = 2000000/500000 = 4-1,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 4-1);
}

/**
 * freq: 200KHz = 200000 ops/sec
 * period: 1sec/200000 = 5us
 */
void timer_init_ISR_200KHz(int timer) {
    // to set timer clock period to 5us (200000 operations per second == 200KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=10-1:
    // 16000000/8/200000 = 2000000/200000 = 10-1,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 10-1);
}

/**
 * freq: 100KHz = 100000 ops/sec
 * period: 1sec/100000 = 10us
 */
void timer_init_ISR_100KHz(int timer) {
    // to set timer clock period to 10us (100000 operations per second == 100KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=20-1:
    // 16000000/8/100000 = 2000000/100000 = 20-1,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 20-1);
}

/**
 * freq: 50KHz = 50000 ops/sec
 * period: 1sec/50000 = 20us
 */
void timer_init_ISR_50KHz(int timer) {
    // to set timer clock period to 20us (50000 operations per second == 50KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=40-1:
    // 16000000/8/50000 = 2000000/50000 = 40,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 40-1);
}

/**
 * freq: 20KHz = 20000 ops/sec
 * period: 1sec/20000 = 50us
 */
void timer_init_ISR_20KHz(int timer) {
    // to set timer clock period to 50us (20000 operations per second == 20KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=100-1:
    // 16000000/8/20000 = 2000000/20000 = 100,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 100-1);
}

/**
 * freq: 10KHz = 10000 ops/sec
 * period: 1sec/10000 = 100us
 */
void timer_init_ISR_10KHz(int timer) {
    // to set timer clock period to 100us (10000 operations per second == 10KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=200-1:
    // 16000000/8/10000 = 2000000/10000 = 200,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 200-1);
}

/**
 * freq: 5KHz = 5000 ops/sec
 * period: 1sec/5000 = 200us
 */
void timer_init_ISR_5KHz(int timer) {
    // to set timer clock period to 200us (5000 operations per second == 5KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=400-1:
    // 16000000/8/5000 = 2000000/5000 = 400,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 400-1);
}

/**
 * freq: 2KHz = 2000 ops/sec
 * period: 1sec/2000 = 500us
 */
void timer_init_ISR_2KHz(int timer) {
    // to set timer clock period to 500us (2000 operations per second == 2KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=1000-1:
    // 16000000/8/2000 = 2000000/2000 = 1000,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 1000-1);
}

/**
 * freq: 1KHz = 1000 ops/sec
 * period: 1sec/1000 = 1ms
 */
void timer_init_ISR_1KHz(int timer) {
    // to set timer clock period to 1ms (1000 operations per second == 1KHz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=2000-1:
    // 16000000/8/1000 = 2000000/1000 = 2000,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 2000-1);
}

/**
 * freq: 500Hz = 500 ops/sec
 * period: 1sec/500 = 2ms
 */
void timer_init_ISR_500Hz(int timer) {
    // to set timer clock period to 2ms (500 operations per second == 500Hz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=4000-1:
    // 16000000/8/500 = 2000000/500 = 4000,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 4000-1);
}

/**
 * freq: 200Hz = 200 ops/sec
 * period: 1sec/200 = 5ms
 */
void timer_init_ISR_200Hz(int timer) {
    // to set timer clock period to 5ms (200 operations per second == 200Hz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=10000-1:
    // 16000000/8/200 = 2000000/200 = 10000,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 10000-1);
}

/**
 * freq: 100Hz = 100 ops/sec
 * period: 1sec/100 = 10ms
 */
void timer_init_ISR_100Hz(int timer) {
    // to set timer clock period to 10ms (100 operations per second == 100Hz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=20000-1:
    // 16000000/8/100 = 2000000/100 = 20000,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 20000-1);
}

/**
 * freq: 50Hz = 50 ops/sec
 * period: 1sec/50 = 20ms
 */
void timer_init_ISR_50Hz(int timer) {
    // to set timer clock period to 20ms (50 operations per second == 50Hz) on 16MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=40000-1:
    // 16000000/8/50 = 2000000/50 = 40000,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 40000-1);
}

/**
 * freq: 20Hz = 20 ops/sec
 * period: 1sec/20 = 50ms
 */
void timer_init_ISR_20Hz(int timer) {
    // to set timer clock period to 50ms (20 operations per second == 20Hz) on 16MHz CPU
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=3125-1:
    // 16000000/256/20 = 62500/20 = 3125,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_256, 3125-1);
}

/**
 * freq: 10Hz = 10 ops/sec
 * period: 1sec/10 = 100ms
 */
void timer_init_ISR_10Hz(int timer) {
    // to set timer clock period to 100ms (10 operations per second == 10Hz) on 16MHz CPU
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=6250-1:
    // 16000000/256/10 = 62500/10 = 6250,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_256, 6250-1);
}

/**
 * freq: 5Hz = 5 ops/sec
 * period: 1sec/5 = 200ms
 */
void timer_init_ISR_5Hz(int timer) {
    // to set timer clock period to 200ms (5 operations per second == 5Hz) on 16MHz CPU
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=12500-1:
    // 16000000/256/5 = 62500/5 = 12500,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_256, 12500-1);
}

/**
 * freq: 2Hz = 2 ops/sec
 * period: 1sec/2 = 500ms
 */
void timer_init_ISR_2Hz(int timer) {
    // to set timer clock period to 500ms (2 operations per second == 2Hz) on 16MHz CPU
    // use prescaler 1:256 (TIMER_PRESCALER_1_256) and adjustment=31250-1:
    // 16000000/256/2 = 62500/2 = 31250,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_256, 31250-1);
}

/**
 * freq: 1Hz = 1 ops/sec
 * period: 1sec
 */
void timer_init_ISR_1Hz(int timer) {
    // to set timer clock period to 1s (1 operation per second == 1Hz) on 16MHz CPU
    // use prescaler 1:1024 (TIMER_PRESCALER_1_1024) and adjustment=15625-1:
    // 16000000/1024/1 = 15625/1 = 15625,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_1024, 15625-1);
}


#endif // ARDUINO_ARCH_AVR

