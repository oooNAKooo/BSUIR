#include <iostream>
#include "samosval.h"
#include "avtobus.h"

using namespace std;

void menu1()
{
	system("CLS"); cout << "�������� ��� ����������: " << endl << "[1] ��������;" << endl << "[2] ������������;" << endl << "[0] �����.";  fflush(stdin);
}

void menu1_1()
{
	system("CLS"); cout << "�������� �������� ���������: " << endl << "[1] ��������;" << endl << "[2] �������;" << endl << "[0] �����.";  fflush(stdin);
}

void menu1_2()
{
	system("CLS"); cout << "�������� ������������ ���������: " << endl << "[1] �������;" << endl << "[2] �������;" << endl << "[0] �����.";  fflush(stdin);
}
void menu2()
{
	system("CLS"); cout << "�������� ���������: " << endl << "[1] ��������;" << endl << "[2] �������;" << endl << "[3] �������;" << endl <<"[4] �������; " << endl << "[0] �����."; fflush(stdin);
}

void menu()
{
	system("CLS");
	cout << "�������� ���������: " <<
		endl << "[1] ��������: " <<
		endl << "[2] �����������: " <<
		endl << "[3] ��������: " <<
		endl << "[0] ��������� ���������." <<
		endl;
	fflush(stdin);
}

void menu3_1()
{
	system("CLS");
	cout << "�������� �������� ��� ���������: " << endl << "[1] ��������: ";
	cout << endl << "[2] C�������:(��/�) ";
	cout << endl << "[3] ��� ����������: ";
	cout << endl << "[4] ���������� ������: ";
	cout << endl << "[5] ����: ";
	cout << endl << "[6] ����������������:(��) " << endl;
	fflush(stdin);
}

void menu3_2()
{
	system("CLS");
	cout << "�������� �������� ��� ���������: " << endl << "[1] ��������: ";
	cout << endl << "[2] C�������:(��/�) ";
	cout << endl << "[3] ������ �������������: ";
	cout << endl << "[4] ��� �������: ";
	cout << endl << "[5] ��� ����������: ";
	cout << endl << "[6] �����:(�) " << endl;
	fflush(stdin);
}
void menu3()
{
	system("CLS"); cout << "�������� ������: " << endl << "[1] ��������;" << endl << "[2] �������;" << endl << "[4] �������;" <<endl << "[0] �����." << endl; fflush(stdin);
}

void menu3_3()
{
	system("CLS");
	cout << "�������� �������� ��� ���������: " << endl << "[1] ��������: ";
	cout << endl << "[2] C�������:(��/�) ";
	cout << endl << "[3] ��� ����������: ";
	cout << endl << "[4] ���������� ������: ";
	cout << endl << "[5] ��� ������: ";
	cout << endl << "[6] ������ �������:(�/��) " << endl;
	fflush(stdin);
}

void menu3_4()
{
	system("CLS");
	cout << "�������� �������� ��� ���������: " << endl << "[1] ��������: ";
	cout << endl << "[2] C�������:(��/�)";
	cout << endl << "[3] ������ �������������: ";
	cout << endl << "[4] ��� �������: ";
	cout << endl << "[5] ��� ����������: ";
	cout << endl << "[6] ������ ������:(�) " << endl;
	fflush(stdin);
}