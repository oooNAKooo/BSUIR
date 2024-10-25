#include <msp430.h>
#include <math.h>
#include <stdint.h>

// Определение команд ЖКИ
#define SET_COLUMN_ADDRESS_LSB 0x00
#define SET_COLUMN_ADDRESS_MSB 0x10
#define SET_PAGE_ADDRESS 0xB0
#define SET_SEG_DIRECTION 0xA0
#define SET_COM_DIRECTION 0xC0
#define SET_POWER_CONTROL 0x2F
#define SET_SCROLL_LINE 0x40
#define SET_VLCD_RESISTOR_RATIO 0x27
#define SET_ELECTRONIC_VOLUME_MSB 0x81
#define SET_ELECTRONIC_VOLUME_LSB 0x0F
#define SET_ALL_PIXEL_ON 0xA4
#define SET_INVERSE_DISPLAY 0xA6
#define SET_DISPLAY_ENABLE 0xAF
#define SET_LCD_BIAS_RATIO 0xA2
#define SET_ADV_PROGRAM_CONTROL0_MSB 0xFA
#define SET_ADV_PROGRAM_CONTROL0_LSB 0x90

#define DOUTX 0x06  // Адрес регистра оси X акселерометра
#define G_RANGE_2 0x80  // Диапазон 2g
#define I2C_DIS 0x10    // Отключение I2C
#define MODE_400 0x04   // Режим измерений 400 Гц
#define NONE 0
#define CD BIT6
#define CS BIT4

uint8_t Dogs102x6_initMacro[] = {
    SET_SCROLL_LINE,
    SET_SEG_DIRECTION,
    SET_COM_DIRECTION,
    SET_ALL_PIXEL_ON,
    SET_INVERSE_DISPLAY,
    SET_LCD_BIAS_RATIO,
    SET_POWER_CONTROL,
    SET_VLCD_RESISTOR_RATIO,
    SET_ELECTRONIC_VOLUME_MSB,
    SET_ELECTRONIC_VOLUME_LSB,
    SET_ADV_PROGRAM_CONTROL0_MSB,
    SET_ADV_PROGRAM_CONTROL0_LSB,
    SET_DISPLAY_ENABLE,
    SET_PAGE_ADDRESS,
    SET_COLUMN_ADDRESS_MSB,
    SET_COLUMN_ADDRESS_LSB
};

int MirrorColMode = 0;
int MAPPING_VALUES[] = { 4571, 2286, 1141, 571, 286, 143, 71 };
uint8_t BITx[] = { BIT6, BIT5, BIT4, BIT3, BIT2, BIT1, BIT0 };
uint8_t symbols[12][11] = {
    {0x20, 0x20, 0x20, 0x20, 0x20, 0xF8, 0x20, 0x20, 0x20, 0x20, 0x20}, // plus
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00}, // minus
    {0xF8, 0xF8, 0xD8, 0xD8, 0xD8, 0xD8, 0xD8, 0xD8, 0xD8, 0xF8, 0xF8}, // num0
    {0xF8, 0xF8, 0x30, 0x30, 0x30, 0x30, 0xF0, 0xF0, 0x70, 0x70, 0x30}, // num1
    {0xF8, 0xF8, 0xC0, 0xC0, 0xC0, 0xF8, 0xF8, 0x18, 0x18, 0xF8, 0xF8}, // num2
    {0xF8, 0xF8, 0x18, 0x18, 0x18, 0xF8, 0xF8, 0x18, 0x18, 0xF8, 0xF8}, // num3
    {0x18, 0x18, 0x18, 0x18, 0xF8, 0xF8, 0xD8, 0xD8, 0xD8, 0xD8, 0xD8}, // num4
    {0xF8, 0xF8, 0x18, 0x18, 0x18, 0xF8, 0xF8, 0xC0, 0xC0, 0xF8, 0xF8}, // num5
    {0xF8, 0xF8, 0xD8, 0xD8, 0xD8, 0xF8, 0xF8, 0xC0, 0xC0, 0xF8, 0xF8}, // num6
    {0xC0, 0xC0, 0xE0, 0x70, 0x38, 0x18, 0x18, 0x18, 0x18, 0xF8, 0xF8}, // num7
    {0xF8, 0xF8, 0xD8, 0xD8, 0xD8, 0xF8, 0xD8, 0xD8, 0xD8, 0xF8, 0xF8}, // num8
    {0xF8, 0xF8, 0x18, 0x18, 0xF8, 0xF8, 0xD8, 0xD8, 0xD8, 0xF8, 0xF8}  // num9
};

// Прототипы функций
void Dogs102x6_clearScreen(void);
void Dogs102x6_setAddress(uint8_t pa, uint8_t ca);
void Dogs102x6_writeData(uint8_t* sData, uint8_t i);
void Dogs102x6_writeCommand(uint8_t* sCmd, uint8_t i);
void printNumber(long int angle);
long int parseProjectionByte(uint8_t projection_byte);
int calculateAngleFromProjection(double projection);
int8_t Cma3000_readRegister(int8_t Address);
void Dogs102x6_setMirrorColDisplay();
void setupButtons();

#pragma vector = PORT2_VECTOR
__interrupt void accelerometerInterrupt(void) {
    // Чтение данных только с оси X
    volatile uint8_t xProjectionByte = Cma3000_readRegister(DOUTX);
    volatile long int xAxisProjection = parseProjectionByte(xProjectionByte);

    // Расчет угла для оси X
    int angle = calculateAngleFromProjection((double) xAxisProjection);

    // Очистка экрана и отображение угла
    Dogs102x6_clearScreen();
    printNumber(angle);
}

#pragma vector = PORT1_VECTOR
__interrupt void buttonS1(void) {
    __delay_cycles(27000);
    if ((P1IN & BIT7) == 0) {
        // Переключение зеркального отображения
        MirrorColMode = !MirrorColMode;
        Dogs102x6_setMirrorColDisplay();
        Dogs102x6_clearScreen();
    }
    P1IFG = 0;  // Очистка флага прерывания
}

// Очистка экрана ЖКИ
void Dogs102x6_clearScreen(void) {
    uint8_t LcdData[] = { 0x00 };
    uint8_t p, c;
    for (p = 0; p < 8; p++) {
        Dogs102x6_setAddress(p, 0);
        for (c = 0; c < 132; c++) {
            Dogs102x6_writeData(LcdData, 1);
        }
    }
}

// Установка адреса на ЖКИ
void Dogs102x6_setAddress(uint8_t pa, uint8_t ca) {
    uint8_t cmd[1];
    if (pa > 7) {
        pa = 7;
    }
    if (ca > 131) {
        ca = 131;
    }
    cmd[0] = SET_PAGE_ADDRESS + pa;
    uint8_t H = 0x00;
    uint8_t L = 0x00;
    uint8_t ColumnAddress[] = { SET_COLUMN_ADDRESS_MSB, SET_COLUMN_ADDRESS_LSB };
    L = (ca & 0x0F);
    H = (ca & 0xF0);
    H = (H >> 4);
    ColumnAddress[0] = SET_COLUMN_ADDRESS_LSB + L;
    ColumnAddress[1] = SET_COLUMN_ADDRESS_MSB + H;
    Dogs102x6_writeCommand(cmd, 1);
    Dogs102x6_writeCommand(ColumnAddress, 2);
}

// Вывод данных на ЖКИ
void Dogs102x6_writeData(uint8_t* sData, uint8_t i) {
    P7OUT &= ~CS;
    P5OUT |= CD;
    while (i) {
        while (!(UCB1IFG & UCTXIFG));
        UCB1TXBUF = *sData;
        sData++;
        i--;
    }
    while (UCB1STAT & UCBUSY);
    P7OUT |= CS;
}

// Отправка команд на ЖКИ
void Dogs102x6_writeCommand(uint8_t* sCmd, uint8_t i) {
    P7OUT &= ~CS;
    P5OUT &= ~CD;
    while (i) {
        while (!(UCB1IFG & UCTXIFG));
        UCB1TXBUF = *sCmd;
        sCmd++;
        i--;
    }
    while (UCB1STAT & UCBUSY);
    P7OUT |= CS;
}

// Зеркальное отображение на ЖКИ
void Dogs102x6_setMirrorColDisplay() {
    uint8_t cmd[] = {SET_COM_DIRECTION};
    if (MirrorColMode == 1) {
        cmd[0] = SET_COM_DIRECTION + 0x08;
    } else {
        cmd[0] = SET_COM_DIRECTION;
    }
    Dogs102x6_writeCommand(cmd, 1);
}

// Вывод числа на ЖКИ
void printNumber(long int number) {
    int nDigits = log10(fabs(number)) + 1; // Количество цифр
    Dogs102x6_setAddress(nDigits + 2, 77);  // Начало отображения числа
    Dogs102x6_writeData(number > 0 ? symbols[0] : symbols[1], 11);  // Знак числа
    long int divider = pow(10, nDigits - 1); // Делитель для извлечения цифр
    number = fabs(number); // Работа с абсолютным значением числа

    // Цикл для вывода каждой цифры
    for (int j = nDigits - 1; j >= 0; j--) {
        int digit = number / divider; // Извлечение текущей цифры
        Dogs102x6_setAddress(j + 2, 77); // Установка адреса на ЖКИ
        Dogs102x6_writeData(symbols[digit + 2], 11); // Вывод цифры
        number = number % divider; // Уменьшение числа
        divider /= 10; // Обновление делителя
    }
}

// Парсинг данных проекции
long int parseProjectionByte(uint8_t projectionByte) {
    long int projectionValue = 0;
    int isNegative = projectionByte & BIT7;

    for (int i = 0; i < 7; i++) {
        if (isNegative) {
            projectionValue += (BITx[i] & projectionByte) ? 0 : MAPPING_VALUES[i];
        } else {
            projectionValue += (BITx[i] & projectionByte) ? MAPPING_VALUES[i] : 0;
        }
    }
    projectionValue *= isNegative ? -1 : 1;
    return projectionValue;
}

// Расчет угла на основе проекции
int calculateAngleFromProjection(double projection) {
    projection = fmin(fmax(projection / 1000, -1), 1);
    return acos(projection) * 57.3;
}

// Чтение регистра акселерометра
int8_t Cma3000_readRegister(int8_t Address) {
    Address <<= 2;
    P3OUT &= ~BIT5;
    int8_t Result = UCA0RXBUF;
    while (!(UCA0IFG & UCTXIFG));
    UCA0TXBUF = Address;
    while (!(UCA0IFG & UCRXIFG));
    Result = UCA0RXBUF;
    while (!(UCA0IFG & UCTXIFG));
    UCA0TXBUF = 0;
    while (!(UCA0IFG & UCRXIFG));
    Result = UCA0RXBUF;
    while (UCA0STAT & UCBUSY);
    P3OUT |= BIT5;
    return Result;
}

// Настройка кнопок
void setupButtons() {
    P1DIR &= ~BIT7;  // Настройка S1 как вход
    P1REN |= BIT7;   // Включаем подтягивающий резистор
    P1OUT |= BIT7;   // Резистор подтяжки к Vcc
    P1IE |= BIT7;    // Прерывание от S1
    P1IES |= BIT7;   // Прерывание по спаду
    P1IFG &= ~BIT7;  // Сброс флага прерывания
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Остановка Watchdog
    setupButtons();  // Настройка кнопок
    Dogs102x6_init();  // Инициализация ЖКИ
    Dogs102x6_clearScreen();  // Очистка экрана
    __bis_SR_register(LPM0_bits + GIE);  // Включение прерываний
    __no_operation();  // Ожидание
    return 0;
}
