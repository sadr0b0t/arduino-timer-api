#include"timer-api.h"

void setup() {
    Serial.begin(9600);

    // choose timers available on your hardware platform
    // выбирайте таймеры, доступные на текущей аппаратной платформе
    
    timer_init_ISR_1Hz(_TIMER2_32BIT);
    // don't start timers at the same time, so interrupts won't overlap
    // не стартовать таймеры в одно время, чтобы прерывания не накладывались
    delay(250);
    timer_init_ISR_2Hz(_TIMER4_32BIT);
}

void loop() {
    Serial.println("Hello from loop");
    delay(5000);
}

void timer_handle_interrupts(int timer) {
    if(timer == _TIMER2_32BIT) {
        static unsigned long prev_time = 0;
        
        unsigned long _time = micros();
        unsigned long _period = _time - prev_time;
        prev_time = _time;
        
        Serial.print("goodbye from timer1: ");
        Serial.println(_period, DEC);
    } else if(timer == _TIMER4_32BIT) {
        static unsigned long prev_time = 0;
        
        unsigned long _time = micros();
        unsigned long _period = _time - prev_time;
        prev_time = _time;
        
        Serial.print("goodbye from timer2: ");
        Serial.println(_period, DEC);
    }
}

