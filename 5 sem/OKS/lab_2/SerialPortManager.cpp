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
        cout << "������ GetCommState" << endl;
        enter();
        return;
    }
    int n;
    cout << "������� ����� ��������: " << endl;
    cin >> n;
    ComDCM.BaudRate = n;
    if (!SetCommState(port, &ComDCM))
    {
        cout << "������ SetCommState!" << endl;
        enter();
        return;
    }
    cout << "����� ��������: " << ComDCM.BaudRate << endl;
    enter();
}

void SerialPortManager::GetSpeed(HANDLE port1, HANDLE port2)
{
    DCB COM1DCM, COM2DCM;
    memset(&COM1DCM, 0, sizeof(COM1DCM));
    memset(&COM2DCM, 0, sizeof(COM2DCM));
    if (!GetCommState(port1, &COM1DCM) || !GetCommState(port2, &COM2DCM))
    {
        cout << "������ GetCommState" << endl;
        enter();
        return;
    }
    cout << "�������� � COM1: " << COM1DCM.BaudRate << endl
        << "�������� � COM2: " << COM2DCM.BaudRate << endl;
    enter();
}

void SerialPortManager::ApplyByteStuffing(vector<char>& data, uint8_t flag) {
    vector<char> stuffedData;
    const char ESCAPE_CHAR = '@'; // ������ ������������� escape-������� - ����������� ������

    for (char c : data) {
        if (static_cast<uint8_t>(c) == flag) {
            // ������ ����� �����, ��������� ����-��������
            stuffedData.push_back(ESCAPE_CHAR);
            stuffedData.push_back(c ^ 0x20); // ������ ������ ������� (����� ����������� ��� 5)
        }
        else {
            // ������ �� ������� ���������
            stuffedData.push_back(c);
        }
    }

    // �������� �������� ������ �� "����������������" ������
    data = stuffedData;
}

void SerialPortManager::WriteData(int* count, uint8_t sourceAddress)
{
    int packetSize;
    cout << "������� ������ ������ (n): ";
    cin >> packetSize;

    if (packetSize <= 0 || packetSize > MAX_PACKET_SIZE)
    {
        cout << "�������� ������ ������. ������ ���� �� 1 �� " << MAX_PACKET_SIZE << "." << endl;
        enter();
        return;
    }

    vector<char> charVector(packetSize); // ��������� ������ ��� ������ ������

    // ��������� ��������� ������
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < packetSize; ++i) {
        charVector[i] = 'a' + rand() % 26; // ��������� ��������� ����� �� 'a' �� 'z'
    }

    uint8_t FLAG = static_cast<uint8_t>('z' + packetSize); // ���� ������ ������ // ��������� ���� 97

    // ���� Source Address, Destination Address � FCS (� ������ ������, �������)
    uint8_t destinationAddress = 0;
    uint8_t fcs = 0;

    DWORD bytewrite;

    // ���������� ����-���������
    ApplyByteStuffing(charVector, FLAG);

    // �������� ������
    if (!WriteFile(m_port1, &FLAG, 1, &bytewrite, NULL) || bytewrite != 1) {
        std::cerr << "������ ������ ����� � ���������������� ����." << endl;
        enter();
        return;
    }
    if (!WriteFile(m_port1, &sourceAddress, 1, &bytewrite, NULL) || bytewrite != 1) {
        std::cerr << "������ ������ ������ ����������� � ���������������� ����." << endl;
        enter();
        return;
    }
    if (!WriteFile(m_port1, &destinationAddress, 1, &bytewrite, NULL) || bytewrite != 1) {
        std::cerr << "������ ������ ������ ���������� � ���������������� ����." << endl;
        enter();
        return;
    }

    LPCVOID dataPtr = static_cast<LPCVOID>(charVector.data()); // ��������� �� ����������� ������

    // �������� ������ ������
    if (!WriteFile(m_port1, dataPtr, packetSize, &bytewrite, NULL) || bytewrite != packetSize) {
        std::cerr << "������ ������ ������ ������ � ���������������� ����." << endl;
        return;
    }

    cout << "������ � ������: " << packetSize << ". ������ ����������: " << bytewrite << endl;
    *count += bytewrite;
    *count += 4; // ���� ��� ������, ��� ���� ����������
    if (!WriteFile(m_port1, &fcs, 1, &bytewrite, NULL) || bytewrite != 1) {
        std::cerr << "������ ������ FCS � ���������������� ����." << endl;
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
        cout << "������ ��������� �� COM-�����: ";

        // ������ ������ � �����
        if (ReadFile(m_port2, &FLAG, 1, &byteread, NULL) && byteread > 0) {
            cout << static_cast<uint8_t>(FLAG); // {
        }
        else {
            cout << "������ ������ ����� �� COM-�����.";
            enter();
            return;
        }

        if (ReadFile(m_port2, &sourceAddress, 1, &byteread, NULL) && byteread > 0) {
            cout << static_cast<int>(sourceAddress); // 1
        }
        else {
            cout << "������ ������ ������ ����������� �� COM-�����.";
            enter();
            return;
        }

        if (ReadFile(m_port2, &destinationAddress, 1, &byteread, NULL) && byteread > 0) {
            cout << static_cast<int>(destinationAddress); // 0
        }
        else {
            cout << "������ ������ ������ ���������� �� COM-�����.";
            enter();
            return;
        }

        // ������ � ����� ������
        if (ReadFile(m_port2, ReadingString, *count - 4, &byteread, NULL) && byteread > 0) {
            for (DWORD i = 0; i < byteread; ++i) // �����
                cout << ReadingString[i];
        }
        else {
            cout << "������ ������ ������ �� COM-�����." << endl;
            enter();
            return;
        }

        // ������ � ����� FCS
        if (ReadFile(m_port2, &fcs, 1, &byteread, NULL) && byteread > 0) {
            cout << static_cast<int>(fcs) << endl; // 0
        }
        else {
            cout << "������ ������ FCS �� COM-�����." << endl;
            enter();
            return;
        }
    }

    *count = 0;
    enter();
}
