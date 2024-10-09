#include <msp430.h>

// Прототип функции настройки АЦП
void configureADC(); 

// Прототип функции для чтения с канала АЦП
unsigned int readADC(unsigned int channel); 

#define LED1 BIT0  // Светодиод 1 подключен к P1.0
#define LED3 BIT2  // Светодиод 3 подключен к P8.2

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;  // Останавливаем watchdog таймер

    // Настройка портов для светодиодов
    P1DIR |= LED1;  // Устанавливаем LED1 как выход
    P1OUT &= ~LED1;  // Изначально LED1 выключен

    P8DIR |= LED3;  // Устанавливаем LED3 как выход
    P8OUT &= ~LED3;  // Изначально LED3 выключен

    configureADC();  // Настраиваем АЦП

    while (1)
    {
        // Чтение термодатчика (канал 10) и опорного напряжения (канал 11)
        unsigned int tempValue = readADC(10);  // Чтение термодатчика
        // tempValue += 50;  // Добавляем 50 для тестирования (например)

        unsigned int vrefValue = readADC(11);  // Читаем опорное напряжение

        // Сравниваем значения термодатчика и опорного напряжения
        if (tempValue > vrefValue) {
            P1OUT |= LED1;  // Включаем LED1, если температура выше опорного напряжения
            P8OUT &= ~LED3;  // Выключаем LED3
        } else {
            P1OUT &= ~LED1;  // Выключаем LED1
            P8OUT |= LED3;  // Включаем LED3, если опорное напряжение выше или равно
        }
    }
}

// Настройка АЦП
void configureADC()
{
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;  // Устанавливаем время выборки и включаем АЦП
    ADC12CTL1 = ADC12SHP;  // Используем источник от таймера
    ADC12CTL2 = ADC12RES_2;  // Устанавливаем разрешение 12 бит
    ADC12MCTL0 = ADC12INCH_10 | ADC12SREF_1;  // Канал 10 (термодатчик), опорное напряжение
    ADC12MCTL1 = ADC12INCH_11 | ADC12SREF_1;  // Канал 11 (опорное напряжение Vref+)
    ADC12CTL0 |= ADC12ENC;  // Разрешаем измерение
}

// Чтение с канала АЦП
unsigned int readADC(unsigned int channel)
{
    ADC12CTL0 &= ~ADC12SC;  // Сброс флага старта
    ADC12CTL0 |= ADC12SC;  // Старт нового преобразования
    while (ADC12CTL1 & ADC12BUSY);  // Ожидание завершения
    return ADC12MEM0;  // Возвращаем результат
}
