#include"timer-api.h"

void setup() {
    Serial.begin(9600);
    while(!Serial);

    // freq=10Hz, period=100ms
    // частота=10Гц, период=100мс
    timer_init_ISR_10Hz(TIMER_DEFAULT);
    
    pinMode(13, OUTPUT);
}

void loop() {
    Serial.println("Hello from loop!");
    delay(6000);

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

    // additional period multiplicator
    // дополнильный множитель периода
    static int count = 11;

    // Print message every 12'th interrupt call:
    // if base freq is 10Hz and base period is 100ms,
    // then message would be printed each 100ms*12=1200ms
    // (5 times per 6 seconds)
    // Печатаем сообщение на каждый 12й вызов прерывания:
    // если базовая частота 10Гц и базовый период 100мс,
    // то сообщение будет печататься каждые 100мс*12=1200мс
    // (5 раз за 6 секунд)
    if(count == 0) {
        unsigned long _time = micros();
        unsigned long _period = _time - prev_time;
        prev_time = _time;
    
        Serial.print("goodbye from timer: ");
        Serial.println(_period, DEC);

        // blink led
        // мигаем лампочкой
        digitalWrite(13, !digitalRead(13));

        // load counter
        // взводим счетчик
        count = 11;
    } else {
        count--;
    }
}

