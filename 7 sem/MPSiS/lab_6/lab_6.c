#include <msp430.h> 
#include "HAL_Dogs102x6.h"
#include "HAL_Cma3000.h"
#include "ff.h"
#include "structure.h"
#include "CTS_Layer.h"
#include <stdlib.h>
#include <math.h>

/* Функция для взаимодействия с GPIO */
#define GPIO_DIR_INPUT(...) GPIO_DIR_INPUT_SUB(__VA_ARGS__)
#define GPIO_DIR_INPUT_SUB(port, pin) (P##port##DIR &= ~(1 << (pin)))

#define GPIO_PULLUP(...) GPIO_PULLUP_SUB(__VA_ARGS__)
#define GPIO_PULLUP_SUB(port, pin) P##port##REN |= (1 << (pin)); \
    P##port##OUT |= (1 << (pin))

#define GPIO_READ_PIN(...) GPIO_READ_PIN_SUB(__VA_ARGS__)
#define GPIO_READ_PIN_SUB(port, pin) ((P##port##IN & (1 << (pin))) ? 1 : 0)

#define GPIO_WRITE_PIN(...) GPIO_WRITE_PIN_SUB(__VA_ARGS__)
#define GPIO_WRITE_PIN_SUB(port, pin, value) (P##port##OUT = (P##port##OUT &~(1 << (pin))) | (value << (pin)))

//описание свойств PAD1
const struct Element PAD1 =
{ //CB0
  .inputBits = CBIMSEL_0,
  .maxResponse = 250,
  .threshold = 125
};

//структура описания PAD1 для библиотеки CTS_Layer.h
const struct Sensor Sensor1 =
{
     .halDefinition = RO_COMPB_TA1_TA0,
     .numElements = 1,
     .baseOffset = 0,
     .cbpdBits = 0x0001, //CB0
     .arrayPtr[0] = &PAD1,
     .cboutTAxDirRegister = (uint8_t *)&P1DIR,
     .cboutTAxSelRegister = (uint8_t *)&P1SEL,
     .cboutTAxBits = BIT6, // P1.6

     // информация таймера
     .measGateSource = TIMER_ACLK,
     .sourceScale = TIMER_SOURCE_DIV_0,
     /* 50 ACLK/1 циклов or 50*1/32Khz = 1.5ms */
     .accumulationCycles = 50
};
//кнопка S2
#define S2_PORT 2
#define S2_PIN 2

// данные и позиции для рисования дополнительной информации
#define DRAW_TEXT_ROW 7
#define LINE_Y 45

// имя файла для хранения буфера
#define FILE_NAME "buffer.bin"

//буфер
#define BUFFER_SIZE 90
#define BUFFER_COUNT (BUFFER_SIZE / 2)
volatile uint16_t buffer[BUFFER_COUNT];
volatile uint8_t index = 0;

//отслеживание состояния нажатия кнопок и файла
uint8_t first_press_PAD = 0;
uint8_t no_press_PAD = 0;
uint8_t first_press_S2 = 0;
uint8_t no_press_S2 = 0;
uint8_t file_draw = 0;
UINT bw = 0;

//значение оси Y акселерометра
uint16_t accel_y = 0;

//получаем данные с акселерометра по Y
void Cma3000_readAccel_y()
{
    Cma3000_yAccel = Cma3000_readRegister(DOUTY);
    __delay_cycles(50 * 25);
    Cma3000_zAccel = Cma3000_readRegister(DOUTZ);
    __delay_cycles(50 * 25);
    double radian_value = atan2((double)Cma3000_yAccel, (double)Cma3000_zAccel);
    accel_y = (uint16_t)((int16_t)(radian_value * 180.0 / M_PI) + 180);
}

// вычисления значения для рисования
uint16_t get_draw_value(uint8_t index)
{
    uint16_t draw_value = (uint16_t)((float)DOGS102x6_X_SIZE * (float)buffer[index] / 360);
    return draw_value;
}

void draw()
{
    uint16_t draw_value = get_draw_value(index);
    uint8_t x_pos = index;
    if (x_pos == 45) {
        Dogs102x6_clearRow(x_pos / 8);
    }
    if (x_pos % 8 == 0)
    {
        Dogs102x6_clearRow(x_pos / 8);
    }
    Dogs102x6_pixelDraw(draw_value, x_pos, DOGS102x6_DRAW_NORMAL);
    Dogs102x6_horizontalLineDraw(0, DOGS102x6_X_SIZE - 1, LINE_Y, DOGS102x6_DRAW_NORMAL);

    uint16_t val = buffer[index];
    int f = 0;
    char str[40] = "";
    if (val == 180) {
        val = 0;
        f = 2;
    }
    else if (val <= 180) {
        val = 180 - val;
        f = 1;
    }
    else {
        val = val - 180;
    }
    int8_t i = 1;
    do {
        str[i++] = (char)(val % 10 + '0');
        val = val / 10;
    }
    while (val > 0 && index < 40);

    uint8_t j = 0;
    for (i; i >= j; i--) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        j++;
    }
    switch(f) {
    case 2:
        str[0] = ' ';
        break;
    case 1:
        str[0] = '-';
        break;
    default: str[0] = '+';
    }
    Dogs102x6_clearRow(7);
    Dogs102x6_stringDraw(DRAW_TEXT_ROW, 0, str, DOGS102x6_DRAW_NORMAL);
}

void draw_from_file(FIL file)
{
    no_press_PAD = 1;
    first_press_PAD = 0;
    file_draw ^= 1;
    if (file_draw)
    {
        f_open(&file, FILE_NAME, FA_READ);
        f_read(&file, buffer, BUFFER_SIZE, &bw);
        Dogs102x6_clearScreen();
        Dogs102x6_horizontalLineDraw(0, DOGS102x6_X_SIZE - 1, LINE_Y, DOGS102x6_DRAW_NORMAL);
        f_close(&file);
        uint16_t i = 0;
    for (i = 0; i < BUFFER_COUNT; i++)
    {
        uint16_t draw_value = get_draw_value(i);
        Dogs102x6_pixelDraw(draw_value, i, DOGS102x6_DRAW_NORMAL);
    }
    }
    else {
        Dogs102x6_clearScreen();
        Dogs102x6_horizontalLineDraw(0, DOGS102x6_X_SIZE - 1, LINE_Y, DOGS102x6_DRAW_NORMAL);
    }
}

void write_to_file(FIL file)
{
    if (first_press_S2 == 0)
    first_press_S2 = 1;
    else if(first_press_S2 == 1)
    {
        no_press_S2 = 1;
        first_press_S2 = 0;
        f_open(&file, FILE_NAME, FA_WRITE | FA_CREATE_ALWAYS);
        f_write(&file, buffer, BUFFER_SIZE, &bw);
        f_close(&file);
    }
}

uint16_t main(void)
{
    //остановка сторожевого таймера
    WDTCTL = WDTPW + WDTHOLD;

    //инициализация S2
    GPIO_DIR_INPUT(S2_PORT, S2_PIN);
    GPIO_PULLUP(S2_PORT, S2_PIN);

    //инициализация экрана
    Dogs102x6_init();
    Dogs102x6_backlightInit();
    Dogs102x6_setBacklight(255);
    Dogs102x6_clearScreen();
    Dogs102x6_horizontalLineDraw(0, DOGS102x6_X_SIZE - 1, LINE_Y, DOGS102x6_DRAW_NORMAL);

    // Инициализация базовых значения для сенсорных кнопок
    TI_CAPT_Init_Baseline(&Sensor1);
    TI_CAPT_Update_Baseline(&Sensor1,5);

    //инициализация акселерометра
    Cma3000_init();
    FATFS fs; //файловая система
    FIL file; //файл для буфера

    // монтирование диска
    FRESULT res = f_mount(0, &fs);
    if (res == FR_NO_FILESYSTEM) {
        f_mkfs("0:", 0, 512);
    }

    //инициализация DMA
    DMACTL0 = DMA0TSEL_5; // пересылка по TA2CCR0.IFG
    // одиночная пересылка, включение DMA, разрешение прерываний
    // нет инкремента dst, src, размер данных 16 бит.
    DMA0CTL = DMADT_0+DMAEN+DMAIE;

    // Размер = 1
    DMA0SZ = 1;
    // DMA0SA - источник (ось Y акселерометра)
    __data16_write_addr((unsigned short) &DMA0SA,(unsigned long) &accel_y);
    // DMA0DA - назначение (элемент буфера)
    __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) &buffer[index]);
    TA2CCR0 = 200;
    TA2CTL = TIMER_ACLK + TIMER_SOURCE_DIV_0;
    TA2CTL |= (TACLR + MC__UP);
    while (1)
    {
        //вход в режим LPMO с разрешением прерываний
        __bis_SR_register(LPM0_bits+GIE);
        //рисуем значение на экран
        if (file_draw == 0)
        {
            draw();
        }
        Cma3000_readAccel_y();

//считываем значения буфера из файла по нажатию PAD1 и выводим на экран
        struct Element * keypressed = 0;
        keypressed = (struct Element *)TI_CAPT_Buttons(&Sensor1);
        if (keypressed == 0)
        {
            no_press_PAD = 0;
        }
        if(keypressed && no_press_PAD == 0)
        {
            if (first_press_PAD == 0)
            {
                first_press_PAD = 1;
            }
            else if(first_press_PAD == 1)
            {
                draw_from_file(file);
            }
        }
        //заносим значения буфера в файл по нажатию S2
        uint8_t value_S2 = !GPIO_READ_PIN(S2_PORT, S2_PIN);
        if (value_S2 == 0)
        {
            no_press_S2 = 0;
        }
        if(value_S2 && no_press_S2 == 0)
        {
            write_to_file(file);
        }
        index++;
        if (index == BUFFER_COUNT)
        {
            index = 0;
        }
//обновляем адрес назначения DMA
        __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) &buffer[index]);

//запуск DMA
        DMA0CTL |= DMAEN;
//запуск Таймера 2
        TA2CTL |= (TACLR + MC__UP);
    }
}
#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
{
    switch(__even_in_range(DMAIV,16))
    {
//прерывание DMA0IFG
        case 2:
        TA2CTL &= ~MC__UP; //остановка таймера
        _bic_SR_register_on_exit(LPM0_bits); // выход из LPM0
        break;
    default: break;
    }
}
