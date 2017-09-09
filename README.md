# arduino-timer-api
Cross-platform Arduino timer API.

Supported platforms:
- Arduino (AVR 16MHz, 16 bit)
- Arduino Due (SAM 84MHz, 32 bit)
- ChipKIT (PIC32MX 80MHz, 32 bit; PIC32MZ 200MHz - partially, wip)

# Введение и руководство

Многозадачная Ардуина: таймеры без боли
https://habrahabr.ru/post/337430/

# Установка

~~~bash
cd ~/Arduino/libraries/
git clone https://github.com/sadr0b0t/arduino-timer-api.git
~~~

и перезапустить среду Arduino.

Или на странице github нажать Clone or download > Download ZIP, затем установить архив arduino-timer-api-master.zip через меню установки библиотек в среде Arduino (Скетч > Подключить библиотеку > Добавить .ZIP библиотеку...).

Примеры должны появиться в меню _File/Examples/arduino-timer-api_

# Запуск таймера

~~~cpp
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
 *   use prescaler 1:8 (TIMER_PRESCALER_1_8) and adjustment=40000-1:
 *   16000000/8/50=40000, minus 1 cause count from zero.
 * 
 * 2) on 80MHz CPU (PIC32MX ChipKIT)
 *   use prescaler 1:64 (TIMER_PRESCALER_1_64) and adjustment=25000-1:
 *   80000000/64/50=25000, minus 1 cause count from zero.
 *
 * 3) on 84MHz CPU (SAM Arduino Due)
 *   use prescaler 1:128 (TIMER_PRESCALER_1_128) and adjustment=13125-1:
 *   80000000/128/50=13125, minus 1 cause count from zero.
 * 
 * Timer interrupt handler timer_handle_interrupts would be called every 20ms
 * (50 times per second == 50Hz freq) in this case.
 * 
 * @param timer
 *   system timer id: use TIMER_DEFAULT for default timer
 *   or _TIMER1...TIMER9, _TIMER1_32BIT..._TIMER9_32BIT for specific timer.
 *   note: availability of specific timer depends on the platform.
 * @param prescaler
 *   timer prescaler (1, 2, 4, 8, 16, 32, 64, 128, 256, 1024),
 *   use constants: PRESCALER_1_1, PRESCALER_1_2, PRESCALER_1_8,
 *   PRESCALER_1_16, PRESCALER_1_32, PRESCALER_1_64, PRESCALER_1_128
 *   PRESCALER_1_256, PRESCALER_1_1024
 *   note: availability of specific prescaler depends on the platform.
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
 * freq: 500KHz = 500000 ops/sec
 * period: 1sec/500000 = 2us
 */
void timer_init_ISR_500KHz(int timer);

/**
 * freq: 200KHz = 200000 ops/sec
 * period: 1sec/200000 = 5us
 */
void timer_init_ISR_200KHz(int timer);s

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
 * freq: 500Hz = 500 ops/sec
 * period: 1sec/500 = 2ms
 */
void timer_init_ISR_500Hz(int timer);

/**
 * freq: 200Hz = 200 ops/sec
 * period: 1sec/200 = 5ms
 */
void timer_init_ISR_200Hz(int timer);

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

/**
 * freq: 2Hz = 2 ops/sec
 * period: 1sec/2 = 500ms
 */
void timer_init_ISR_2Hz(int timer);

/**
 * freq: 1Hz = 1 ops/sec
 * period: 1sec
 */
void timer_init_ISR_1Hz(int timer);
~~~

## Выбор таймера

На всех платформах доступно значение TIMER_DEFAULT, которое указывает на рабочий таймер по умолчанию.

Если есть необходимость использовать другой таймер, можно указать его имя напрямую: _TIMER1...TIMER9, _TIMER1_32BIT..._TIMER9_32BIT

Однако для разных аппаратных платформ доступно разное количество таймеров с разными именами, поэтому указание конкретного таймера по имени может привести к потере кросс-платформенности.

Доступные таймеры:
- AVR ATmega1280, ATmega2560: _TIMER1, _TIMER3,_TIMER4, _TIMER5; TIMER_DEFAULT = _TIMER5
- AVR AT90USB646, AT90USB1286, ATmega128, ATmega1281, ATmega1284, ATmega1284P, AVR_ATmega2561:  _TIMER1, _TIMER3; TIMER_DEFAULT = _TIMER3
- AVR ATmega32U4: _TIMER1; TIMER_DEFAULT = _TIMER1
- AVR другие чипы: _TIMER1; TIMER_DEFAULT = _TIMER1

- PIC32 (ChipKIT): _TIMER1, _TIMER2, _TIMER3, _TIMER4, _TIMER5, _TIMER2_32BIT, _TIMER4_32BIT; TIMER_DEFAULT = _TIMER4_32BIT
- SAM (Arduino Due): _TIMER1/_TIMER1_32BIT, _TIMER2/_TIMER2_32BIT, _TIMER3/_TIMER3_32BIT, _TIMER4/_TIMER4_32BIT, _TIMER5/_TIMER5_32BIT, _TIMER6/_TIMER6_32BIT, _TIMER7/_TIMER7_32BIT, _TIMER8/_TIMER8_32BIT, _TIMER9/_TIMER9_32BIT (все таймеры 32-битные, _TIMERX_32BIT == _TIMERX); TIMER_DEFAULT = _TIMER3/_TIMER3_32BIT;


## Минимальные частоты и разрядность таймеров

Варианты вызовов timer_init_ISR_2Hz (2Гц, период 500мс) и timer_init_ISR_1Hz (1Гц, период 1с) на PIC32MX 80МГц будут работать только с 32-битными таймерами (_TIMER2_32BIT и _TIMER4_32BIT; TIMER_DEFAULT - по умолчанию = _TIMER4_32BIT), т.к. при 16-битных режимах таймеров PIC32MX 80МГц комбинация "делитель частоты" (prescaler - максимальный вариант 1/256) + "поправка периода" (adjustment - максимальный вариант 2^16=65536-1) дают минимальную частоту 5Гц (период - 200мс):  
80000000/256/65536 = 4.8Гц

На PIC32 32-битные таймеры создаются комбинацией 2х 16-битных таймеров:
- Timer4(32бит) = Timer4(16бит)+Timer5(16бит)
- Timer2(32бит) = Timer2(16бит)+Timer3(16бит)

поэтому при использовании таймера _TIMER2_32BIT, обычные таймеры _TIMER2 и _TIMER3 будут заняты, при использовании _TIMER4_32BIT - заняты будут _TIMER4 и _TIMER5.

На AVR/Arduino можно получить частоту 1Гц стандартными делителями на 16-битном таймере.

На SAM/Arduino Due все таймеры 32-битные.

Тип данных для параметра adjustment - unsigned int.
- На PIC32 разрядность int - 32 бит, этого хватит и для 16-тибитного режима таймера (если не закладывать значение более 2^16=65536-1) и для 32-битного (пойдет любое значение до 2^32=4294967296-1).
- На SAM разрядность int - 32 бит, все таймеры 32-битные.
- На AVR разрядность int - 16 бит, это опять же, как раз достаточно для 16-битных таймеров.

Таким образом, хотя разрядность параметра adjustment с типом int будет разной на разных платформах, во всех случаях значение будет соответствовать аппаратным свойствам таймеров.

## Максимальные частоты

Эксперименты с высокими частотами прерываний на разных чипах
- PIC32MX 80MHz (ChipKIT Uno32)
- AVR 16MHz (Arduino Leonardo)

cкетч: Examples/timer-api/timer-api-test-max-freq

Для PIC32MX 80МГц самый лучший надежный вариант - 200КГц: целевой период 5мкс совпадает с замерами. На 500КГц (целевой период 2мкс) замер периода показывает 4мкс (в 2 раза больше), но если просуммировать период 1000 циклов, получается 2006мкс, т.е. всего на 6 мкс больше, чем целевое значение (2*1000=2000мкс). Судя по всему, ошибка не накапливается по циклам, а является константой, т.е. скорее всего добавляется на одном из циклов, например, при проведении замера вызовом micros (т.е. на такой частоте сам замер дает значимую погрешность). Для частоты 1МГц (целевой период 1мкс) ошибка уже накапливается - на 1000 циклов суммарный период получается те же 2005мкс против целевых 1000мкс, т.е. частота для прерываний полностью не рабочая.

Для AVR 16МГц лучший относительно надежный вариант - 20КГц (целевой период - 50мкс): есть кое-какая погрешность +/-4 (на пике 6) микросекунды, но на сумме 2х вызовов период получается почти все время ровно 100мкс. Варианты 50КГц (период 20мкс) и 100КГц (период 10мкс), вообще, тоже работают, но та же погрешность +/4 микросекунды на одном периоде уже становится существенной по сравнений с целевым периодом, хотя на сумме 1000 периодов погрешность остаётся той же константой +/-4мкс. Судя по всему, эти +/4 микросекунды являются допустимой погрешностью при вызове прерывания на этом чипе. Вариант 200КГц (целевой период 5мкс) уже полностью не рабочий: на одном периоде замер показывает те же 12-16мкс, на сумме ошибка не остаётся константой, а нарастает (10 периодов - 84мкс вместо нужных 50ти).

~~~cpp
    // period = 1us
    // PIC32MX (ChipKIT Uno32): 3us, x2=5us, x10=21us, x1000=2005us (fail)
    // AVR (Arduino Leonardo): 12/16us, x10=80/84/92us (fail)
    timer_init_ISR_1MHz(TIMER_DEFAULT);
~~~
~~~cpp
    // period = 2us
    // PIC32MX (ChipKIT Uno32): 4us, x2=6us, x10=22us, x1000=2006us (+2/6us, ~fail)
    // AVR (Arduino Leonardo): 12/16us, x10=84us (fail)
    timer_init_ISR_500KHz(TIMER_DEFAULT);
~~~
~~~cpp
    // period = 5us
    // PIC32MX (ChipKIT Uno32): 5us, x10=50us (ok)
    // AVR (Arduino Leonardo): 12/16us, x2=20us, x10=84us (fail)
    timer_init_ISR_200KHz(TIMER_DEFAULT);
~~~
~~~cpp
    // period = 10us
    // PIC32MX (ChipKIT Uno32): 10us (ok)
    // AVR (Arduino Leonardo): 12us, x2=20us, x10=100/104/108us, x1000=10000  (~ok)
    timer_init_ISR_100KHz(TIMER_DEFAULT);
~~~
~~~cpp
    // period = 20us
    // PIC32MX (ChipKIT Uno32): 20us (ok)
    // AVR (Arduino Leonardo): 12/20/24us, x2=40/44/48us, x1000=19996/20000/20004 (~ok)
    timer_init_ISR_50KHz(TIMER_DEFAULT);
~~~
~~~cpp
    // period = 50us
    // PIC32MX (ChipKIT Uno32): 50us (ok)
    // AVR (Arduino Leonardo): 48/52/56us, x2=100us (ok)
    timer_init_ISR_20KHz(TIMER_DEFAULT);
~~~
~~~cpp
    // period = 100us
    // PIC32MX (ChipKIT Uno32): 100us (ok)
    // AVR (Arduino Leonardo): 96/100/104us, x2=200/204us (ok)
    timer_init_ISR_10KHz(TIMER_DEFAULT);
~~~


# Произвольная частота

~~~cpp
#include"timer-api.h"

На разных платформах доступны разные варианты значений делителя частоты prescaler. В случае указания некорректного значения делителя prescaler поведения системы  не определено.

- AVR (Arduino): TIMER_PRESCALER_1_1, TIMER_PRESCALER_1_8, TIMER_PRESCALER_1_64, TIMER_PRESCALER_1_256, TIMER_PRESCALER_1_1024
- SAM (Arduino Due): TIMER_PRESCALER_1_2, TIMER_PRESCALER_1_8, TIMER_PRESCALER_1_32, TIMER_PRESCALER_1_128
- PIC32 (ChiptKIT): PRESCALER_1, PRESCALER_2, PRESCALER_4, PRESCALER_8, PRESCALER_16, PRESCALER_32, PRESCALER_64, PRESCALER_256

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
    static unsigned long prev_time = micros();

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

