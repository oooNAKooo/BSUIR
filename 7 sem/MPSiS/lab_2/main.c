#include <msp430.h>

#define LED1 BIT0      // Индикация активного режима (P1.0)
#define LED2 BIT1      // Резерв (P8.1)
#define LED3 BIT2      // Индикация режима пониженного энергопотребления (P8.2)
#define BUTTON BIT7    // Кнопка S1 подключена к P1.7

void setup_clock();    // Прототип функции настройки тактирования
void delay(unsigned int cycles); // Прототип функции задержки

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Останавливаем watchdog таймер

    // Настройка портов для светодиодов
    P1DIR |= LED1;                // Устанавливаем LED1 как выход
    P1OUT &= ~LED1;               // Изначально LED1 выключен

    P8DIR |= LED2 + LED3;         // Устанавливаем LED2 и LED3 как выходы
    P8OUT &= ~(LED2 + LED3);      // Изначально LED2 и LED3 выключены

    // Настройка кнопки
    P1DIR &= ~BUTTON;             // Устанавливаем кнопку как вход
    P1REN |= BUTTON;              // Включаем подтягивающий резистор кнопки
    P1OUT |= BUTTON;              // Подтягиваем резистор к Vcc

    unsigned char button_state = 0; // Переменная для отслеживания состояния кнопки

    setup_clock();  // Настройка тактирования

    while (1)
    {
        if ((P1IN & BUTTON) == 0) // Проверяем, нажата ли кнопка (низкий уровень)
        {
            __delay_cycles(20000); // Антидребезг

            if ((P1IN & BUTTON) == 0) // Дополнительная проверка кнопки
            {
                button_state ^= 1; // Инвертируем состояние кнопки

                if (button_state == 1) // Если кнопка нажата первый раз
                {
                    P1OUT |= LED1;     // Включаем LED1 (индикация активного режима)
                    delay(10000);      // Задержка
                    P8OUT |= LED2;     // Включаем LED2
                    delay(10000);      // Задержка
                    P8OUT |= LED3;     // Включаем LED3
                }
                else // Если кнопка нажата повторно
                {
                    P1OUT &= ~LED1;    // Выключаем LED1
                    P8OUT &= ~(LED2 + LED3); // Выключаем LED2 и LED3
                    // Входим в режим LPM1 (режим пониженного энергопотребления)
                    __bis_SR_register(LPM1_bits + GIE); // Включаем LPM1
                }

                while ((P1IN & BUTTON) == 0); // Ожидаем, пока кнопку отпустят
            }
        }
    }
}

// Настройка тактовых частот
void setup_clock() {
    // Настраиваем DCO на 3.1 МГц, SMCLK с делителем /16
    UCSCTL3 = SELREF__REFOCLK;         // Источник опорной частоты для FLL - REFO
    UCSCTL4 = SELA__REFOCLK;           // ACLK на REFO

    UCSCTL1 = DCORSEL_4;               // Диапазон 3.1 МГц для DCO
    UCSCTL2 = FLLD__16 + 63;           // Делитель /16 и множитель (3.1 МГц)

    // Ожидание стабилизации частоты
    __delay_cycles(250000);
}

// Функция для создания задержки
void delay(unsigned int cycles)
{
    while (cycles--)
    {
        __no_operation(); // Ничего не делаем, просто ждем
    }
}
