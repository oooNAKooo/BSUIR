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
{	//setiosflags(ios::left) выравнивание по левому краю 
	cout << setiosflags(ios::left) <<setw(14) << "Название: " << setw(17) << "Cкорость:(км/ч)" << setw(23) << "Ход упраления: " << setw(27) << "Количество ковшей: " << setw(15) << "Цвет: " << "Грузоподъемность:(кг)";
	cout << endl;
}

ostream& operator<< (ostream& on, const samosval& obj3)
{
	return on << endl << setw(14) << obj3.nazvanie << setw(17) << obj3.scorost << setw(23) << obj3.hod_uprav << setw(27) << obj3.kovsh << setw(15) << obj3.cvet  << obj3.gruzopodem << endl;
}

istream& operator>>(istream& in, samosval& obj3)
{
	cout << "Заполните данные о cамосвале: " << endl << "[1] Название: ";
	in >> obj3.nazvanie;
	cout << endl << "[2] Cкорость:(км/ч) ";
	in >> obj3.scorost;
	cout << endl << "[3] Ход управления: ";
	in >> obj3.hod_uprav;
	cout << endl << "[4] Количество ковшей: ";
	in >> obj3.kovsh;
	cout << endl << "[5] Цвет: ";
	in >> obj3.cvet;
	cout << endl << "[6] Грузоподъемность:(кг) ";
	in >> obj3.gruzopodem;
	return in;
}
