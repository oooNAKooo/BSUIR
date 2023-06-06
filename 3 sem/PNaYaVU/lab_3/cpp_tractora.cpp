#include "tractor.h"

string tractor::getVid() const
{
	return vid;
}
int tractor::getRashod() const
{
	return rashod;
}
void tractor::setVid(string v)
{
	this->vid = v;
}

void tractor::setRashod(int r)
{
	this->rashod = r;
}

void tractor::vivod()
{	//setiosflags(ios::left) ������������ �� ������ ���� 
	cout << setiosflags(ios::left) << setw(14) << "��������: " << setw(17) << "C�������:(��/�)" << setw(23) << "��� ���������: " << setw(27) << "���������� ������: " << setw(15) << "��� ������: " << "������ �������:(�/��)";
	cout << endl;
}

ostream& operator<< (ostream& on, const tractor& obj3)
{
	return on << endl << setw(14) << obj3.nazvanie << setw(17) << obj3.scorost << setw(23) << obj3.hod_uprav << setw(27) << obj3.kovsh << setw(15) << obj3.vid << obj3.rashod << endl;
}

istream& operator>>(istream& in, tractor& obj3)
{
	cout << "��������� ������ � ��������: " << endl << "[1] ��������: ";
	in >> obj3.nazvanie;
	cout << endl << "[2] C�������:(��/�) ";
	in >> obj3.scorost;
	cout << endl << "[3] ��� ����������: ";
	in >> obj3.hod_uprav;
	cout << endl << "[4] ���������� ������: ";
	in >> obj3.kovsh;
	cout << endl << "[5] ��� ������: ";
	in >> obj3.vid;
	cout << endl << "[6] ������ �������:(�/��) ";
	in >> obj3.rashod;
	return in;
}
