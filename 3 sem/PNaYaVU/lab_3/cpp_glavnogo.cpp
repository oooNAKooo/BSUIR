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
//	cout << "����� ����������: " << setw(10) << "�������� ����������: " << endl;
//	cout << ss.nazvanie << setw(10) << ss.scorost << endl;
//	return os;
//}
//
//istream& operator>>(istream& in, Transport& is)
//{
//	cout << "������� ����� ����������: ";
//	in >> is.nazvanie;
//	cout << "������� �������� ����������: ";
//	in >> is.scorost;
//	return in;
//}
