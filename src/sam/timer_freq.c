
#ifdef ARDUINO_ARCH_SAM

#include "timer_setup.h"

// Typical freqs

/**
 * freq: 1MHz = 1000000 ops/sec
 * period: 1sec/1000000 = 1us
 */
void timer_init_ISR_1MHz(int timer) {
    // to set timer clock period to 1us (1000000 operations per second == 1MHz) on 84MHz CPU
    // use prescaler 1:2 (TIMER_PRESCALER_1_2) and adjustment=42-1:
    // 84000000/2/1000000 = 42000000/1000000 = 42,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_2, 42-1);
}

/**
 * freq: 500KHz = 500000 ops/sec
 * period: 1sec/500000 = 2us
 */
void timer_init_ISR_500KHz(int timer) {
    // to set timer clock period to 2us (500000 operations per second == 500KHz) on 84MHz CPU
    // use prescaler 1:2 (TIMER_PRESCALER_1_2) and adjustment=84-1:
    // 84000000/2/500000 = 42000000/500000 = 84,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_2, 84-1);
}

/**
 * freq: 200KHz = 200000 ops/sec
 * period: 1sec/200000 = 5us
 */
void timer_init_ISR_200KHz(int timer) {
    // to set timer clock period to 5us (200000 operations per second == 200KHz) on 84MHz CPU
    // use prescaler 1:2 (TIMER_PRESCALER_1_2) and adjustment=210-1:
    // 84000000/2/200000 = 42000000/200000 = 210,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_2, 210-1);
}

/**
 * freq: 100KHz = 100000 ops/sec
 * period: 1sec/100000 = 10us
 */
void timer_init_ISR_100KHz(int timer) {
    // to set timer clock period to 10us (100000 operations per second == 100KHz) on 84MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=105-1:
    // 84000000/8/100000 = 10500000/100000 = 105,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 105-1);
}

/**
 * freq: 50KHz = 50000 ops/sec
 * period: 1sec/50000 = 20us
 */
void timer_init_ISR_50KHz(int timer) {
    // to set timer clock period to 20us (50000 operations per second == 50KHz) on 84MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=210-1:
    // 84000000/8/50000 = 10500000/50000 = 210,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 210-1);
}

/**
 * freq: 20KHz = 20000 ops/sec
 * period: 1sec/20000 = 50us
 */
void timer_init_ISR_20KHz(int timer) {
    // to set timer clock period to 50us (20000 operations per second == 20KHz) on 84MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=525-1:
    // 84000000/8/20000 = 10500000/20000 = 525,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 525-1);
}

/**
 * freq: 10KHz = 10000 ops/sec
 * period: 1sec/10000 = 100us
 */
void timer_init_ISR_10KHz(int timer) {
    // to set timer clock period to 100us (10000 operations per second == 10KHz) on 84MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=1050-1:
    // 84000000/8/10000 = 10500000/10000 = 1050,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 1050-1);
}

/**
 * freq: 5KHz = 5000 ops/sec
 * period: 1sec/5000 = 200us
 */
void timer_init_ISR_5KHz(int timer) {
    // to set timer clock period to 200us (5000 operations per second == 5KHz) on 84MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=2100-1:
    // 84000000/8/5000 = 10500000/5000 = 2100,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 2100-1);
}

/**
 * freq: 2KHz = 2000 ops/sec
 * period: 1sec/2000 = 500us
 */
void timer_init_ISR_2KHz(int timer) {
    // to set timer clock period to 500us (2000 operations per second == 2KHz) on 84MHz CPU
    // use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=5250-1:
    // 84000000/8/2000 = 10500000/2000 = 5250,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_8, 5250-1);
}

/**
 * freq: 1KHz = 1000 ops/sec
 * period: 1sec/1000 = 1ms
 */
void timer_init_ISR_1KHz(int timer) {
    // to set timer clock period to 1ms (1000 operations per second == 1KHz) on 84MHz CPU
    // use prescaler 1:32 (TIMER_PRESCALER_1_32) and adjustment=2625-1:
    // 84000000/32/1000 = 2625000/1000 = 2625,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_32, 2625-1);
}

/**
 * freq: 500Hz = 500 ops/sec
 * period: 1sec/500 = 2ms
 */
void timer_init_ISR_500Hz(int timer) {
    // to set timer clock period to 2ms (500 operations per second == 500Hz) on 84MHz CPU
    // use prescaler 1:32 (TIMER_PRESCALER_1_32) and adjustment=5250-1:
    // 84000000/32/500 = 2625000/500 = 5250,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_32, 5250-1);
}

/**
 * freq: 200Hz = 200 ops/sec
 * period: 1sec/200 = 5ms
 */
void timer_init_ISR_200Hz(int timer) {
    // to set timer clock period to 5ms (200 operations per second == 200Hz) on 84MHz CPU
    // use prescaler 1:32 (TIMER_PRESCALER_1_32) and adjustment=13125-1:
    // 84000000/32/200 = 2625000/200 = 13125,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_32, 13125-1);
}

/**
 * freq: 100Hz = 100 ops/sec
 * period: 1sec/100 = 10ms
 */
void timer_init_ISR_100Hz(int timer) {
    // to set timer clock period to 10ms (100 operations per second == 100Hz) on 84MHz CPU
    // use prescaler 1:32 (TIMER_PRESCALER_1_32) and adjustment=26250-1:
    // 84000000/32/100 = 2625000/100 = 26250,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_32, 26250-1);
}

/**
 * freq: 50Hz = 50 ops/sec
 * period: 1sec/50 = 20ms
 */
void timer_init_ISR_50Hz(int timer) {
    // to set timer clock period to 20ms (50 operations per second == 50Hz) on 84MHz CPU
    // use prescaler 1:32 (TIMER_PRESCALER_1_32) and adjustment=52500-1:
    // 84000000/32/50 = 2625000/50 = 52500,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_32, 52500-1);
}

/**
 * freq: 20Hz = 20 ops/sec
 * period: 1sec/20 = 50ms
 */
void timer_init_ISR_20Hz(int timer) {
    // to set timer clock period to 50ms (20 operations per second == 20Hz) on 84MHz CPU
    // use prescaler 1:32 (TIMER_PRESCALER_1_32) and adjustment=131250-1:
    // 84000000/32/20 = 2625000/20 = 131250,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_32, 131250-1);
}

/**
 * freq: 10Hz = 10 ops/sec
 * period: 1sec/10 = 100ms
 */
void timer_init_ISR_10Hz(int timer) {
    // to set timer clock period to 100ms (10 operations per second == 10Hz) on 84MHz CPU
    // use prescaler 1:128 (TIMER_PRESCALER_1_128) and adjustment=65625-1:
    // 84000000/128/10 = 656250/10 = 65625,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_128, 65625-1);
}

/**
 * freq: 5Hz = 5 ops/sec
 * period: 1sec/5 = 200ms
 */
void timer_init_ISR_5Hz(int timer) {
    // to set timer clock period to 200ms (5 operations per second == 5Hz) on 84MHz CPU
    // use prescaler 1:128 (TIMER_PRESCALER_1_128) and adjustment=131250-1:
    // 84000000/128/5 = 656250/5 = 131250,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_128, 131250-1);
}

/**
 * freq: 2Hz = 2 ops/sec
 * period: 1sec/2 = 500ms
 */
void timer_init_ISR_2Hz(int timer) {
    // to set timer clock period to 500ms (2 operations per second == 2Hz) on 84MHz CPU
    // use prescaler 1:128 (TIMER_PRESCALER_1_128) and adjustment=328125-1:
    // 84000000/128/2 = 656250/2 = 328125,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_128, 328125-1);
}


/**
 * freq: 1Hz = 1 ops/sec
 * period: 1sec
 */
void timer_init_ISR_1Hz(int timer) {
    // to set timer clock period to 1s (1 operation per second == 1Hz) on 84MHz CPU
    // use prescaler 1:128 (TIMER_PRESCALER_1_128) and adjustment=656250-1:
    // 84000000/128/1 = 656250/1 = 656250,
    // minus 1 cause count from zero.
    timer_init_ISR(timer, TIMER_PRESCALER_1_128, 656250-1);
}


#endif // ARDUINO_ARCH_SAM

