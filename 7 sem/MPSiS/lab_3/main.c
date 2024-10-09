#include <msp430.h>

void setup_clock();             // Прототип функции настройки тактирования
void toggle_led(int led);       // Прототип функции переключения светодиодов
unsigned int level = 3;         // Переменная для уровня питания Vcore

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Останавливаем watchdog таймер

    // Настройка портов для светодиодов
    P1DIR |= BIT0 + BIT1;       // LED1 и LED2 как выходы
    P8DIR |= BIT2;              // LED3 как выход
    P1OUT &= ~(BIT0 + BIT1);    // Отключаем LED1 и LED2
    P8OUT &= ~BIT2;             // Отключаем LED3

    // Настройка кнопки S1
    P1DIR &= ~BIT7;             // Кнопка S1 как вход
    P1REN |= BIT7;              // Включаем подтягивающий резистор на S1
    P1OUT |= BIT7;              // Подтягиваем к Vcc

    P1IE |= BIT7;               // Разрешаем прерывание от S1
    P1IES |= BIT7;              // Прерывание по спаду
    P1IFG &= ~BIT7;             // Очищаем флаг прерывания

    setup_clock();              // Настраиваем тактовую частоту

    __bis_SR_register(GIE);     // Включаем глобальные прерывания

    while (1)
    {
        __no_operation();       // Ожидание события
    }
}

// Настройка тактирования
void setup_clock()
{
    // Настройка DCO на 3.1 МГц и делитель SMCLK на /16
    UCSCTL3 = SELREF__REFOCLK;         // Источник опорной частоты для FLL — REFO
    UCSCTL4 = SELA__REFOCLK;           // ACLK на REFO
    UCSCTL1 = DCORSEL_4;               // Диапазон 3.1 МГц для DCO
    UCSCTL2 = FLLD__16 + 63;           // Делитель /16 и множитель (3.1 МГц)
    __delay_cycles(250000);            // Ожидание стабилизации частоты
}

// Прерывание от кнопки S1
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (P1IFG & BIT7)
    {
        P1IFG &= ~BIT7;  // Очищаем флаг прерывания

        if (P1OUT & BIT0)  // Если LED1 включен, переходим в режим LPM1
        {
            P1OUT &= ~BIT0;      // Выключаем LED1 (индикация выхода из активного режима)
            P8OUT |= BIT2;       // Включаем LED3 (индикация режима энергосбережения)
            __bis_SR_register(LPM1_bits + GIE); // Вход в LPM1
        }
        else
        {
            P8OUT &= ~BIT2;      // Выключаем LED3 (индикация энергосбережения)
            P1OUT |= BIT0;       // Включаем LED1 (индикация активного режима)
            __bic_SR_register_on_exit(LPM1_bits); // Выход из LPM1
        }
    }
}
