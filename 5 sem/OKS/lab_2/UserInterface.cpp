#include "UserInterface.h"
#include <iostream>

using namespace std;

int menu()
{
    system("CLS");
    int n;
    cout << "1-�������� ������ � COM2" << endl
        << "2-��������� ������ �� COM1" << endl
        << "3-�������� �������� � COM1" << endl
        << "4-�������� �������� � COM2" << endl
        << "5-������� �������� � COM-������" << endl
        << "6-������� COM-����� � ��������� ���������" << endl;
    cin >> n;
    return n;
}

void enter()
{
    cout << "��� ����������� ������� Enter....." << endl;
    cin.ignore();
    cin.get();
}
