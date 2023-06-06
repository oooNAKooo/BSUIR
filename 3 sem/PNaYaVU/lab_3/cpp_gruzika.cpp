#include "Gruzik.h"


string gruzi::getHod_uprav() const
{
	return hod_uprav;
}
int gruzi::getKovsh() const
{
	return kovsh;
}
void gruzi::setHod_uprav(string hu)
{
	this->hod_uprav = hu;
}

void gruzi::setKovsh(int k)
{
	this->kovsh = k;
}
ostream& operator<<(ostream& on, const gruzi& obj1)
{
	cout << "Ход управления: " << setw(10) << "Количество ковшей: " << endl;
	cout << obj1.hod_uprav << setw(10) << obj1.kovsh << endl;
	return on;
}

istream& operator>>(istream& in, gruzi& obj1)
{
	return in;
}
