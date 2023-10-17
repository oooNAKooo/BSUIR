#include "UserInterface.h"
#include <iostream>

using namespace std;

int menu()
{
    system("CLS");
    int n;
    cout << "1-Записать данные в COM2" << endl
        << "2-Прочитать данные из COM1" << endl
        << "3-Изменить скорость в COM1" << endl
        << "4-Изменить скорость в COM2" << endl
        << "5-Вывести скорость в COM-портах" << endl
        << "6-Закрыть COM-порты и завершить программу" << endl;
    cin >> n;
    return n;
}

void enter()
{
    cout << "Для продолжения нажмите Enter....." << endl;
    cin.ignore();
    cin.get();
}
