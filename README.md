# arduino-timer-api
Cross-platform Arduino timer API.

Supported platforms:
- Arduino (AVR 16MHz)
- ChipKIT (PIC32MX 80MHz; PIC32MZ 200MHz - partially, wip)

# Установка

```bash
git clone https://github.com/1i7/arduino-timer-api.git
cp -r ./arduino-timer-api/timer-api ~/Arduino/libraries/
```
Перезапустить среду Arduino. Примеры должны появиться в меню _File/Examples/timer-api_

# Запуск таймера

~~~cpp
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

    // any code here: blocking or non-blicking
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
~~~

# API

## Базовые вызовы
~~~cpp
/**
 * Init ISR (Interrupt service routine) for the timer and start timer.
 * 
 * General algorithm
 * http://www.robotshop.com/letsmakerobots/arduino-101-timers-and-interrupts
 * 1. CPU frequency 16Mhz for Arduino
 * 2. maximum timer counter value (256 for 8bit, 65536 for 16bit timer)
 * 3. Divide CPU frequency through the choosen prescaler (16000000 / 256 = 62500)
 * 4. Divide result through the desired frequency (62500 / 2Hz = 31250)
 * 5. Verify the result against the maximum timer counter value (31250 < 65536 success).
 *    If fail, choose bigger prescaler.
 * 
 * Example: to set timer clock period to 20ms (50 operations per second == 50Hz)
 * 
 * 1) on 16MHz CPU (AVR Arduino)
 *   use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=40000:
 *   16000000/8/50=40000
 * 
 * 2) on 80MHz CPU (PIC32MX ChipKIT)
 *   use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=25000:
 *   80000000/64/50=25000
 * 
 * Timer interrupt handler timer_handle_interrupts would be called every 20ms
 * (50 times per second == 50Hz freq) in this case.
 * 
 * @param timer
 *   system timer id: use TIMER_DEFAULT for default timer
 *   or TIMER1, TIMER3, TIMER4 or TIMER5 for specific timer.
 *   note: TIMERX constant would be set to '-1' if selected timer
 *   is not available on current platform.
 * @param prescaler
 *   timer prescaler (1, 2, 4, 8, 16, 32, 64, 256),
 *   use constants: PRESCALER_1, PRESCALER_2, PRESCALER_8,
 *   PRESCALER_16, PRESCALER_32, PRESCALER_64, PRESCALER_256
 * @param adjustment
 *   adjustment divider after timer prescaled - timer compare match value.
 */
void timer_init_ISR(int timer, int prescaler, int adjustment);

/**
 * Stop ISR (Interrupt service routine) for the timer.
 * 
 * @param timer
 *     system timer id for started ISR
 */
void timer_stop_ISR(int timer);

/**
 * Timer ISR (Interrupt service routine) handler.
 * Implementation must be provided in module with user code.
 * 
 * @param timer
 *     system timer id for started ISR 
 */
void timer_handle_interrupts(int timer);
~~~

## Встроенные варианты частот
~~~cpp
/**
 * freq: 100KHz = 100000 ops/sec
 * period: 1sec/100000 = 10us
 */
void timer_init_ISR_100KHz(int timer);

/**
 * freq: 50KHz = 50000 ops/sec
 * period: 1sec/50000 = 20us
 */
void timer_init_ISR_50KHz(int timer);

/**
 * freq: 20KHz = 20000 ops/sec
 * period: 1sec/20000 = 50us
 */
void timer_init_ISR_20KHz(int timer);

/**
 * freq: 10KHz = 10000 ops/sec
 * period: 1sec/10000 = 100us
 */
void timer_init_ISR_10KHz(int timer);

/**
 * freq: 5KHz = 5000 ops/sec
 * period: 1sec/5000 = 200us
 */
void timer_init_ISR_5KHz(int timer);

/**
 * freq: 2KHz = 2000 ops/sec
 * period: 1sec/2000 = 500us
 */
void timer_init_ISR_2KHz(int timer);

/**
 * freq: 1KHz = 1000 ops/sec
 * period: 1sec/1000 = 1ms
 */
void timer_init_ISR_1KHz(int timer);

/**
 * freq: 100Hz = 100 ops/sec
 * period: 1sec/100 = 10ms
 */
void timer_init_ISR_100Hz(int timer);

/**
 * freq: 50Hz = 50 ops/sec
 * period: 1sec/50 = 20ms
 */
void timer_init_ISR_50Hz(int timer);

/**
 * freq: 20Hz = 20 ops/sec
 * period: 1sec/20 = 50ms
 */
void timer_init_ISR_20Hz(int timer);

/**
 * freq: 10Hz = 10 ops/sec
 * period: 1sec/10 = 100ms
 */
void timer_init_ISR_10Hz(int timer);

/**
 * freq: 5Hz = 5 ops/sec
 * period: 1sec/5 = 200ms
 */
void timer_init_ISR_5Hz(int timer);
~~~

_Замечание_  
Варианты вызовов timer_init_ISR_2Hz (2Гц, период 500мс) и timer_init_ISR_1Hz (1Гц, период 1с) пока отключены, т.к. при 16-битных режимах таймеров PIC32MX 80МГц комбинация делитель частоты (prescaler - максимальный вариант 1/256) + поправка периода (adjustment - максимальный вариант 2^16=65536) дают минимальную частоту 5Гц (период - 200мс):  
80000000/256/65536 = 4.8Гц

Чтобы сделать еще меньше, нужно переводить таймер в режим 32 бит (при этом на PIC32 не все таймеры работают в 32-битном режиме, а те, которые работают, занимают ресурсы 2го таймера: например, Timer4(16бит)+Timer5(16бит)=Timer4(32бит)).

На Ардуине можно получить частоту 1Гц стандартными делителями, но без аналогичного вариата для PIC32 вызовы не получится сделать кросс-платформенными.

# Собственная частота

~~~cpp
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
~~~

# Ссылки
### AVR (Arduino)
http://www.robotshop.com/letsmakerobots/arduino-101-timers-and-interrupts  
http://playground.arduino.cc/Code/Timer1  
https://github.com/arduino/Arduino/blob/ide-1.5.x/libraries/Servo/src/avr/Servo.cpp  
https://github.com/arduino/Arduino/blob/ide-1.5.x/libraries/Servo/src/avr/ServoTimers.h

### PIC32 (ChipKIT)
PIC32 family reference manual Section 14. Timers  
http://umassamherstm5.org/tech-tutorials/pic32-tutorials/ubw32-tutorials/3-interrupts  
http://umassamherstm5.org/tech-tutorials/pic32-tutorials/ubw32-tutorials/3-interrupts  
https://github.com/chipKIT32/chipKIT-core/blob/master/pic32/libraries/Servo/utility/int.c

