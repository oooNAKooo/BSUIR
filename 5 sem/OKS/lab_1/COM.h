#include <windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

void centerText(string text) { // ������������� ������
    CONSOLE_SCREEN_BUFFER_INFO csbi; // ������ ���������� � ������� ��������� ��������� ������
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi); // ����������� ������ �������
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1; // ����������� ������ �������� ����
    int padding = (consoleWidth - text.length()) / 2; // ���������� ��������
    cout << string(padding, ' ') << text << endl; // ����� ������ � �������������� ���
}

int menu()   //����
{
    system("cls");
    cout << endl;
    centerText("==============================================================");
    centerText("����");
    centerText("==============================================================");

    centerText("1. �������� � COM1;");
    centerText("2. ������� �� COM2;");
    centerText("3. �������� �������� ��� COM1;");
    centerText("4. �������� �������� ��� COM2;");
    centerText("5. ������ COM-������;");
    centerText("0. �����.");

    centerText("==============================================================");
    cout << "\n\t�������� �����: ";

    return _getch();
}

void SetParams(HANDLE Port)   //��������� COM-�����
{
    DCB Params = { 0 }; // DCB ��������� ����������� ��������� ������� �������������� � COM-������
    if (Port == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            cout << "\n\t���� �� ����������!" << endl;
        }
        else
        {
            cout << "\n\t����������� ������!" << endl;
        }
    }
    Params.DCBlength = sizeof(Params);
    if (!GetCommState(&Port, &Params))
    {
        cout << "\n\t�������� ������\n";
    }
    Params.Parity = EVENPARITY; // ������ ������� // NOPARITY (��� ��������), ODDPARITY (�������� �������)
    Params.BaudRate = 9600; // �������� �������� ������
    Params.ByteSize = 8; // ������ �����
    Params.StopBits = ONESTOPBIT; // �������� ����
    Params.Parity = NOPARITY; // �������
    if (!SetCommState(Port, &Params))
    {
        cout << "\n\t������ ���������!" << endl;
    }
}

void GetParams(HANDLE Port1, HANDLE Port2)   //��������� ���������� COM-�����
{
    DCB Params1 = { 0 };
    DCB Params2 = { 0 };
    if (!GetCommState(Port1, &Params1) || !GetCommState(Port2, &Params2))
    {
        cout << "\n\t������ ��������� ����������!" << endl;
    }
    cout << "\n\t�������� COM_1: " << Params1.BaudRate << endl << "�������� COM_2: " << Params2.BaudRate << endl;
}

void WriteCOM(HANDLE Port, int *count)   //������ � COM-����
{
    char data[256] = "";
    cout << "\n\t������� ������: ";
    cin >> data;
    DWORD size = strlen(data); // ������ ������
    DWORD bytes;
    BOOL Ret = WriteFile(Port, &data, size, &bytes, NULL);
    system("cls");
    cout << "\n\t���� � COM_1: " << bytes << "\n\t����������: " << data << endl;
    *count += bytes;
}

void ReadCOM(HANDLE port, int *count)   //������ �� COM-�����
{
    if (*count != 0)
    {
        DWORD size = 0;
        char buff;
        cout << "\n\t���� � COM_2: " << *count << "\n\t����������: ";
        for (int i = 0; i < *count; i++)
        {
            ReadFile(port, &buff, 1, &size, NULL); //������ ����� (������; �����, ��������� ����� ������; ���-�� ����; ���� ���������;����������)
            cout << buff;
        }
    }
    else cout << "\n\t��� ����������!";
    cout << endl;
    *count = 0;
}

void ChangeSpeed(HANDLE Port)   //��������� �������� COM-�����
{
    system("cls");
    int n;
    cout << "\n\t�������� ��������: ";
    cin >> n;
    DCB Params = { 0 };
    if (!GetCommState(Port, &Params))
    {
        cout << "\n\t������ ��������� ������!" << endl;
    }
    Params.BaudRate = n;
    if (!SetCommState(Port, &Params))
    {
        cout << "\n\t������ ���������!" << endl;
    }
    system("cls");
    cout << "\n\t���� ��������: " << n << endl;
}