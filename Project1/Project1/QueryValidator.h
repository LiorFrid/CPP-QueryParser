

#pragma once
#ifndef Input_Checker
#define Input_Checker
#include <iostream>
#include<string>
#include<fstream>
#include"Table.h"
using namespace std;
class QueryValidator
{
private:
	string queryString;
	bool isCorrectQuery = true;
	string whatWrongMsg;
	string m_FileContent;
	vector<Table>ListOfTables;
	void selectStrChecker(string str);
	string checkDistinctWord(string str);
	void checkAttributeforAttLIst(string str);
	void checkAttribute(std::string& str, bool& flag, string &kind);
	void whereStrChecker(string str);
	void checkByANDandOR(string str);
	void checkCondition(string str);
	bool checkIsOppExist(string str, string& opp);
	string getKindByString(string str);
	bool checkIfINTEGER(string str);
	void fromStrChecker(string str);
	string cutStr(int first, int size);
	void checkTable(string str);
	string removeCharFromString(string str, char toRemove);

public:
	vector<Table> getTables();
	QueryValidator(string input);
	bool getIsCorrectQuery();
	void isSelectAsNeeded();
	void isFromAsNeeded();
	void isWhereAsNeeded();
	void getFileContentAndInterpretTables();
	string getWhatWrong();
	void isKeyWordExist();


};
#endif // !Input_Checker



