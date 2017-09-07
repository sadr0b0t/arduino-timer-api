#include"timer-api.h"

void setup() {
    Serial.begin(9600);

    // http://www.robotshop.com/letsmakerobots/arduino-101-timers-and-interrupts
    // 1. CPU frequency 16Mhz for Arduino
    // 2. maximum timer counter value (256 for 8bit, 65536 for 16bit timer)
    // 3. Divide CPU frequency through the choosen prescaler (16000000 / 256 = 62500)
    // 4. Divide result through the desired frequency (62500 / 2Hz = 31250)
    // 5. Verify the result against the maximum timer counter value (31250 < 65536 success).
    //    If fail, choose bigger prescaler.
    
    // Arduino 16МГц
    // Настроим и запустим таймер с периодом 20 миллисекунд (50 срабатываний в секунду == 50Гц):
    // prescaler=1:8, adjustment=40000-1:
    // 16000000/8/50=40000 (50Hz - срабатывает 50 раз в секунду, т.е. каждые 20мс),
    // минус 1, т.к. считаем от нуля.
    // Обработчик прерывания от таймера - функция timer_handle_interrupts 
    // (с заданными настройками будет вызываться каждые 20мс).
    timer_init_ISR(TIMER_DEFAULT, TIMER_PRESCALER_1_8, 40000-1);
    
    // ChipKIT PIC32MX 80МГц
    // Настроим и запустим таймер с периодом 20 миллисекунд (50 срабатываний в секунду == 50Гц):
    // prescaler=1:64, adjustment=25000-1:
    // 80000000/64/25000=50 (срабатывает 50 раз в секунду, т.е. каждые 20мс),
    // минус 1, т.к. считаем от нуля.
    // Обработчик прерывания от таймера - функция timer_handle_interrupts 
    // (с заданными настройками будет вызываться каждые 20мс).
    //timer_init_ISR(TIMER_DEFAULT, TIMER_PRESCALER_1_64, 25000-1);
    
    // Arduino Due SAM 84МГц
    // Настроим и запустим таймер с периодом 20 миллисекунд (50 срабатываний в секунду == 50Гц):
    // prescaler=1:128, adjustment=13125-1:
    // 80000000/128/13125=50 (срабатывает 50 раз в секунду, т.е. каждые 20мс),
    // минус 1, т.к. считаем от нуля.
    // Обработчик прерывания от таймера - функция timer_handle_interrupts 
    // (с заданными настройками будет вызываться каждые 20мс).
    //timer_init_ISR(TIMER_DEFAULT, TIMER_PRESCALER_1_128, 13125-1);
    
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

    static int count = 49;

    // print status every 50 interrupt call,
    // on timer freq 50Hz - once per second
    // печатаем статус каждые 50 вызовов прерывания,
    // на частоте таймера 50Гц - 1 раз в секунду
    if(count == 0) {
        Serial.print("goodbye from timer: ");
        Serial.println(_period, DEC);
        
        digitalWrite(13, !digitalRead(13));
        
        count = 49;
    } else {
        count--;
    }
}

