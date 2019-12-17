#pragma once
#ifndef TABLE_
#define TABLE_

#include <iostream>
#include<string>
#include<vector>
using namespace std;
struct Attribute {
	string name;
	string kind;
};
class Table
{
private:
	string nameOfTable;
	vector<Attribute> Attributes;
	void extractAttributes(string line);

public:
	string GetnameOfTable();
	vector<Attribute>  GetAttributes();
	Table(string line);
};

#endif // !TABLE_
