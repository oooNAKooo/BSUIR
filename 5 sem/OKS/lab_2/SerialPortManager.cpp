#include "SerialPortManager.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

const int MAX_PACKET_SIZE = 255;

SerialPortManager::SerialPortManager(HANDLE port1, HANDLE port2)
    : m_port1(port1), m_port2(port2)
{
}

void SerialPortManager::NewSpeed(HANDLE port)
{
    DCB ComDCM = { 0 };
    ComDCM.DCBlength = sizeof(DCB);
    if (!GetCommState(port, &ComDCM))
    {
        cout << "Ошибка GetCommState" << endl;
        enter();
        return;
    }
    int n;
    cout << "Введите новую скорость: " << endl;
    cin >> n;
    ComDCM.BaudRate = n;
    if (!SetCommState(port, &ComDCM))
    {
        cout << "Ошибка SetCommState!" << endl;
        enter();
        return;
    }
    cout << "Новая скорость: " << ComDCM.BaudRate << endl;
    enter();
}

void SerialPortManager::GetSpeed(HANDLE port1, HANDLE port2)
{
    DCB COM1DCM, COM2DCM;
    memset(&COM1DCM, 0, sizeof(COM1DCM));
    memset(&COM2DCM, 0, sizeof(COM2DCM));
    if (!GetCommState(port1, &COM1DCM) || !GetCommState(port2, &COM2DCM))
    {
        cout << "Ошибка GetCommState" << endl;
        enter();
        return;
    }
    cout << "Скорость в COM1: " << COM1DCM.BaudRate << endl
        << "Скорость в COM2: " << COM2DCM.BaudRate << endl;
    enter();
}

void SerialPortManager::ApplyByteStuffing(vector<char>& data, uint8_t flag) {
    vector<char> stuffedData;
    const char ESCAPE_CHAR = '@'; // Пример использования escape-символа - управляющий символ

    for (char c : data) {
        if (static_cast<uint8_t>(c) == flag) {
            // Символ равен флагу, применяем байт-стаффинг
            stuffedData.push_back(ESCAPE_CHAR);
            stuffedData.push_back(c ^ 0x20); // Пример замены символа (здесь инвертируем бит 5)
        }
        else {
            // Символ не требует изменений
            stuffedData.push_back(c);
        }
    }

    // Заменяем исходные данные на "заэкранированные" данные
    data = stuffedData;
}

void SerialPortManager::WriteData(int* count, uint8_t sourceAddress)
{
    int packetSize;
    cout << "Введите размер пакета (n): ";
    cin >> packetSize;

    if (packetSize <= 0 || packetSize > MAX_PACKET_SIZE)
    {
        cout << "Неверный размер пакета. Должно быть от 1 до " << MAX_PACKET_SIZE << "." << endl;
        enter();
        return;
    }

    vector<char> charVector(packetSize); // выделение памяти под данные пакета

    // Генерация случайных данных
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < packetSize; ++i) {
        charVector[i] = 'a' + rand() % 26; // Генерация случайной буквы от 'a' до 'z'
    }

    uint8_t FLAG = static_cast<uint8_t>('z' + packetSize); // влаг начала пакета // стартовый байт 97

    // Поля Source Address, Destination Address и FCS (в данном случае, нулевые)
    uint8_t destinationAddress = 0;
    uint8_t fcs = 0;

    DWORD bytewrite;

    // Применение байт-стаффинга
    ApplyByteStuffing(charVector, FLAG);

    // Отправка пакета
    if (!WriteFile(m_port1, &FLAG, 1, &bytewrite, NULL) || bytewrite != 1) {
        std::cerr << "Ошибка записи флага в последовательный порт." << endl;
        enter();
        return;
    }
    if (!WriteFile(m_port1, &sourceAddress, 1, &bytewrite, NULL) || bytewrite != 1) {
        std::cerr << "Ошибка записи адреса отправителя в последовательный порт." << endl;
        enter();
        return;
    }
    if (!WriteFile(m_port1, &destinationAddress, 1, &bytewrite, NULL) || bytewrite != 1) {
        std::cerr << "Ошибка записи адреса получателя в последовательный порт." << endl;
        enter();
        return;
    }

    LPCVOID dataPtr = static_cast<LPCVOID>(charVector.data()); // указатель на константные данные

    // Отправка данных пакета
    if (!WriteFile(m_port1, dataPtr, packetSize, &bytewrite, NULL) || bytewrite != packetSize) {
        std::cerr << "Ошибка записи данных пакета в последовательный порт." << endl;
        return;
    }

    cout << "Байтов в пакете: " << packetSize << ". Байтов отправлено: " << bytewrite << endl;
    *count += bytewrite;
    *count += 4; // учет доп байтов, кот были отправлены
    if (!WriteFile(m_port1, &fcs, 1, &bytewrite, NULL) || bytewrite != 1) {
        std::cerr << "Ошибка записи FCS в последовательный порт." << endl;
        enter();
        return;
    }
    enter();
}

void SerialPortManager::ReadData(int* count, uint8_t sourceAddress)
{
    if (*count != 0) {
        uint8_t FLAG, sourceAddress, destinationAddress, fcs;
        char ReadingString[MAX_PACKET_SIZE];
        DWORD byteread = 0;
        cout << "Данные прочитаны из COM-порта: ";

        // Чтение данных в буфер
        if (ReadFile(m_port2, &FLAG, 1, &byteread, NULL) && byteread > 0) {
            cout << static_cast<uint8_t>(FLAG); // {
        }
        else {
            cout << "Ошибка чтения флага из COM-порта.";
            enter();
            return;
        }

        if (ReadFile(m_port2, &sourceAddress, 1, &byteread, NULL) && byteread > 0) {
            cout << static_cast<int>(sourceAddress); // 1
        }
        else {
            cout << "Ошибка чтения адреса отправителя из COM-порта.";
            enter();
            return;
        }

        if (ReadFile(m_port2, &destinationAddress, 1, &byteread, NULL) && byteread > 0) {
            cout << static_cast<int>(destinationAddress); // 0
        }
        else {
            cout << "Ошибка чтения адреса получателя из COM-порта.";
            enter();
            return;
        }

        // Чтение и вывод данных
        if (ReadFile(m_port2, ReadingString, *count - 4, &byteread, NULL) && byteread > 0) {
            for (DWORD i = 0; i < byteread; ++i) // буква
                cout << ReadingString[i];
        }
        else {
            cout << "Ошибка чтения данных из COM-порта." << endl;
            enter();
            return;
        }

        // Чтение и вывод FCS
        if (ReadFile(m_port2, &fcs, 1, &byteread, NULL) && byteread > 0) {
            cout << static_cast<int>(fcs) << endl; // 0
        }
        else {
            cout << "Ошибка чтения FCS из COM-порта." << endl;
            enter();
            return;
        }
    }

    *count = 0;
    enter();
}
