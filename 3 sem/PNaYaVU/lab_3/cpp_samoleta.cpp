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
	cout << setiosflags(ios::left) << setw(14) << "��������: " << setw(20) << "C�������:(��/�) " << setw(27) << "������ �������������: " << setw(16) << "��� �������: " << setw(20) << "��� ����������: " << "������ ������:(�)";
	cout << endl;
}

ostream& operator<<(ostream& on, const samolet& obj4)
{
	return on << endl << setw(14) << obj4.nazvanie << setw(20) << obj4.scorost << setw(27) << obj4.strana << setw(16) << obj4.godik << setw(20) << obj4.tip_transporta << obj4.visota << endl;
}
istream& operator>>(istream& in, samolet& obj4)
{
	cout << "��������� ������ � ��������: " << endl << "[1] ��������: ";
	in >> obj4.nazvanie;
	cout << endl << "[2] ��������:(��/�) ";
	in >> obj4.scorost;
	cout << endl << "[3] ������ �������������: ";
	in >> obj4.strana;
	cout << endl << "[4] ��� �������: ";
	in >> obj4.godik;
	cout << endl << "[5] ��� ����������: ";
	in >> obj4.tip_transporta;
	cout << endl << "[6] ������ ������:(�) ";
	in >> obj4.visota;

	return in;
}
