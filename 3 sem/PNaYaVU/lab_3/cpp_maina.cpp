#include <iostream>
#include "samosval.h"
#include "avtobus.h"

using namespace std;

void menu1()
{
	system("CLS"); cout << "Выберите тип транспорта: " << endl << "[1] Грузовые;" << endl << "[2] Общественные;" << endl << "[0] Назад.";  fflush(stdin);
}

void menu1_1()
{
	system("CLS"); cout << "Выберите грузовой транспорт: " << endl << "[1] Самосвал;" << endl << "[2] Трактор;" << endl << "[0] Назад.";  fflush(stdin);
}

void menu1_2()
{
	system("CLS"); cout << "Выберите общественный транспорт: " << endl << "[1] Автобус;" << endl << "[2] Самолет;" << endl << "[0] Назад.";  fflush(stdin);
}
void menu2()
{
	system("CLS"); cout << "Выберите транспорт: " << endl << "[1] Самосвал;" << endl << "[2] Автобус;" << endl << "[3] Трактор;" << endl <<"[4] Самолет; " << endl << "[0] Назад."; fflush(stdin);
}

void menu()
{
	system("CLS");
	cout << "Выберите категорию: " <<
		endl << "[1] Добавить: " <<
		endl << "[2] Просмотреть: " <<
		endl << "[3] Изменить: " <<
		endl << "[0] Завершить программу." <<
		endl;
	fflush(stdin);
}

void menu3_1()
{
	system("CLS");
	cout << "Выберите параметр для изменения: " << endl << "[1] Название: ";
	cout << endl << "[2] Cкорость:(км/ч) ";
	cout << endl << "[3] Ход управления: ";
	cout << endl << "[4] Количество ковшей: ";
	cout << endl << "[5] Цвет: ";
	cout << endl << "[6] Грузоподъемность:(кг) " << endl;
	fflush(stdin);
}

void menu3_2()
{
	system("CLS");
	cout << "Выберите параметр для изменения: " << endl << "[1] Название: ";
	cout << endl << "[2] Cкорость:(км/ч) ";
	cout << endl << "[3] Страна производитель: ";
	cout << endl << "[4] Год выпуска: ";
	cout << endl << "[5] Тип пассажиров: ";
	cout << endl << "[6] Длина:(м) " << endl;
	fflush(stdin);
}
void menu3()
{
	system("CLS"); cout << "Выберите машину: " << endl << "[1] Самосвал;" << endl << "[2] Автобус;" << endl << "[4] Самолет;" <<endl << "[0] Назад." << endl; fflush(stdin);
}

void menu3_3()
{
	system("CLS");
	cout << "Выберите параметр для изменения: " << endl << "[1] Название: ";
	cout << endl << "[2] Cкорость:(км/ч) ";
	cout << endl << "[3] Ход управления: ";
	cout << endl << "[4] Количество ковшей: ";
	cout << endl << "[5] Вид мотора: ";
	cout << endl << "[6] Расход топлива:(л/км) " << endl;
	fflush(stdin);
}

void menu3_4()
{
	system("CLS");
	cout << "Выберите параметр для изменения: " << endl << "[1] Название: ";
	cout << endl << "[2] Cкорость:(км/ч)";
	cout << endl << "[3] Страна производитель: ";
	cout << endl << "[4] Год выпуска: ";
	cout << endl << "[5] Тип транспорта: ";
	cout << endl << "[6] Высота полета:(м) " << endl;
	fflush(stdin);
}