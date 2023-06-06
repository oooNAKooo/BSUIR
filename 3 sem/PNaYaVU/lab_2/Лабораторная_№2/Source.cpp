#include <iostream>
#include <string>
#include <assert.h>
#include "Pryat.h"
#include <cassert>

String::String(const char* s) //����������� � string
{
	cout << "constrictor: " << s << endl;
	length = strlen(s); //���� �����
	sPtr = new char[length + 1]; //����� ������
	assert(sPtr != 0); //�����, ���� �� ���������
	strcpy_s(sPtr,strlen(s)+1, s); //�������� ������ � ������
}
String::String(const String& copy) //����������� �����
{
	cout << "copy const: " << copy.sPtr << endl;
	length = copy.length; //�������� �����
	sPtr = new char[length + 1]; //����� ������
	assert(sPtr != 0); //�������� ����������
	strcpy_s(sPtr, strlen(copy.sPtr) + 1, copy.sPtr); //�������� ������
}
String::~String() //����������
{
	cout << "destructior: " << sPtr << endl;
	delete[] sPtr;
}
const String& String::operator=(const String& right) //������������ �������� =
{
	cout << "operators called " << endl;
	if (&right != this) //��������� ����������������
	{
		delete[] sPtr; 
		length = right.length; //����� ����� ������
		sPtr = new char[length + 1]; //��������� ������
		assert(sPtr != 0);//�������� ����������
		strcpy_s(sPtr, strlen(right.sPtr) + 1, right.sPtr); //�������� ������
	}
	else
	{
		cout << "chto - to" << endl;
		return *this; //��������� �������������
	}
}

String& String::operator+=(const String& right) //�������� ������� � ���� ��������
{
	char* tempPtr = sPtr; //���������, ����� ����� �������
	length += right.length; //����� ����� ������
	sPtr = new char[length + 1]; //�������� ���������� �����
	assert(sPtr != 0); //�����, ���� ������ �� ���������
	strcpy_s(sPtr, strlen(tempPtr) + 1, tempPtr); //����� ����� ������ �������. ������ �������� ����� ��� ������������� � ���������� ��������
	strcat_s(sPtr, strlen(right.sPtr) + 1, right.sPtr); //������ ����� ������ �������
	delete[] tempPtr; 
	return *this;
}

String& String::operator+(const String& right)
{
	cout << "operator+ called" << endl;
	int nov_dlina = length + right.length;
	char *tempPtr = new char[nov_dlina + 1];
	strcpy_s(tempPtr,strlen(sPtr)+1, sPtr);
	strcpy_s(tempPtr + length,strlen(right.sPtr)+1, right.sPtr);
	delete[] this->sPtr;
	sPtr = tempPtr;
	length = nov_dlina;
	return *this;
}

int String::operator!()const
{
	return length == 0;
}
int String::operator==(const String &right) const
{
	return strcmp(sPtr, right.sPtr) == 0;
}
int String::operator!=(const String& right) const
{
	return strcmp(sPtr, right.sPtr) != 0;
}
int String::operator<(const String& right) const
{
	return strcmp(sPtr, right.sPtr) < 0;
}
int String::operator>(const String& right) const
{
	return strcmp(sPtr, right.sPtr) > 0;
}
int String::operator>=(const String& right) const
{
	return strcmp(sPtr, right.sPtr) >= 0;
}
int String::operator<=(const String& right) const
{
	return strcmp(sPtr, right.sPtr) <= 0;
}
char& String::operator[](int subscript)
{
	if (subscript >= length && subscript < 0)
		exit(1);
	else
		return sPtr[subscript];
}
String& String::operator()(int index, int subLength)
{
	assert(index >= 0 && index < length&& subLength >= 0);
	String* subPtr = new String;
	assert(subPtr != 0);
	int size;
	if ((subLength == 0) || (index + subLength > length))
	{
		size = length - index + 1;
	}
	else
	{
		size = subLength + 1;
	}
	delete subPtr->sPtr;
	subPtr->length = size;
	subPtr->sPtr = new char[size];
	assert(subPtr->sPtr != 0);
	for (int i = index, j = 0; i < index + size - 1; i++, j++)
	{
		subPtr->sPtr[j] = sPtr[i];
		subPtr->sPtr[j] = '\0';
		return *subPtr;
	}
}
int String::getLength()const
{
	return length;
}

ostream& operator<<(ostream& output, const String& s)
{
	output << s.sPtr;
	return output;
}

istream& operator>>(istream& input, String& s)
{
	static char temp[100];
	input >> temp;
	s = temp;
	return input;
}