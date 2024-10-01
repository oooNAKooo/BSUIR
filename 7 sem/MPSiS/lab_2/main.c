#include <msp430.h>

#define LED1 BIT0      // ��������� ��������� ������ (P1.0)
#define LED2 BIT1      // ������ (P8.1)
#define LED3 BIT2      // ��������� ������ ����������� ����������������� (P8.2)
#define BUTTON BIT7    // ������ S1 ���������� � P1.7

void setup_clock();    // �������� ������� ��������� ������������
void delay(unsigned int cycles); // �������� ������� ��������

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // ������������� watchdog ������

    // ��������� ������ ��� �����������
    P1DIR |= LED1;                // ������������� LED1 ��� �����
    P1OUT &= ~LED1;               // ���������� LED1 ��������

    P8DIR |= LED2 + LED3;         // ������������� LED2 � LED3 ��� ������
    P8OUT &= ~(LED2 + LED3);      // ���������� LED2 � LED3 ���������

    // ��������� ������
    P1DIR &= ~BUTTON;             // ������������� ������ ��� ����
    P1REN |= BUTTON;              // �������� ������������� �������� ������
    P1OUT |= BUTTON;              // ����������� �������� � Vcc

    unsigned char button_state = 0; // ���������� ��� ������������ ��������� ������

    setup_clock();  // ��������� ������������

    while (1)
    {
        if ((P1IN & BUTTON) == 0) // ���������, ������ �� ������ (������ �������)
        {
            __delay_cycles(20000); // �����������

            if ((P1IN & BUTTON) == 0) // �������������� �������� ������
            {
                button_state ^= 1; // ����������� ��������� ������

                if (button_state == 1) // ���� ������ ������ ������ ���
                {
                    P1OUT |= LED1;     // �������� LED1 (��������� ��������� ������)
                    delay(10000);      // ��������
                    P8OUT |= LED2;     // �������� LED2
                    delay(10000);      // ��������
                    P8OUT |= LED3;     // �������� LED3
                }
                else // ���� ������ ������ ��������
                {
                    P1OUT &= ~LED1;    // ��������� LED1
                    P8OUT &= ~(LED2 + LED3); // ��������� LED2 � LED3
                    // ������ � ����� LPM1 (����� ����������� �����������������)
                    __bis_SR_register(LPM1_bits + GIE); // �������� LPM1
                }

                while ((P1IN & BUTTON) == 0); // �������, ���� ������ ��������
            }
        }
    }
}

// ��������� �������� ������
void setup_clock() {
    // ����������� DCO �� 3.1 ���, SMCLK � ��������� /16
    UCSCTL3 = SELREF__REFOCLK;         // �������� ������� ������� ��� FLL - REFO
    UCSCTL4 = SELA__REFOCLK;           // ACLK �� REFO

    UCSCTL1 = DCORSEL_4;               // �������� 3.1 ��� ��� DCO
    UCSCTL2 = FLLD__16 + 63;           // �������� /16 � ��������� (3.1 ���)

    // �������� ������������ �������
    __delay_cycles(250000);
}

// ������� ��� �������� ��������
void delay(unsigned int cycles)
{
    while (cycles--)
    {
        __no_operation(); // ������ �� ������, ������ ����
    }
}
