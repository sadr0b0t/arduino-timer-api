#include"timer-api.h"

int _timer = TIMER_DEFAULT;
//int _timer = _TIMER1;
//int _timer = _TIMER2;
//int _timer = _TIMER3;
//int _timer = _TIMER4;
//int _timer = _TIMER5;
//int _timer = _TIMER6;
//int _timer = _TIMER7;
//int _timer = _TIMER8;
//int _timer = _TIMER9;
//int _timer = _TIMER1_32BIT;
//int _timer = _TIMER2_32BIT;
//int _timer = _TIMER3_32BIT;
//int _timer = _TIMER4_32BIT;
//int _timer = _TIMER5_32BIT;
//int _timer = _TIMER6_32BIT;
//int _timer = _TIMER7_32BIT;
//int _timer = _TIMER8_32BIT;
//int _timer = _TIMER9_32BIT;

void setup() {
    Serial.begin(9600);
    while(!Serial);

    pinMode(13, OUTPUT);
}

void loop() {
    Serial.println("Start timer");
    timer_init_ISR_1Hz(_timer);
    delay(5000);
    
    Serial.println("Stop timer");
    timer_stop_ISR(_timer);
    delay(5000);
}

/**
 * Timer interrupt service routine, called with chosen period
 * @param timer - timer id
 */
/**
 * Процедура, вызываемая прерыванием по событию таймера с заданным периодом
 * @param timer - идентификатор таймера
 */
void timer_handle_interrupts(int timer) {
    static unsigned long prev_time = 0;
    
    unsigned long _time = micros();
    unsigned long _period = _time - prev_time;
    prev_time = _time;
    
    Serial.print("goodbye from timer: ");
    Serial.println(_period, DEC);

    // blink led
    // мигаем лампочкой
    digitalWrite(13, !digitalRead(13));
}

