#include <msp430.h> // Подключаем заголовочный файл MSP430

#define LED1 BIT0  // Светодиод 1 подключен к P1.0 #define LED2 BIT1  // Светодиод 2 подключен к P8.1 #define LED3 BIT2  // Светодиод 3 подключен к P8.2 #define BUTTON BIT7  // Кнопка подключена к P1.7
void delay(unsigned int cycles); // Прототип функции задержки кнопки

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Останавливаем watchdog таймер

    // Настройка портов для светодиодов
    P1DIR |= LED1;	// Устанавливаем LED1 как выход
    P1OUT &= ~LED1;	// Изначально LED1 выключен
    
    P8DIR |= LED2 + LED3;	// Устанавливаем LED2 и LED3 как выходы P8OUT &= ~(LED2 + LED3);	// Изначально LED2 и LED3 выключены
    
    // Настройка кнопки
    P1DIR &= ~BUTTON;	// Устанавливаем кнопку как вход
    P1REN |= BUTTON;	// Включаем внутренний резистор подтяжки P1OUT |= BUTTON;		// Подтягиваем резистор к Vcc
    unsigned char button_state = 0; // Переменная для отслеживания состояния кнопки
    while (1)
    {
        if ((P1IN & BUTTON) == 0) // Проверяем, нажата ли кнопка (низкий уровень)
        {
            delay_cycles(20000); // Задержка для антидребезга
            if ((P1IN & BUTTON) == 0) // Дополнительная проверка кнопки
            {
                 button_state ^= 1; // Инвертируем состояние кнопки
                 if (button_state == 1) // Если кнопка нажата первый раз
                 {
                    P1OUT |= LED1;	// Включаем LED1 delay(10000);	// Задержка P8OUT |= LED2;	// Включаем LED2 delay(10000);	// Задержка P8OUT |= LED3;	// Включаем LED3
                 }
                 else // Если кнопка нажата повторно
                 {
                P1OUT &= ~LED1;	// Выключаем LED1
                P8OUT &= ~(LED2 + LED3); // Выключаем LED2 и LED3
                }
              while ((P1IN & BUTTON) == 0); // Ожидаем, пока кнопку отпустят
            }
        }
    }
 }
 
// Функция для создания задержки
void delay(unsigned int cycles)
{
    while (cycles--)
    {
      no_operation(); // Ничего не делаем, просто ждем
    }
}
