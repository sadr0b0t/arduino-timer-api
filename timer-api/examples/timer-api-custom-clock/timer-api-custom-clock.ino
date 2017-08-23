#include"timer-api.h"

void setup() {
    Serial.begin(9600);

    // http://www.robotshop.com/letsmakerobots/arduino-101-timers-and-interrupts
    // 1. CPU frequency 16Mhz for Arduino
    // 2. maximum timer counter value (256 for 8bit, 65536 for 16bit timer)
    // 3. Divide CPU frequency through the choosen prescaler (16000000 / 256 = 62500)
    // 4. Divide result through the desired frequency (62500 / 2Hz = 31250)
    // 5. Verify the result against the maximum timer counter value (31250 < 65536 success) if fail, choose bigger prescaler.
    
    // Arduino 16МГц
    // Настроим и запустим таймер с периодом 20 миллисекунд (50 срабатываний в секунду == 50Гц):
    // prescaler=1:8, adjustment=40000:
    // 16000000/8/50=40000 (50Hz - срабатывает 50 раз в секунду, т.е. каждые 20мс)
    // Обработчик прерывания от таймера - функция timer_handle_interrupts 
    // (с заданными настройками будет вызываться каждые 20мс).
    //timer_init_ISR(TIMER_DEFAULT, TIMER_PRESCALER_1_8, 40000-1);
    
    // ChipKIT PIC32MX 80МГц
    // Настроим и запустим таймер с периодом 20 миллисекунд (50 срабатываний в секунду == 50Гц):
    // prescaler=1:64, adjustment=25000:
    // 80000000/64/25000=50 (срабатывает 50 раз в секунду, т.е. каждые 20мс)
    // Обработчик прерывания от таймера - функция timer_handle_interrupts 
    // (с заданными настройками будет вызываться каждые 20мс).
    timer_init_ISR(TIMER_DEFAULT, TIMER_PRESCALER_1_64, 25000-1);
    
    pinMode(13, OUTPUT);
}

void loop() {
    Serial.println("Hello from loop!");
    delay(5000);

    // any code here: blocking or non-blicking
}


/**
 * Процедура, вызываемая прерыванием по событию таймера с заданным периодом.
 */
void timer_handle_interrupts(int timer) {
    static long prev_time = micros();

    long _time = micros();
    long _period = _time - prev_time;
    prev_time = _time;

    static int count = 50;

    // печатаем статус каждые 50 вызовов прерывания,
    // на частоте 50Гц - 1 раз в секунду
    if(count == 0) {
        Serial.print("goodbye from timer: ");
        Serial.println(_period, DEC);
        
        digitalWrite(13, !digitalRead(13));
        
        count = 50;
    }
    count--;
}

