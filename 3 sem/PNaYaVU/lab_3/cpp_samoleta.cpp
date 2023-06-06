#include "samolet.h"

string samolet::getTip_Transporta() const
{
	return tip_transporta;
}

int samolet::getVisota() const
{
	return visota;
}

void samolet::setTip_Transporta(string t_t)
{
	this->tip_transporta = t_t;
}

void samolet::setVisota(int vis)
{
	this->visota = vis;
}

void samolet::vivod()
{
	cout << setiosflags(ios::left) << setw(14) << "Название: " << setw(20) << "Cкорость:(км/ч) " << setw(27) << "Страна производитель: " << setw(16) << "Год выпуска: " << setw(20) << "Тип транспорта: " << "Высота полета:(м)";
	cout << endl;
}

ostream& operator<<(ostream& on, const samolet& obj4)
{
	return on << endl << setw(14) << obj4.nazvanie << setw(20) << obj4.scorost << setw(27) << obj4.strana << setw(16) << obj4.godik << setw(20) << obj4.tip_transporta << obj4.visota << endl;
}
istream& operator>>(istream& in, samolet& obj4)
{
	cout << "Заполните данные о самолете: " << endl << "[1] Название: ";
	in >> obj4.nazvanie;
	cout << endl << "[2] Скорость:(км/ч) ";
	in >> obj4.scorost;
	cout << endl << "[3] Страна производитель: ";
	in >> obj4.strana;
	cout << endl << "[4] Год выпуска: ";
	in >> obj4.godik;
	cout << endl << "[5] Тип транспорта: ";
	in >> obj4.tip_transporta;
	cout << endl << "[6] Высота полета:(м) ";
	in >> obj4.visota;

	return in;
}
