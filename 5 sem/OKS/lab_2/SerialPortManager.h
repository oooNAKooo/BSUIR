#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <Windows.h>
#include <vector>
#include "UserInterface.h"

class SerialPortManager
{
public:
    SerialPortManager(HANDLE port1, HANDLE port2);

    void NewSpeed(HANDLE port);
    void GetSpeed(HANDLE port1, HANDLE port2);
    void WriteData(int* count, uint8_t sourceAddress);
    void ReadData(int* count, uint8_t sourceAddress);

private:
    HANDLE m_port1;
    HANDLE m_port2;

    void ApplyByteStuffing(std::vector<char>& data, uint8_t flag);
};

#endif
