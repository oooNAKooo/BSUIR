#include "dlya_vseh.h"


string mashinki::getStrana() const
{
	return strana;
}
int mashinki::getGodik() const
{
	return godik;
}
void mashinki::setStrana(string s)
{
	this->strana = s;
}

void mashinki::setGodik(int g)
{
	this->godik = g;
}
ostream& operator<< (ostream& on, const mashinki& obj2)
{
	cout << "Страна производитель: " << setw(10) << "Год выпуска: " << endl;
	cout << obj2.strana << setw(10) << obj2.godik << endl;
	return on;
}

istream& operator>>(istream& in, mashinki& obj1)
{
	return in;
}