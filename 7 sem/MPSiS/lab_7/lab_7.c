#include <msp430.h>
#include "HAL_Dogs102x6.h"
#include <math.h>
#include "HAL_FLASH.h"
#include "structure.h"
#include "CTS_Layer.h"
/* Функция для взаимодействия с GPIO */
#define GPIO_DIR_OUTPUT(...) GPIO_DIR_OUTPUT_SUB(__VA_ARGS__)
#define GPIO_DIR_OUTPUT_SUB(port, pin) (P##port##DIR |= (1 << (pin)))
#define GPIO_DIR_INPUT(...) GPIO_DIR_INPUT_SUB(__VA_ARGS__)
#define GPIO_DIR_INPUT_SUB(port, pin) (P##port##DIR &= ~(1 << (pin)))
#define GPIO_PULLUP(...) GPIO_PULLUP_SUB(__VA_ARGS__)
#define GPIO_PULLUP_SUB(port, pin) P##port##REN |= (1 << (pin)); \
                                   P##port##OUT |= (1 << (pin))
#define GPIO_PULLDOWN(...) GPIO_PULLDOWN_SUB(__VA_ARGS__)
#define GPIO_PULLDOWN_SUB(port, pin) P##port##REN |= (1 << (pin)); \
                                     P##port##OUT &= ~(1 << (pin))
#define GPIO_NOPULL(...) GPIO_NOPULL_SUB(__VA_ARGS__)
#define GPIO_NOPULL_SUB(port, pin) (P##port##REN &= ~(1 << (pin)))
#define GPIO_READ_PIN(...) GPIO_READ_PIN_SUB(__VA_ARGS__)
#define GPIO_READ_PIN_SUB(port, pin) ((P##port##IN & (1 << (pin))) ? 1 : 0)
#define GPIO_WRITE_PIN(...) GPIO_WRITE_PIN_SUB(__VA_ARGS__)
#define GPIO_WRITE_PIN_SUB(port, pin, value) (P##port##OUT = (P##port##OUT & ~(1 << (pin))) | (value << (pin)))
#define GPIO_TOGGLE_PIN(...) GPIO_TOGGLE_PIN_SUB(__VA_ARGS__)
#define GPIO_TOGGLE_PIN_SUB(port, pin) (P##port##OUT ^= (1 << (pin)))
#define GPIO_TRIG_EDGE_FALLING(...) GPIO_TRIG_EDGE_FALLING_SUB(__VA_ARGS__)
#define GPIO_TRIG_EDGE_FALLING_SUB(port, pin) (P##port##IES |= (1 << (pin)))
#define GPIO_TRIG_EDGE_RISING(...) GPIO_TRIG_EDGE_RISING_SUB(__VA_ARGS__)
#define GPIO_TRIG_EDGE_RISING_SUB(port, pin) (P##port##IES &= ~(1 << (pin)))
#define GPIO_INTERRUPT_ENABLE(...) GPIO_INTERRUPT_ENABLE_SUB(__VA_ARGS__)
#define GPIO_INTERRUPT_ENABLE_SUB(port, pin) P##port##IFG &= ~(1 << (pin)); \
 P##port##IE |= (1 << (pin))
#define GPIO_INTERRUPT_DISABLE(...) GPIO_INTERRUPT_DISABLE_SUB(__VA_ARGS__)
#define GPIO_INTERRUPT_DISABLE_SUB(port, pin) (P##port##IE &= ~(1 << (pin)))
#define GPIO_PERIPHERAL(...) GPIO_PERIPHERAL_SUB(__VA_ARGS__)
#define GPIO_PERIPHERAL_SUB(port, pin) (P##port##SEL |= (1 << (pin)))
#define GPIO_CLEAR_IT_FLAG(...) GPIO_CLEAR_IT_FLAG_SUB(__VA_ARGS__)
#define GPIO_CLEAR_IT_FLAG_SUB(port, pin) (P##port##IFG &= ~(1 << (pin)))
 /********************************************************************/
 // Состояние калькулятора
typedef enum
{
     RESULT = 0,
     CALCULATE,
     ERROR
} CALC_STATE;
 // Направление перемещения акселерометра
typedef enum
{
     NONE = 0,
     LEFT,
     RIGHT,
     UP,
     DOWN
} MENU_DIR;
typedef enum
{
    NOT_PRESSED = 0,
    PRESSED = 1
} PAD_STATE;
 // Описание свойств PAD1
const struct Element PAD1 = { //CB0
  .inputBits = CBIMSEL_0,
  .maxResponse = 250,
  .threshold = 125 };
 //описание свойств PAD2
const struct Element PAD2 = { //CB0
        .inputBits = CBIMSEL_1,
        .maxResponse = 250,
        .threshold = 125 };
 //pad4
const struct Element PAD4 = { //CB3
        .inputBits = CBIMSEL_3,
        .maxResponse = 250,
        .threshold = 125 };
 // Описание свойств PAD5
const struct Element PAD5 = { //CB4
        .inputBits = CBIMSEL_4,
        .maxResponse = 1900,
        .threshold = 475 };
 // Cтруктура описания PAD1 для библиотеки CTS_Layer.h
const struct Sensor Sensor1 = { .halDefinition = RO_COMPB_TA1_TA0,
                                .numElements = 1,
                                .baseOffset = 0,
                                .cbpdBits = 0x0001, //CB0
                                .arrayPtr[0] = &PAD1,
                                .cboutTAxDirRegister =(uint8_t*) &P1DIR,
                                .cboutTAxSelRegister = (uint8_t*) &P1SEL,
                                .cboutTAxBits = BIT6, // P1.6
                                // Информация таймера
                                .measGateSource = TIMER_ACLK,
                                 .sourceScale =TIMER_SOURCE_DIV_0,
                                 /* 50 ACLK/1 циклов or 50*1/32Khz = 1.5ms */
                                 .accumulationCycles = 50 };
// Cтруктура описания PAD2 для библиотеки CTS_Layer.h
const struct Sensor Sensor2 = { .halDefinition = RO_COMPB_TA1_TA0,
                                 .numElements = 1,
                                 .baseOffset = 1,
                                 .cbpdBits = 0x0001, //CB1 ????
                                 .arrayPtr[0] = &PAD2,
                                 .cboutTAxDirRegister = (uint8_t*) &P1DIR,
                                 .cboutTAxSelRegister = (uint8_t*) &P1SEL,
                                 .cboutTAxBits = BIT6, // P1.6
                                 // информация таймера
                                 .measGateSource = TIMER_ACLK,
                                 .sourceScale = TIMER_SOURCE_DIV_0,
                                 /* 50 ACLK/1 циклов or 50*1/32Khz = 1.5ms */
                                 .accumulationCycles = 50 };
// Cтруктура описания PAD5 для библиотеки CTS_Layer.h
const struct Sensor Sensor4 = { .halDefinition = RO_COMPB_TA1_TA0,
                                 .numElements = 1,
                                 .baseOffset = 3,
                                 .cbpdBits = 0x0010, //CB4
                                 .arrayPtr[0] = &PAD4,
                                 .cboutTAxDirRegister = (uint8_t*) &P1DIR,
                                 .cboutTAxSelRegister = (uint8_t*) &P1SEL,
                                 .cboutTAxBits = BIT6, // P1.6
                                 // Информация таймера
                                 .measGateSource = TIMER_ACLK,
                                 .sourceScale = TIMER_SOURCE_DIV_0,
                                 /* 50 ACLK/1 циклов or 272*1/32Khz = 8.5ms */
                                 .accumulationCycles = 50 };
// Cтруктура описания PAD5 для библиотеки CTS_Layer.h
const struct Sensor Sensor5 = { .halDefinition = RO_COMPB_TA1_TA0,
                                 .numElements = 1,
                                 .baseOffset = 4,
                                 .cbpdBits = 0x0010, //CB4
                                 .arrayPtr[0] = &PAD5,
                                 .cboutTAxDirRegister = (uint8_t*) &P1DIR,
                                 .cboutTAxSelRegister = (uint8_t*) &P1SEL,
                                 .cboutTAxBits = BIT6, // P1.6
                                 // Информация таймера
                                 .measGateSource = TIMER_ACLK,
                                 .sourceScale = TIMER_SOURCE_DIV_0,
                                 /* 50 ACLK/1 циклов or 272*1/32Khz = 8.5ms */
                                 .accumulationCycles = 370 };
void initPads()
{ // Инициализация базовых значения для сенсорных кнопок
  TI_CAPT_Init_Baseline(&Sensor1);
  TI_CAPT_Update_Baseline(&Sensor1, 5);
  TI_CAPT_Init_Baseline(&Sensor2);
  TI_CAPT_Update_Baseline(&Sensor2, 5);
  TI_CAPT_Init_Baseline(&Sensor4);
  TI_CAPT_Update_Baseline(&Sensor4, 5);
  TI_CAPT_Init_Baseline(&Sensor5);
  TI_CAPT_Update_Baseline(&Sensor5, 5);}
PAD_STATE pollPad1()
{
    static PAD_STATE state = NOT_PRESSED;
    static struct Element *padInfo;
    static uint8_t wasHold = 0;
    padInfo = (struct Element*) TI_CAPT_Buttons(&Sensor1);
    if (padInfo == 0)
    {
        wasHold = 0;
    }
    if (padInfo && !wasHold)
    {
        state = PRESSED;
        wasHold = 1; // ??
    }
    else
    {
        state = NOT_PRESSED;
    }
    return state;
}
PAD_STATE pollPad2()
{
    static PAD_STATE state = NOT_PRESSED;
    static struct Element *padInfo;
    static uint8_t wasHold = 0;
    padInfo = (struct Element*) TI_CAPT_Buttons(&Sensor2);
    if (padInfo == 0)
    {
        wasHold = 0;
    }
    if (padInfo && !wasHold)
    {
        state = PRESSED;
        wasHold = 1; // ??
    }
    else
    {
        state = NOT_PRESSED;
    }
    return state;
}
PAD_STATE pollPad5()
{
    static PAD_STATE state = NOT_PRESSED;
    static struct Element *padInfo;
    static uint8_t wasHold = 0;
    padInfo = (struct Element*) TI_CAPT_Buttons(&Sensor5);
    if (padInfo == 0)
    {
        wasHold = 0;
    }
    if (padInfo && !wasHold)
     {
     state = PRESSED;
     wasHold = 1; // ??
     }
     else
     {
     state = NOT_PRESSED;
     }
     return state;
}
PAD_STATE pollPad4()
{
     static PAD_STATE state = NOT_PRESSED;
     static struct Element *padInfo;
     static uint8_t wasHold = 0;
     padInfo = (struct Element*) TI_CAPT_Buttons(&Sensor4);
     if (padInfo == 0)
     {
         wasHold = 0;
     }
     if (padInfo && !wasHold)
     {
         state = PRESSED;
         wasHold = 1; // ??
     }
     else
     {
         state = NOT_PRESSED;
     }
     return state;
}
/********************************************************************/
// TIMER1_PERIOD_1MS = ACLK * 0.001с (ACLK = 32768 Гц)
#define TIMER1_PERIOD_1MS 33
// Определения позиций элементов интерфейса
#define MENU_ROWS 3
#define MENU_COLUMNS 4
#define CHARACTER_WIDTH 6
#define BOX_X 7
#define BOX_X_BETWEEN 3
#define BOX_Y 11
#define CHAR_LEFT_ALIGN 1
#define TOP_HORIZONTAL_LINE0 (BOTTOM_HORIZONTAL_LINE0 - BOX_Y - 1)
#define BOTTOM_HORIZONTAL_LINE0 (BOTTOM_HORIZONTAL_LINE1 - 16)
#define TOP_HORIZONTAL_LINE1 (BOTTOM_HORIZONTAL_LINE1 - BOX_Y - 1)
#define BOTTOM_HORIZONTAL_LINE1 (BOTTOM_HORIZONTAL_LINE2 - 16)
#define TOP_HORIZONTAL_LINE2 (BOTTOM_HORIZONTAL_LINE2 - BOX_Y - 1)
#define BOTTOM_HORIZONTAL_LINE2 57
#define LEFT_VERTICAL_LINE0 (RIGHT_VERTICAL_LINE0 - BOX_X - 1)
#define RIGHT_VERTICAL_LINE0 (LEFT_VERTICAL_LINE1 - BOX_X_BETWEEN - 1)
#define LEFT_VERTICAL_LINE1 (RIGHT_VERTICAL_LINE1 - BOX_X - 1)
#define RIGHT_VERTICAL_LINE1 (LEFT_VERTICAL_LINE2 - BOX_X_BETWEEN - 1)
#define LEFT_VERTICAL_LINE2 (RIGHT_VERTICAL_LINE2 - BOX_X - 1)
#define RIGHT_VERTICAL_LINE2 (LEFT_VERTICAL_LINE3 - BOX_X_BETWEEN - 1)
#define LEFT_VERTICAL_LINE3 (RIGHT_VERTICAL_LINE3 - BOX_X - 1)
#define RIGHT_VERTICAL_LINE3 100
#define MS_BOX_X 13
#define LEFT_VERTICAL_LINE_MS (RIGHT_VERTICAL_LINE_MS - 1 - MS_BOX_X)
#define RIGHT_VERTICAL_LINE_MS (LEFT_VERTICAL_LINE_MR - BOX_X_BETWEEN - 1)
#define LEFT_VERTICAL_LINE_MR (RIGHT_VERTICAL_LINE_MR - 1 - MS_BOX_X)
#define RIGHT_VERTICAL_LINE_MR RIGHT_VERTICAL_LINE1
#define DATA_ROW 0
#define CHAR_POS_ROW0 2
#define CHAR_POS_ROW1 4
#define CHAR_POS_ROW2 6
#define CHAR_POS_COLUMN0 (LEFT_VERTICAL_LINE0 + CHAR_LEFT_ALIGN + 1)
#define CHAR_POS_COLUMN1 (LEFT_VERTICAL_LINE1 + CHAR_LEFT_ALIGN + 1)
#define CHAR_POS_COLUMN2 (LEFT_VERTICAL_LINE2 + CHAR_LEFT_ALIGN + 1)
#define CHAR_POS_COLUMN3 (LEFT_VERTICAL_LINE3 + CHAR_LEFT_ALIGN + 1)
#define CHAR_POS_MS (LEFT_VERTICAL_LINE_MS + CHAR_LEFT_ALIGN + 1)
#define CHAR_POS_MR (LEFT_VERTICAL_LINE_MR + CHAR_LEFT_ALIGN + 1)
// Порт и пин S1
#define BUTT1_PORT 1
#define BUTT1_PIN 7
// Адрес для хранения сохраняемых значений
#define INFO_FLASH_B_ADDR 0x1900
//#define INFO_FLASH_A_ADDR 0x1980
#define RADIAN_30_DEGREES 0.524
void Delay(uint16_t time_ms);
void Dogs102x6_BoxDraw(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t
style);
// Функции для каждой кнопки
void Press_EQ(void);
void Press_MS(void);
void Press_MR(void);
void Press_MULT(void);
void Press_DIV(void);
void Press_PLUS(void);
void Press_MINUS(void);
void Press_0(void);
void Press_1(void);
void Press_2(void);
void Press_EQ(void);
void Press_C(void);
///////////////////////////////
MENU_DIR Process_Menu_Move(void);
void Draw_Flash_Value(void);
void Draw_Data_Value(void);
// Результат вычислений
int16_t result = 0;
int16_t left_operand = 0;
int16_t right_operand = 0;
// Тип операции
uint8_t operation = 0;
// Хранение времени ожидания для функции Delay
volatile uint16_t delay_ms = 0;
// Переменная состояний калькулятора
CALC_STATE calc_state = RESULT;
// Позиция выбранного кнопки
uint8_t x = 0;
uint8_t y = 0;
// Указатели на функции для каждой кнопки
void (*functions[MENU_ROWS][MENU_COLUMNS]) (void) =
{
 {Press_MS, Press_MR, Press_MULT, Press_DIV},
 {Press_2, 0, Press_PLUS, Press_MINUS},
 {Press_1, Press_0, Press_EQ, Press_C}
};
uint8_t positions[MENU_ROWS][MENU_COLUMNS][4] =
{
 {
 {LEFT_VERTICAL_LINE_MS, TOP_HORIZONTAL_LINE0, RIGHT_VERTICAL_LINE_MS, BOTTOM_HORIZONTAL_LINE0},
 {LEFT_VERTICAL_LINE_MR, TOP_HORIZONTAL_LINE0, RIGHT_VERTICAL_LINE_MR, BOTTOM_HORIZONTAL_LINE0},
 {LEFT_VERTICAL_LINE2, TOP_HORIZONTAL_LINE0, RIGHT_VERTICAL_LINE2, BOTTOM_HORIZONTAL_LINE0},
 {LEFT_VERTICAL_LINE3, TOP_HORIZONTAL_LINE0, RIGHT_VERTICAL_LINE3, BOTTOM_HORIZONTAL_LINE0},
 },
 {
 {LEFT_VERTICAL_LINE0, TOP_HORIZONTAL_LINE1, RIGHT_VERTICAL_LINE0, BOTTOM_HORIZONTAL_LINE1},
 {0, 63, 0, 63},
 {LEFT_VERTICAL_LINE2, TOP_HORIZONTAL_LINE1, RIGHT_VERTICAL_LINE2, BOTTOM_HORIZONTAL_LINE1},
 {LEFT_VERTICAL_LINE3, TOP_HORIZONTAL_LINE1, RIGHT_VERTICAL_LINE3, BOTTOM_HORIZONTAL_LINE1},
 },
 {
 {LEFT_VERTICAL_LINE0, TOP_HORIZONTAL_LINE2, RIGHT_VERTICAL_LINE0, BOTTOM_HORIZONTAL_LINE2},
 {LEFT_VERTICAL_LINE1, TOP_HORIZONTAL_LINE2, RIGHT_VERTICAL_LINE1, BOTTOM_HORIZONTAL_LINE2},
 {LEFT_VERTICAL_LINE2, TOP_HORIZONTAL_LINE2, RIGHT_VERTICAL_LINE2, BOTTOM_HORIZONTAL_LINE2},
 {LEFT_VERTICAL_LINE3, TOP_HORIZONTAL_LINE2, RIGHT_VERTICAL_LINE3, BOTTOM_HORIZONTAL_LINE2},
 },
};
// младший разряд (полубайт)
uint8_t to_hex_lb(uint16_t tmp)
{
    uint8_t smb=0;
    smb = (tmp % 10) +'0';
    return smb;}
// старший разряд (полубайт)
uint8_t to_hex_hb(uint16_t tmp)
{
    uint8_t smb=0;
    smb = ((tmp / 10)% 10) + '0';
    return smb;
}
// старший разряд (полубайт)
uint8_t to_hex_hhb(uint16_t tmp)
{
    uint8_t smb=0;
    smb = ((tmp / 100)% 10) + '0';
    return smb;
}
void Press_MS(void)
{
    if (calc_state == RESULT)
    {
        Flash_SegmentErase((uint8_t*) (INFO_FLASH_B_ADDR));
        Flash_SegmentErase((uint8_t*) (INFO_FLASH_B_ADDR+8));
        // int x=0;
        // FlashWrite_16((uint16_t*) &x, (uint16_t*) (INFO_FLASH_B_ADDR), 1);
        FlashWrite_16((uint16_t*) &result, (uint16_t*) (INFO_FLASH_B_ADDR), 1);
    }
}
void Press_MR(void)
{
    if (calc_state == RESULT)
    {
        result = *(int16_t*) (INFO_FLASH_B_ADDR);
    }
    else if (calc_state == CALCULATE)
    {
        right_operand = *(int16_t*) (INFO_FLASH_B_ADDR);
    }
}
void Press_MULT(void)
{
    Press_EQ();
    calc_state = CALCULATE;
    left_operand = result;
    right_operand = 0;
    operation = 'x';
}
void Press_DIV(void)
{
    Press_EQ();
    calc_state = CALCULATE;
    left_operand = result;
    right_operand = 0;
    operation = '/';
}
void Press_PLUS(void)
{
    Press_EQ();
    calc_state = CALCULATE;
    left_operand = result;
    right_operand = 0;
    operation = '+';
}
void Press_MINUS(void)
{
    Press_EQ();
    calc_state = CALCULATE;
    left_operand = result;
    right_operand = 0;
    operation = '-';
}
void Press_2(void)
{
    uint8_t sign = 0;
    if (calc_state == RESULT)
    {
        if (result < 0)
        {
            sign = 1;
            result = -result;
        }
        int16_t tmp=0,old_tmp=0;
        old_tmp=(result / 100)% 10;
        tmp=(result / 100)% 10;
        tmp++;
        if (tmp >= 10) tmp=0;
        tmp=tmp-old_tmp;
        result+=tmp*100;
        if (sign)
        {
            result = -result;
        }
    }
    else if (calc_state == CALCULATE)
    {
        if (right_operand < 0)
        {
            sign = 1;
            right_operand = -right_operand;
        }
        int16_t tmp=0,old_tmp=0;
        old_tmp=(right_operand / 100)% 10;
        tmp=(right_operand / 100)% 10;
        tmp++;
        if (tmp >= 10) tmp=0;
             tmp=tmp-old_tmp;
             right_operand+=tmp*100;
        if (sign)
        {
            right_operand = -right_operand;
        }
     }
}
void Press_1(void)
{
     uint8_t sign = 0;
     if (calc_state == RESULT)
     {
         if (result < 0)
         {
             sign = 1;
             result = -result;
         }
         int16_t tmp=0,old_tmp=0;
         old_tmp=(result / 10)% 10;
         tmp=(result / 10)% 10;
         tmp++;
         if (tmp >= 10) tmp=0;
         tmp=tmp-old_tmp;
         result+=tmp*10;
         if (sign)
         {
         result = -result;
         }
     }
     else if (calc_state == CALCULATE)
     {
         if (right_operand < 0)
         {
             sign = 1;
             right_operand = -right_operand;
         }
         int16_t tmp=0,old_tmp=0;
         old_tmp=(right_operand / 10)% 10;
         tmp=(right_operand / 10)% 10;
         tmp++;
         if (tmp >= 10) tmp=0;
         tmp=tmp-old_tmp;
         right_operand+=tmp*10;
         if (sign)
         {
         right_operand = -right_operand;
         }
     }
}
void Press_0(void)
{
    uint8_t sign = 0;
    if (calc_state == RESULT)
    {
        if (result < 0)
        {
            sign = 1;
            result = -result;
        }
        int16_t tmp=0,old_tmp=0;
        old_tmp=result % 10;
        tmp=result % 10;
        tmp++;
        if (tmp >= 10) tmp=0;
        tmp=tmp-old_tmp;
        result+=tmp;
        if (sign)
        {
            result = -result;
        }
    }
    else if (calc_state == CALCULATE)
    {
        if (right_operand < 0)
        {
            sign = 1;
            right_operand = -right_operand;
        }
        int16_t tmp=0,old_tmp=0;
        old_tmp=right_operand % 10;
        tmp=right_operand % 10;
        tmp++;
        if (tmp >= 10) tmp=0;
        tmp=tmp-old_tmp;
        right_operand+=tmp;
        if (sign)
        {
            right_operand = -right_operand;
        }
    }
}
void Press_EQ(void)
{
    if (calc_state == CALCULATE)
    {
        left_operand = left_operand > 0 ? left_operand & 0x0fff : -((-left_operand) & 0x0fff);
        right_operand = right_operand > 0 ? right_operand & 0x0fff : -((-right_operand) & 0x0fff);
        switch(operation)
        {
        case '+':
            result = left_operand + right_operand;
            break;
        case '-':
            result = left_operand - right_operand;
            break;
        case '/':
            if (right_operand == 0)
            {
                calc_state = ERROR;
                result = 0;
                left_operand = 0;
                right_operand = 0;
                return;
            }
            result = left_operand / right_operand;
            break;
        case 'x':
            // Знаковое шестнадцатибитное умножение
            MPYS = (int16_t)left_operand;
            OP2 = (int16_t)right_operand;
            result = (int16_t)RESLO;
            break;
        }
        operation = 0;
        calc_state = RESULT;
    }
}
void Press_C(void)
{
    calc_state = RESULT;
    operation = 0;
    left_operand = 0;
    right_operand = 0;
    result = 0;
}
MENU_DIR Process_Menu_Move(void)
{
    static MENU_DIR prev_dir = NONE;
    MENU_DIR current_dir = NONE;
    PAD_STATE state;
    state = pollPad1();
    if (state)
    {
        current_dir = UP;
    }
    else
    {
        state = pollPad2();
        if (state)
        {
            current_dir = LEFT;
        }
            else
            {
                state = pollPad4();
                if (state)
                {
                    current_dir = RIGHT;
                }
            }
        // }
     }
     if (prev_dir != NONE)
     {
         prev_dir = current_dir;
         current_dir = NONE;
     }
     else
     {
         prev_dir = current_dir;
     }
     return current_dir;
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    initPads();
    // Инициализация экрана
    Dogs102x6_init();
    Dogs102x6_backlightInit();
    Dogs102x6_setBacklight(255);
    Dogs102x6_clearScreen();
    // Рисование интерфейса
    Dogs102x6_horizontalLineDraw(0, DOGS102x6_X_SIZE - 1, 9, DOGS102x6_DRAW_NORMAL);
    for (x = 0; x < MENU_ROWS; x++)
    {
        for (y = 0; y < MENU_COLUMNS; y++)
        {
            Dogs102x6_BoxDraw(positions[x][y][0], positions[x][y][1], positions[x][y][2], positions[x][y][3], DOGS102x6_DRAW_NORMAL);
        }
    }
    Dogs102x6_charDraw(CHAR_POS_ROW0, CHAR_POS_COLUMN3, '/', DOGS102x6_DRAW_NORMAL);
    Dogs102x6_charDraw(CHAR_POS_ROW1, CHAR_POS_COLUMN3, '-', DOGS102x6_DRAW_NORMAL);
    Dogs102x6_charDraw(CHAR_POS_ROW2, CHAR_POS_COLUMN3, 'C', DOGS102x6_DRAW_NORMAL);
    Dogs102x6_charDraw(CHAR_POS_ROW0, CHAR_POS_COLUMN2, 'x', DOGS102x6_DRAW_NORMAL);
    Dogs102x6_charDraw(CHAR_POS_ROW1, CHAR_POS_COLUMN2, '+', DOGS102x6_DRAW_NORMAL);
    Dogs102x6_charDraw(CHAR_POS_ROW2, CHAR_POS_COLUMN2, '=', DOGS102x6_DRAW_NORMAL);
    Dogs102x6_charDraw(CHAR_POS_ROW2, CHAR_POS_COLUMN1, '0', DOGS102x6_DRAW_NORMAL);
    Dogs102x6_charDraw(CHAR_POS_ROW2, CHAR_POS_COLUMN0, '1', DOGS102x6_DRAW_NORMAL);
    Dogs102x6_charDraw(CHAR_POS_ROW1, CHAR_POS_COLUMN0, '2', DOGS102x6_DRAW_NORMAL);
    Dogs102x6_stringDraw(CHAR_POS_ROW0, CHAR_POS_MR, "MR", DOGS102x6_DRAW_NORMAL);
    Dogs102x6_stringDraw(CHAR_POS_ROW0, CHAR_POS_MS, "MS", DOGS102x6_DRAW_NORMAL);
    Draw_Flash_Value();
    Draw_Data_Value();
    GPIO_DIR_INPUT(BUTT1_PORT, BUTT1_PIN);
    GPIO_PULLUP(BUTT1_PORT, BUTT1_PIN);
    // TA2 используется для мигания кнопок
    // Тактируется от ACLK/8, режим счета вверх, очищаем регистр TA0R,
    TA2CTL = TASSEL__ACLK | MC__UP | TACLR | ID__8;
    // Разрешаем прерывания TA1.0
    TA2CCTL0 = CCIE;
    // Устанавливаем период
    TA2CCR0 = 640 - 1;
    uint8_t i = 0;
    x = 0;
    y = 0;
    uint8_t first_press_S1 = 0;
    uint8_t no_press_S1 = 0;
    while(1)
    {
        // Обрабатываем изменения положения акселерометра
        MENU_DIR dir = Process_Menu_Move();
        switch(dir)
        {
        case UP:
            x = (x + MENU_ROWS - 1 ) % MENU_ROWS;
            break;
        case DOWN:
            x = (x + 1) % MENU_ROWS;
            break;
        case LEFT:
            y = (y + MENU_COLUMNS - 1) % MENU_COLUMNS;
            break;
        case RIGHT:
            y = (y + 1) % MENU_COLUMNS;
            break;
        }
        if (x == 1 && y == 1)
        {
            if (dir == UP)
            {
                x--;
            }
            else if (dir == DOWN)
            {
                x++;
            }
            else if (dir == LEFT)
            {
                y = 0;
            }
            else if (dir == RIGHT)
            {
                y = 2;
            }
        }
        uint8_t value_S1 = !GPIO_READ_PIN(BUTT1_PORT, BUTT1_PIN);
        if (value_S1 == 0)
        {
            no_press_S1 = 0;
        }
        if(value_S1 && no_press_S1 == 0)
        {
            if (first_press_S1 == 0)
            {
                first_press_S1 = 1;
            }
            else if(first_press_S1 == 1)
            {
                no_press_S1 = 1;
                first_press_S1 = 0;
                functions[x][y]();
                Draw_Data_Value();
                Draw_Flash_Value();
            }
        }
        Delay(1);
    }
    return 0;
}
void Dogs102x6_BoxDraw(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t style)
{
    Dogs102x6_horizontalLineDraw(x1, x2, y1, style);
    Dogs102x6_horizontalLineDraw(x1, x2, y2, style);
    Dogs102x6_verticalLineDraw(y1, y2, x1, style);
    Dogs102x6_verticalLineDraw(y1, y2, x2, style);
}
void Draw_Flash_Value(void)
{
    // Отображение содержимого флеш-памяти
    int16_t temp_val = *(int16_t*)(INFO_FLASH_B_ADDR);
    uint8_t draw_x = 0;
    Dogs102x6_charDraw(2, 12, ' ', DOGS102x6_DRAW_NORMAL);
    if (temp_val < 0)
    {
        Dogs102x6_charDraw(CHAR_POS_ROW0, 0, '-', DOGS102x6_DRAW_NORMAL);
        temp_val = -temp_val;
        draw_x += CHARACTER_WIDTH;
    }
    // Dogs102x6_charDraw(CHAR_POS_ROW0, draw_x, '0' + ((temp_val >> 1) & 0x01), DOGS102x6_DRAW_NORMAL);
        // draw_x += CHARACTER_WIDTH;
        // Dogs102x6_charDraw(CHAR_POS_ROW0, draw_x, '0' + (temp_val & 0x01), DOGS102x6_DRAW_NORMAL);
    Dogs102x6_charDraw(CHAR_POS_ROW0, draw_x, to_hex_hhb(temp_val), DOGS102x6_DRAW_NORMAL);
    draw_x += CHARACTER_WIDTH;
    Dogs102x6_charDraw(CHAR_POS_ROW0, draw_x, to_hex_hb(temp_val), DOGS102x6_DRAW_NORMAL);
    draw_x += CHARACTER_WIDTH;
    Dogs102x6_charDraw(CHAR_POS_ROW0, draw_x, to_hex_lb(temp_val), DOGS102x6_DRAW_NORMAL);
    draw_x += CHARACTER_WIDTH;
    Dogs102x6_charDraw(CHAR_POS_ROW0, draw_x, ' ', DOGS102x6_DRAW_NORMAL);}
void Draw_Data_Value(void)
{
    // Рисование чисел на экране
    Dogs102x6_clearRow(0);
    uint8_t c_x = 0;
    Dogs102x6_charDraw(0, c_x, 'F', DOGS102x6_DRAW_NORMAL);
    c_x += CHARACTER_WIDTH;
    Dogs102x6_charDraw(0, c_x, 'L', DOGS102x6_DRAW_NORMAL);
    c_x += CHARACTER_WIDTH;
    Dogs102x6_charDraw(0, c_x, 'A', DOGS102x6_DRAW_NORMAL);
    c_x += CHARACTER_WIDTH;
    Dogs102x6_charDraw(0, c_x, 'S', DOGS102x6_DRAW_NORMAL);
    c_x += CHARACTER_WIDTH;
    Dogs102x6_charDraw(0, c_x, 'H', DOGS102x6_DRAW_NORMAL);
    Dogs102x6_verticalLineDraw(0, 63, 33, DOGS102x6_DRAW_NORMAL);
    if (calc_state == RESULT)
    {
        // Выводим результат
        uint8_t draw_x = DOGS102x6_X_SIZE - CHARACTER_WIDTH;
        int16_t temp_val = result;
        if (result < 0)
        {
            temp_val = -temp_val;
            Dogs102x6_charDraw(DATA_ROW, draw_x, '-', DOGS102x6_DRAW_NORMAL);
        }
        Dogs102x6_charDraw(DATA_ROW, draw_x, to_hex_lb(temp_val), DOGS102x6_DRAW_NORMAL);
        draw_x -= CHARACTER_WIDTH;
        Dogs102x6_charDraw(DATA_ROW, draw_x, to_hex_hb(temp_val), DOGS102x6_DRAW_NORMAL);
        draw_x -= CHARACTER_WIDTH;
        Dogs102x6_charDraw(DATA_ROW, draw_x, to_hex_hhb(temp_val), DOGS102x6_DRAW_NORMAL);
        draw_x -= CHARACTER_WIDTH;
        if (result < 0)
        {
            Dogs102x6_charDraw(DATA_ROW, draw_x, '-', DOGS102x6_DRAW_NORMAL);
        }
    }
    else if (calc_state == CALCULATE)
    {
        // Выводим выполняемую операцию
        uint8_t draw_x = DOGS102x6_X_SIZE - CHARACTER_WIDTH;
        int16_t temp_val = right_operand;
        if (right_operand < 0)
        {
            temp_val = -temp_val;
            Dogs102x6_charDraw(DATA_ROW, draw_x, '-', DOGS102x6_DRAW_NORMAL);
        }
        Dogs102x6_charDraw(DATA_ROW, draw_x, to_hex_lb(temp_val), DOGS102x6_DRAW_NORMAL);
        draw_x -= CHARACTER_WIDTH;
        Dogs102x6_charDraw(DATA_ROW, draw_x, to_hex_hb(temp_val), DOGS102x6_DRAW_NORMAL);
        draw_x -= CHARACTER_WIDTH;
        Dogs102x6_charDraw(DATA_ROW, draw_x, to_hex_hhb(temp_val),
        DOGS102x6_DRAW_NORMAL);
        draw_x -= CHARACTER_WIDTH;
        if (right_operand < 0)
        {
            Dogs102x6_charDraw(DATA_ROW, draw_x, '-', DOGS102x6_DRAW_NORMAL);
            draw_x -= CHARACTER_WIDTH;
        }
        Dogs102x6_charDraw(DATA_ROW, draw_x, operation, DOGS102x6_DRAW_NORMAL);
        draw_x -= CHARACTER_WIDTH;
        temp_val = left_operand;
        if (left_operand < 0)
        {
             temp_val = -temp_val;
        }
        Dogs102x6_charDraw(DATA_ROW,draw_x, to_hex_lb(temp_val), DOGS102x6_DRAW_NORMAL);
        draw_x -= CHARACTER_WIDTH;
        Dogs102x6_charDraw(DATA_ROW, draw_x, to_hex_hb(temp_val), DOGS102x6_DRAW_NORMAL);
        draw_x -= CHARACTER_WIDTH;
        Dogs102x6_charDraw(DATA_ROW, draw_x, to_hex_hhb(temp_val), DOGS102x6_DRAW_NORMAL);
        draw_x -= CHARACTER_WIDTH;
        if (left_operand < 0)
        {
            Dogs102x6_charDraw(DATA_ROW, draw_x, '-', DOGS102x6_DRAW_NORMAL);
            draw_x -= CHARACTER_WIDTH;
        }
    }
    else if (calc_state == ERROR)
    {
        Dogs102x6_charDraw(DATA_ROW, 84, 'E', DOGS102x6_DRAW_NORMAL);
        Dogs102x6_charDraw(DATA_ROW, 90, 'R', DOGS102x6_DRAW_NORMAL);
        Dogs102x6_charDraw(DATA_ROW, 96, 'R', DOGS102x6_DRAW_NORMAL);
    }
}
// Прерывание для мигания выбранной кнопки
#pragma vector=TIMER2_A0_VECTOR
__interrupt void TIMER2_A0_ISR(void)
{
    static uint8_t style = 0;
    static uint8_t prev_x = 0;
    static uint8_t prev_y = 0;
    if(prev_x != x || prev_y != y)
    {
        Dogs102x6_BoxDraw(positions[prev_x][prev_y][0], positions[prev_x][prev_y][1], positions[prev_x][prev_y][2], positions[prev_x][prev_y][3], DOGS102x6_DRAW_NORMAL);
    }
    Dogs102x6_BoxDraw(positions[x][y][0], positions[x][y][1], positions[x][y][2], positions[x][y][3], style);
    style ^= 1;
    prev_x = x;
    prev_y = y;
}
// Прерывания таймера для задержки
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    if (delay_ms == 0)
    {
        // Выход из LPM0 по завершении прерывания
        _bic_SR_register_on_exit(LPM0_bits + GIE);
        TA1CCTL0 &= ~CCIE;
    }
    else
    {
        delay_ms--;
    }
    // Сбрасываем флаг прерывания
    TA1CCTL0 &= ~CCIFG;
}
void Delay(uint16_t time_ms)
{
    delay_ms = time_ms;
    // Таймер для ожидания
    TA1CCTL0 = CCIE;
    TA1CCR0 = TIMER1_PERIOD_1MS - 1;
    TA1CTL = TASSEL__ACLK + MC__UP + ID__1 + TACLR;
    __bis_SR_register(LPM0_bits + GIE);
}
