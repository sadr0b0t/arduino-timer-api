#include"timer-api.h"

void setup() {
    Serial.begin(9600);

    // частота 5Гц == 5 операций в секунду <=>
    // период таймера = 200мс
    timer_init_ISR_5Hz(TIMER_DEFAULT);

    // частота 50Гц == 50 операций в секунду <=>
    // период таймера = 20мс
    //timer_init_ISR_50Hz(TIMER_DEFAULT);
    
    pinMode(13, OUTPUT);
}

void loop() {
    Serial.println("Hello from loop!");
    delay(5000);
}

void timer_handle_interrupts(int timer) {
    static long prev_time = micros();

    long _time = micros();
    long _period = _time - prev_time;
    prev_time = _time;
    
    static int count = 5;

    // печатаем сообщение на каждый 5й вызов прерывания
    // (на частоте 5Гц - 1 раз в секунду)
    if(count == 0) {
        Serial.print("goodbye from timer: ");
        Serial.println(_period, DEC);
        
        digitalWrite(13, !digitalRead(13));
        
        count = 5;
    }
    count--;
}

