#include "COM.h"

int main()
{
    system("chcp 1251 > null"); // руссификация

    HANDLE COM_1 = ::CreateFile(L"COM1", GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    HANDLE COM_2 = ::CreateFile(L"COM2", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    
    SetParams(COM_1);
    SetParams(COM_2);
    
    int count = 0;

    while (1)
    {
        switch (menu())
        {
        case '1': system("cls"); WriteCOM(COM_1, &count); _getch(); break;
        case '2': system("cls"); ReadCOM(COM_2, &count); _getch(); break;
        case '3': system("cls"); ChangeSpeed(COM_1); _getch(); break;
        case '4': system("cls"); ChangeSpeed(COM_2); _getch(); break;
        case '5': system("cls"); GetParams(COM_1, COM_2); _getch(); break;
        case '0': system("cls"); cout << "\n\tРабота завершена."; return 0;
        default:  system("cls"); cout << "\n\tВведена не верная команда!\n"; cout << "\t"; system("pause"); break;
        }
    }
    return 0;
}