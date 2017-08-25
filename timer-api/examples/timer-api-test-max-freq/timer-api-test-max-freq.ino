#include"timer-api.h"

// Experiments with high inerrupt freqs on different chips
// Эксперименты с высокими частотами прерываний на разных чипах

// original freq
#define FREQ_DIVIDER 1

// half freq (x2 period)
//#define FREQ_DIVIDER 2

// 1/10 freq (x10 period)
//#define FREQ_DIVIDER 10

// 1/1000 freq (x1000 period)
//#define FREQ_DIVIDER 1000

void setup() {
    Serial.begin(9600);

    // PIC32MX 80MHz (ChipKIT Uno32)
    // AVR 19MHz (Arduino Leonardo)

    // period = 1us
    // PIC32MX (ChipKIT Uno32): 3us, x2=5us, x10=21us, x1000=2005us (fail)
    // AVR (Arduino Leonardo): 12/16us, x10=80/84/92us (fail)
    //timer_init_ISR_1MHz(TIMER_DEFAULT);

    // period = 2us
    // PIC32MX (ChipKIT Uno32): 4us, x2=6us, x10=22us, x1000=2006us (+2us, ~fail)
    // AVR (Arduino Leonardo): 12/16us, x10=84us (fail)
    //timer_init_ISR_500KHz(TIMER_DEFAULT);

    // period = 5us
    // PIC32MX (ChipKIT Uno32): 5us, x10=50us (ok)
    // AVR (Arduino Leonardo): 12/16us, x2=20us, x10=84us (fail)
    //timer_init_ISR_200KHz(TIMER_DEFAULT);

    // period = 10us
    // PIC32MX (ChipKIT Uno32): 10us (ok)
    // AVR (Arduino Leonardo): 12us, x2=20us, x10=100/104/108us, x1000=10000  (~ok)
    timer_init_ISR_100KHz(TIMER_DEFAULT);
    
    // period = 20us
    // PIC32MX (ChipKIT Uno32): 20us (ok)
    // AVR (Arduino Leonardo): 12/20/24us, x2=40/44/48us, x1000=19996/20000/20004 (~ok)
    //timer_init_ISR_50KHz(TIMER_DEFAULT);

    // period = 50us
    // PIC32MX (ChipKIT Uno32): 50us (ok)
    // AVR (Arduino Leonardo): 48/52/56us, x2=100us (ok)
    //timer_init_ISR_20KHz(TIMER_DEFAULT);
    
    // period = 100us
    // PIC32MX (ChipKIT Uno32): 100us (ok)
    // AVR (Arduino Leonardo): 96/100/104us, x2=200/204us (ok)
    //timer_init_ISR_10KHz(TIMER_DEFAULT);
}

unsigned long _period;

void loop() {
    Serial.print("recent period:");
    Serial.print(_period, DEC);
    Serial.println("us");
    delay(1000);
}

void timer_handle_interrupts(int timer) {
    static unsigned long prev_time = 0;
    static long count = FREQ_DIVIDER - 1;
    
    if(count == 0) {
        unsigned long _time = micros();
        _period = _time - prev_time;
        prev_time = _time;
      
        count = FREQ_DIVIDER - 1;
    } else {
        count--;
    }
}

