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
{	//setiosflags(ios::left) выравнивание по левому краю 
	cout << setiosflags(ios::left) << setw(14) << "Название: " << setw(17) << "Cкорость:(км/ч)" << setw(23) << "Ход упраления: " << setw(27) << "Количество ковшей: " << setw(15) << "Вид мотора: " << "Расход топлива:(л/км)";
	cout << endl;
}

ostream& operator<< (ostream& on, const tractor& obj3)
{
	return on << endl << setw(14) << obj3.nazvanie << setw(17) << obj3.scorost << setw(23) << obj3.hod_uprav << setw(27) << obj3.kovsh << setw(15) << obj3.vid << obj3.rashod << endl;
}

istream& operator>>(istream& in, tractor& obj3)
{
	cout << "Заполните данные о тракторе: " << endl << "[1] Название: ";
	in >> obj3.nazvanie;
	cout << endl << "[2] Cкорость:(км/ч) ";
	in >> obj3.scorost;
	cout << endl << "[3] Ход управления: ";
	in >> obj3.hod_uprav;
	cout << endl << "[4] Количество ковшей: ";
	in >> obj3.kovsh;
	cout << endl << "[5] Вид мотора: ";
	in >> obj3.vid;
	cout << endl << "[6] Расход топлива:(л/км) ";
	in >> obj3.rashod;
	return in;
}
