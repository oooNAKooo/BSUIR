#include <windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

void centerText(string text) { // центрирование текста
    CONSOLE_SCREEN_BUFFER_INFO csbi; // хранит информацию о текущем состоянии экранного буфера
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi); // определение ширины консоли
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1; // определение ширины текущего окна
    int padding = (consoleWidth - text.length()) / 2; // вычисление пробелов
    cout << string(padding, ' ') << text << endl; // вывод текста с центрированием его
}

int menu()   //меню
{
    system("cls");
    cout << endl;
    centerText("==============================================================");
    centerText("МЕНЮ");
    centerText("==============================================================");

    centerText("1. Записать в COM1;");
    centerText("2. Считать из COM2;");
    centerText("3. Изменить скорость для COM1;");
    centerText("4. Изменить скорость для COM2;");
    centerText("5. Данные COM-портов;");
    centerText("0. Выход.");

    centerText("==============================================================");
    cout << "\n\tВыберите опцию: ";

    return _getch();
}

void SetParams(HANDLE Port)   //настройка COM-порта
{
    DCB Params = { 0 }; // DCB позволяет настраивать различные аспекты взаимодействия с COM-портом
    if (Port == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            cout << "\n\tПорт не существует!" << endl;
        }
        else
        {
            cout << "\n\tНеизвестная ошибка!" << endl;
        }
    }
    Params.DCBlength = sizeof(Params);
    if (!GetCommState(&Port, &Params))
    {
        cout << "\n\tПолучена ошибка\n";
    }
    Params.Parity = EVENPARITY; // четный паритет // NOPARITY (без паритета), ODDPARITY (нечетный паритет)
    Params.BaudRate = 9600; // скорость передачи данных
    Params.ByteSize = 8; // размер байта
    Params.StopBits = ONESTOPBIT; // стоповые биты
    Params.Parity = NOPARITY; // паритет
    if (!SetCommState(Port, &Params))
    {
        cout << "\n\tОшибка настройки!" << endl;
    }
}

void GetParams(HANDLE Port1, HANDLE Port2)   //получение параметров COM-порта
{
    DCB Params1 = { 0 };
    DCB Params2 = { 0 };
    if (!GetCommState(Port1, &Params1) || !GetCommState(Port2, &Params2))
    {
        cout << "\n\tОшибка получения параметров!" << endl;
    }
    cout << "\n\tСкорость COM_1: " << Params1.BaudRate << endl << "Скорость COM_2: " << Params2.BaudRate << endl;
}

void WriteCOM(HANDLE Port, int *count)   //запись в COM-порт
{
    char data[256] = "";
    cout << "\n\tВведите строку: ";
    cin >> data;
    DWORD size = strlen(data); // размер строки
    DWORD bytes;
    BOOL Ret = WriteFile(Port, &data, size, &bytes, NULL);
    system("cls");
    cout << "\n\tБайт в COM_1: " << bytes << "\n\tИнформация: " << data << endl;
    *count += bytes;
}

void ReadCOM(HANDLE port, int *count)   //чтение из COM-порта
{
    if (*count != 0)
    {
        DWORD size = 0;
        char buff;
        cout << "\n\tБайт в COM_2: " << *count << "\n\tИнформация: ";
        for (int i = 0; i < *count; i++)
        {
            ReadFile(port, &buff, 1, &size, NULL); //чтение файла (откуда; буфер, скоторого будет считка; кол-во байт; куда сохраняем;асинхрость)
            cout << buff;
        }
    }
    else cout << "\n\tНет информации!";
    cout << endl;
    *count = 0;
}

void ChangeSpeed(HANDLE Port)   //изменение скорости COM-порта
{
    system("cls");
    int n;
    cout << "\n\tИзмените скорость: ";
    cin >> n;
    DCB Params = { 0 };
    if (!GetCommState(Port, &Params))
    {
        cout << "\n\tОшибка получения данных!" << endl;
    }
    Params.BaudRate = n;
    if (!SetCommState(Port, &Params))
    {
        cout << "\n\tОшибка настройки!" << endl;
    }
    system("cls");
    cout << "\n\tВаша скорость: " << n << endl;
}