#pragma once
#ifndef STRING1_H
#define STRING1_H
#define assert
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

using namespace std;

class String
{
	friend ostream& operator<<(ostream&, const String&);  
	friend istream& operator>>(istream&, String&); //
public:
	String(const char* = ""); //����������� ��������������
	String(const String&); //����������� �����
	~String(); //����������
	const String& operator=(const String&); //������������
	String& operator+=(const String&); //�������� +=
    String& operator+(const String&); //�������� +
	int operator!() const; //���� �� 0
	int operator==(const String&) const; //�������� �1=�2
	int operator!=(const String&) const; //�������� �1!=�2
	int operator<(const String&) const; //�������� �1<�2
	int operator>(const String&) const; //�������� �1>�2
	int operator>=(const String&) const; //�������� �1>=�2
	int operator<=(const String&) const; //�������� c1<=�2
	char &operator[](int); //������� ������ char
	String &operator()(int, int); //������� ���������
	int getLength() const; //������� ����� ������
private:
	char* sPtr; //��������� �� ����� ������
	int length; //����� ������
};
#endif