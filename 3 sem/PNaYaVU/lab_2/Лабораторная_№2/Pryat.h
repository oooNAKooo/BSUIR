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
	String(const char* = ""); //конструктор преобразования
	String(const String&); //конструктор копии
	~String(); //деструктор
	const String& operator=(const String&); //присваивание
	String& operator+=(const String&); //оператор +=
    String& operator+(const String&); //оператор +
	int operator!() const; //если не 0
	int operator==(const String&) const; //проверка с1=с2
	int operator!=(const String&) const; //проверка с1!=с2
	int operator<(const String&) const; //проверка с1<с2
	int operator>(const String&) const; //проверка с1>с2
	int operator>=(const String&) const; //проверка с1>=с2
	int operator<=(const String&) const; //проверка c1<=с2
	char &operator[](int); //возврат ссылки char
	String &operator()(int, int); //возврат подстроки
	int getLength() const; //возврат длины строки
private:
	char* sPtr; //указатель на начло строки
	int length; //длина строки
};
#endif