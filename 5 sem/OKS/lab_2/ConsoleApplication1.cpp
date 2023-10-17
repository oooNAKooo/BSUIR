#include <iostream>
#include <Windows.h>
#include <exception>
#include "SerialPortManager.h"
#include "UserInterface.h"

using namespace std;

int main()
{
    setlocale(0, "");
    LPCWSTR com1 = L"COM1", com2 = L"COM2";
    HANDLE m_first_Handle = CreateFile(com1, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (m_first_Handle == (HANDLE)-1)
    {
        m_first_Handle = 0;
        throw exception();
    }
    HANDLE m_second_Handle = CreateFile(com2, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (m_second_Handle == (HANDLE)-1)
    {
        m_second_Handle = 0;
        throw exception();
    }

    SerialPortManager serialPortManager(m_first_Handle, m_second_Handle);
    int count = 0;

    while (true)
    {
        int operation = menu();
        switch (operation)
        {
        case 1:
            serialPortManager.WriteData(&count, 1);
            break;
        case 2:
            serialPortManager.ReadData(&count, 2);
            break;
        case 3:
            serialPortManager.NewSpeed(m_first_Handle);
            break;
        case 4:
            serialPortManager.NewSpeed(m_second_Handle);
            break;
        case 5:
            serialPortManager.GetSpeed(m_first_Handle, m_second_Handle);
            break;
        case 6:
            CloseHandle(m_first_Handle);
            CloseHandle(m_second_Handle);
            cout << "До свидания" << endl;
            return 0;
            break;
        default:
            cout << "Вы ввели неверную опцию, пожалуйста, попробуйте еще раз!" << endl;
            enter();
            break;
        }
    }
    return 0;
}
