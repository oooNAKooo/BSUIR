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
	cout << setiosflags(ios::left) << setw(14) << "��������: " << setw(20) << "C�������:(��/�) " << setw(27) << "������ �������������: " << setw(16) << "��� �������: " << setw(20) << "��� ����������: " << "�����:(�)";
	cout << endl;
}
ostream& operator<< (ostream& on, const avtobus& obj2)
{
	return on << endl << setw(14) << obj2.nazvanie << setw(20) << obj2.scorost << setw(27) << obj2.strana << setw(16) << obj2.godik << setw(20) << obj2.tip_klientov << obj2.dlina << endl;
}

istream& operator>>(istream& in, avtobus& obj2)
{
	cout << "��������� ������ �� ��������: " << endl << "[1] ��������: ";
	in >> obj2.nazvanie;
	cout << endl << "[2] ��������:(��/�) ";
	in >> obj2.scorost;
	cout << endl << "[3] ������ �������������: ";
	in >> obj2.strana;
	cout << endl << "[4] ��� �������: ";
	in >> obj2.godik;
	cout << endl << "[5] ��� ����������: ";
	in >> obj2.tip_klientov;
	cout << endl << "[6] �����:(�) ";
	in >> obj2.dlina;

	return in;
}