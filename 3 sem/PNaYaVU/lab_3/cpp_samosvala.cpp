#include "samosval.h"

string samosval::getCvet() const
{
	return cvet;
}
int samosval::getGruzopodem() const
{
	return gruzopodem;
}
void samosval::setCvet(string c)
{
	this->cvet = c;
}

void samosval::setGruzopodem(int g)
{
	this->gruzopodem = g;
}

void samosval::vivod()
{	//setiosflags(ios::left) ������������ �� ������ ���� 
	cout << setiosflags(ios::left) <<setw(14) << "��������: " << setw(17) << "C�������:(��/�)" << setw(23) << "��� ���������: " << setw(27) << "���������� ������: " << setw(15) << "����: " << "����������������:(��)";
	cout << endl;
}

ostream& operator<< (ostream& on, const samosval& obj3)
{
	return on << endl << setw(14) << obj3.nazvanie << setw(17) << obj3.scorost << setw(23) << obj3.hod_uprav << setw(27) << obj3.kovsh << setw(15) << obj3.cvet  << obj3.gruzopodem << endl;
}

istream& operator>>(istream& in, samosval& obj3)
{
	cout << "��������� ������ � c��������: " << endl << "[1] ��������: ";
	in >> obj3.nazvanie;
	cout << endl << "[2] C�������:(��/�) ";
	in >> obj3.scorost;
	cout << endl << "[3] ��� ����������: ";
	in >> obj3.hod_uprav;
	cout << endl << "[4] ���������� ������: ";
	in >> obj3.kovsh;
	cout << endl << "[5] ����: ";
	in >> obj3.cvet;
	cout << endl << "[6] ����������������:(��) ";
	in >> obj3.gruzopodem;
	return in;
}
