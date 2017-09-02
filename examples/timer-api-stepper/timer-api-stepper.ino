#include"timer-api.h"

// Rotate stepper motor in background
// Вращение шаговым моторов в фоновом режиме

// Pinout for CNC-shield
// http://blog.protoneer.co.nz/arduino-cnc-shield/

// X
#define STEP_PIN 2
#define DIR_PIN 5
#define EN_PIN 8

// Y
//#define STEP_PIN 3
//#define DIR_PIN 6
//#define EN_PIN 8

// Z
//#define STEP_PIN 4
//#define DIR_PIN 7
//#define EN_PIN 8

void setup() {
    Serial.begin(9600);

    // step-dir motor driver pins
    // пины драйвера мотора step-dir
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    pinMode(EN_PIN, OUTPUT);

    // Будем вращать мотор с максимальной скоростью,
    // для разных настроек делителя шага оптимальная
    // частота таймера будет разная.
    // Оптимальные варианты задержки между шагами
    // для разных делителей:
    // https://github.com/sadr0b0t/stepper_h
    // 1/1: 1500 мкс
    // 1/2: 650 мкс
    // 1/4: 330 мкс
    // 1/8: 180 мкс
    // 1/16: 80 мкс
    // 1/32: 40 мкс


    // Делилель шага 1/1
    // freq=500Hz, period=2ms
    // частота=500Гц, период=2мс
    //timer_init_ISR_500Hz(TIMER_DEFAULT);
    // помедленнее
    timer_init_ISR_200Hz(TIMER_DEFAULT);


    // Делилель шага 1/2
    // freq=1KHz, period=1ms
    // частота=1КГц, период=1мс
    //timer_init_ISR_1KHz(TIMER_DEFAULT);
    // помедленнее
    //timer_init_ISR_500Hz(TIMER_DEFAULT);

    // Делилель шага 1/4
    // freq=2KHz, period=500us
    // частота=2КГц, период=500мкс
    //timer_init_ISR_2KHz(TIMER_DEFAULT);
    // помедленнее
    //timer_init_ISR_1KHz(TIMER_DEFAULT);

    // Делилель шага 1/8
    // freq=5KHz, period=200us
    // частота=5КГц, период=200мкс
    //timer_init_ISR_5KHz(TIMER_DEFAULT);
    // помедленнее
    //timer_init_ISR_2KHz(TIMER_DEFAULT);
    
    // Делилель шага 1/16
    // freq=10KHz, period=100us
    // частота=10КГц, период=100мкс
    //timer_init_ISR_10KHz(TIMER_DEFAULT);
    // помедленнее
    //timer_init_ISR_5KHz(TIMER_DEFAULT);
    
    // Делилель шага 1/32
    // freq=20KHz, period=50us
    // частота=20КГц, период=50мкс
    //timer_init_ISR_20KHz(TIMER_DEFAULT);
    // помедленнее
    //timer_init_ISR_10KHz(TIMER_DEFAULT);

    /////////
    // выключим мотор на старте
    // EN=HIGH to disable
    digitalWrite(EN_PIN, HIGH);

    // enter command from keyboard
    // просим ввести направление с клавиатуры
    Serial.println("Choose direction: '<' '>', space or 's' to stop");
}

void loop() {
    if(Serial.available() > 0) {
        // read the incoming byte:
        int inByte = Serial.read();
        if(inByte == '<' || inByte == ',') {
            Serial.println("go back");
            
            // go back
            // назад
            digitalWrite(DIR_PIN, HIGH);
            
            // EN=LOW to enable
            digitalWrite(EN_PIN, LOW);
        } else if(inByte == '>' || inByte == '.') {
            Serial.println("go forth");

            // go forth
            // вперед
            digitalWrite(DIR_PIN, LOW);
            
            // EN=LOW to enable
            digitalWrite(EN_PIN, LOW);
        } else if(inByte == ' ' || inByte == 's') {
            Serial.println("stop");
            
            // stop
            // стоп
            
            // EN=HIGH to disable
            digitalWrite(EN_PIN, HIGH);
        } else {
            Serial.println("press '<' or '>' to choose direction, space or 's' to stop,");
        }
    }
    delay(100);
}

void timer_handle_interrupts(int timer) {
    // step on HIGH->LOW front
    // шаг на фронте HIGH->LOW
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(1);
    digitalWrite(STEP_PIN, LOW);
}

