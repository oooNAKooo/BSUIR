#include "Glavnyi.h"

int Transport::getScor()const
{
	return scorost;
}

string Transport::getNazv()const
{
	return nazvanie;
}

void Transport::setNazv(string n)
{
	this->nazvanie = n;
}

void Transport::setScor(int s)
{
	this->scorost = s;
}
//
//ostream& operator<< (ostream& os, const Transport& ss)
//{
//	cout << "Марка автомобиля: " << setw(10) << "Скорость автомобиля: " << endl;
//	cout << ss.nazvanie << setw(10) << ss.scorost << endl;
//	return os;
//}
//
//istream& operator>>(istream& in, Transport& is)
//{
//	cout << "Введите марку автомобиля: ";
//	in >> is.nazvanie;
//	cout << "Введите скорость автомобиля: ";
//	in >> is.scorost;
//	return in;
//}
