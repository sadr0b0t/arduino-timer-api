#include"timer-api.h"

void setup() {
    Serial.begin(9600);

    // freq=1Hz, period=1s
    // частота=1Гц, период=1с
    timer_init_ISR_1Hz(TIMER_DEFAULT);

    // freq=2Hz, period=500ms
    // частота=2Гц, период=500мс
    //timer_init_ISR_2Hz(TIMER_DEFAULT);
    
    // freq=5Hz, period=200ms
    // частота=5Гц, период=200мс
    //timer_init_ISR_5Hz(TIMER_DEFAULT);

    //timer_init_ISR_500KHz(TIMER_DEFAULT);
    //timer_init_ISR_200KHz(TIMER_DEFAULT);
    //timer_init_ISR_100KHz(TIMER_DEFAULT);
    //timer_init_ISR_50KHz(TIMER_DEFAULT);
    //timer_init_ISR_20KHz(TIMER_DEFAULT);
    //timer_init_ISR_10KHz(TIMER_DEFAULT);
    //timer_init_ISR_5KHz(TIMER_DEFAULT);
    //timer_init_ISR_2KHz(TIMER_DEFAULT);
    //timer_init_ISR_1KHz(TIMER_DEFAULT);
    //timer_init_ISR_500Hz(TIMER_DEFAULT);
    //timer_init_ISR_200Hz(TIMER_DEFAULT);
    //timer_init_ISR_100Hz(TIMER_DEFAULT);
    //timer_init_ISR_50Hz(TIMER_DEFAULT);
    //timer_init_ISR_20Hz(TIMER_DEFAULT);
    //timer_init_ISR_10Hz(TIMER_DEFAULT);
    //timer_init_ISR_5Hz(TIMER_DEFAULT);
    //timer_init_ISR_2Hz(TIMER_DEFAULT);
    //timer_init_ISR_1Hz(TIMER_DEFAULT);

    pinMode(13, OUTPUT);
}

void loop() {
    Serial.println("Hello from loop!");
    delay(5000);

    // any code here: blocking or non-blocking
    // здесь любой код: блокирующий или неблокирующий
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

