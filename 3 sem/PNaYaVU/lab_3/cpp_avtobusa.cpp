#include "avtobus.h"



//string avtobus::getTip_klientov() const
//{
//	return tip_klientov;
//}

int avtobus::getDlina() const
{
	return dlina;
}

void avtobus::setTip_klientov(string t)
{
	this->tip_klientov = t;
}
void avtobus::setDlina(int d)
{
	this->dlina = d;
}


void avtobus::vivod()
{
	cout << setiosflags(ios::left) << setw(14) << "Название: " << setw(20) << "Cкорость:(км/ч) " << setw(27) << "Страна производитель: " << setw(16) << "Год выпуска: " << setw(20) << "Тип пассажиров: " << "Длина:(м)";
	cout << endl;
}
ostream& operator<< (ostream& on, const avtobus& obj2)
{
	return on << endl << setw(14) << obj2.nazvanie << setw(20) << obj2.scorost << setw(27) << obj2.strana << setw(16) << obj2.godik << setw(20) << obj2.tip_klientov << obj2.dlina << endl;
}

istream& operator>>(istream& in, avtobus& obj2)
{
	cout << "Заполните данные об автобусе: " << endl << "[1] Название: ";
	in >> obj2.nazvanie;
	cout << endl << "[2] Скорость:(км/ч) ";
	in >> obj2.scorost;
	cout << endl << "[3] Страна производитель: ";
	in >> obj2.strana;
	cout << endl << "[4] Год выпуска: ";
	in >> obj2.godik;
	cout << endl << "[5] Тип пассажиров: ";
	in >> obj2.tip_klientov;
	cout << endl << "[6] Длина:(м) ";
	in >> obj2.dlina;

	return in;
}