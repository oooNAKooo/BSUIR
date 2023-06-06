// Курсач t,fysq.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "Glavnyi.h"
#include "samosval.h"
#include "avtobus.h"
#include "samolet.h"
#include "tractor.h"

using namespace std;

void menu();
void menu1();
void menu1_1();
void menu1_2();
void menu2();
void menu3();
void menu3_1();
void menu3_2();
void menu3_3();
void menu3_4();

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int t;
    string s;
    int y1, y2, y2_1, y2_2, y3, y4, y4_1, y4_2, y4_3, y4_4;

    samosval obj1;
    avtobus obj2;
    tractor obj3;
    samolet obj4;

b:
    while (1)
    {
        menu();

        y1 = _getch();

        switch (y1)
        {
        case '1':b1:while (1)
        {
            menu1();
            y2 = _getch();
            switch (y2)
            {

            case '1':  while (1)
            {
                menu1_1();
                y2_1 = _getch();
                switch (y2_1)
                {
                case '1':system("CLS"); cin >> obj1; break;
                case'2' : system("CLS"); cin >> obj3; break;
                case '0': cout << endl; system("pause"); goto b1;
                default: cout << endl << "Введите один из указанных параметров!" << endl; system("pause"); break;

                }
            }break;
            case '2': while (1)
            {
                menu1_2();
                y2_2 = _getch();
                switch (y2_2)
                {
                case '1': system("CLS"); cin >> obj2; break;
                case '2': system("CLS"); cin >> obj4; break;
                case '0': cout << endl; system("pause"); goto b1;
                default: cout << endl << "Введите один из указанных параметров!" << endl; system("pause"); break;
                }
            }break;
            case '0': cout << endl; system("pause"); goto b;
            default: cout << endl << "Введите один из указанных параметров!" << endl; system("pause"); break;
            }

        case '2': while (1)
        {
            menu2();
            y3 = _getch();
            switch (y3)
            {
            case '1': system("CLS"); obj1.vivod(); cout << obj1 << endl; system("pause"); break;
            case '2': system("CLS"); obj2.vivod(); cout << obj2 << endl; system("pause"); break;
            case'3':system("CLS"); obj3.vivod(); cout << obj3 << endl; system("pause"); break;
            case '4': system("CLS"); obj4.vivod(); cout << obj4 << endl; system("pause"); break;
            case '0': cout << endl; system("pause"); goto b;
            default: cout << endl << "Введите один из указанных параметров!" << endl; system("pause"); break;
            }
        }
        case '3': while (1)
        {
            menu3();
            y4 = _getch();
            switch (y4)
            {
            case '1': menu3_1();
                y4_1 = _getch();
                switch (y4_1)
                {
                case '1':system("CLS"); cin >> s; obj1.setNazv(s); system("pause"); break;
                case '2':system("CLS"); cin >> t; obj1.setScor(t); system("pause"); break;
                case '3':system("CLS"); cin >> s; obj1.setHod_uprav(s); system("pause"); break;
                case '4':system("CLS"); cin >> t; obj1.setKovsh(t); system("pause"); break;
                case '5':system("CLS"); cin >> s; obj1.setCvet(s); system("pause"); break;
                case '6':system("CLS"); cin >> t; obj1.setGruzopodem(t); system("pause"); break;
                default: cout << endl << "Введите один из указанных параметров!" << endl; system("pause"); break;
                }
                break;
            case '2':menu3_2();
                y4_2 = _getch();
                switch (y4_2)
                {
                case '1':system("CLS"); cin >> s; obj2.setNazv(s); system("pause"); break;
                case '2':system("CLS"); cin >> t; obj2.setScor(t); system("pause"); break;
                case '3':system("CLS"); cin >> s; obj2.setStrana(s); system("pause"); break;
                case '4':system("CLS"); cin >> t; obj2.setGodik(t); system("pause"); break;
                case '5':system("CLS"); cin >> s; obj2.setTip_klientov(s); system("pause"); break;
                case '6':system("CLS"); cin >> t; obj2.setDlina(t); system("pause"); break;
                default: cout << endl << "Введите один из указанных параметров!" << endl; system("pause"); break;
                }
                break;
            case '3':menu3_3();
                y4_3 = _getch();
                switch(y4_3)
                {
                case '1':system("CLS"); cin >> s; obj3.setNazv(s); system("pause"); break;
                case '2':system("CLS"); cin >> t; obj3.setScor(t); system("pause"); break;
                case '3':system("CLS"); cin >> s; obj3.setHod_uprav(s); system("pause"); break;
                case '4':system("CLS"); cin >> t; obj3.setKovsh(t); system("pause"); break;
                case '5':system("CLS"); cin >> s; obj3.setVid(s); system("pause"); break;
                case '6':system("CLS"); cin >> t; obj3.setRashod(t); system("pause"); break;
                default: cout << endl << "Введите один из указанных параметров!" << endl; system("pause"); break;
                }
            case '4': menu3_4();
                y4_4 = _getch();
                {
                    switch (y4_4)
                    {
                    case '1':system("CLS"); cin >> s; obj4.setNazv(s); system("pause"); break;
                    case '2':system("CLS"); cin >> t; obj4.setScor(t); system("pause"); break;
                    case '3':system("CLS"); cin >> s; obj4.setStrana(s); system("pause"); break;
                    case '4':system("CLS"); cin >> t; obj4.setGodik(t); system("pause"); break;
                    case '5':system("CLS"); cin >> s; obj4.setTip_Transporta(s); system("pause"); break;
                    case '6':system("CLS"); cin >> t; obj4.setVisota(t); system("pause"); break;
                    default: cout << endl << "Введите один из указанных параметров!" << endl; system("pause"); break;
                    }
                }break;
            case '0': cout << endl; system("pause"); goto b;
            default: cout << endl << "Введите один из указанных параметров!" << endl; system("pause"); break;
            }
        }
        case '0':system("CLS"); cout << "До скорой встречи!" << endl; system("pause"); return 0;
        default: cout << "Введите один из указанных параметров!" << endl; system("pause"); goto b;
        }
        }
        return 0;
    }
}
