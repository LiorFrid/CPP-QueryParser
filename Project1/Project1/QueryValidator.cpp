#include "QueryValidator.h"

vector<Table> QueryValidator::getTables()
{
	return ListOfTables;
}

QueryValidator::QueryValidator(string input)
{
	this->queryString = input;
}

bool QueryValidator::getIsCorrectQuery()
{
	return this->isCorrectQuery;
}

string QueryValidator::getWhatWrong()
{
	return this->whatWrongMsg;
}

void QueryValidator::isKeyWordExist()
{
	size_t foundSELECT = queryString.find("SELECT");
	if (foundSELECT == std::string::npos)
	{
		isCorrectQuery = false;
		whatWrongMsg += "no SELECT\n";
	}

	size_t foundFROM = queryString.find("FROM");
	if (foundFROM == std::string::npos)
	{
		isCorrectQuery = false;
		whatWrongMsg += "no FROM\n";
	}

	size_t foundWHERE = queryString.find("WHERE");
	if (foundWHERE == std::string::npos)
	{
		isCorrectQuery = false;
		whatWrongMsg += "no WHERE\n";
	}

	if(!(foundSELECT < foundFROM && foundFROM < foundWHERE))
	{
		isCorrectQuery = false;
		whatWrongMsg += "INCORRECT ORDER\n";
	}
}

void QueryValidator::isSelectAsNeeded()
{
	if (isCorrectQuery)
	{
		string str = cutStr(queryString.find("SELECT") + 6, queryString.find("FROM") - (queryString.find("SELECT") + 6));
		selectStrChecker(str);
	}
}

void QueryValidator::selectStrChecker(string str)
{
	str = removeCharFromString(str, ' ');
	str = checkDistinctWord(str);
	if (str != "*")
	{
		size_t s = str.find(",");
		if (s != std::string::npos)
		{
			checkAttributeforAttLIst(str.substr(0, s));
			selectStrChecker(str.substr(s + 1, str.length() - s));
		}
		else
		{
			checkAttributeforAttLIst(str);
		}
	}
}

string QueryValidator::checkDistinctWord(string str)
{
	size_t size = str.find("DISTINCT");
	if (size != std::string::npos && size == 0)
		return str.substr(8, str.length() - 8);
	else
		return str;
}

void QueryValidator::checkAttributeforAttLIst(string str)
{
	bool flag = false;
	string kind;
	checkAttribute(str, flag, kind);
	if (!flag)
	{
		isCorrectQuery = flag;
		whatWrongMsg += "Parsing <attribute_list> failed\n";
	}
}

void QueryValidator::checkAttribute(std::string& str, bool& flag, string &kind)
{
	string nameoftable = str.substr(0, str.find("."));
	string nameofAtt = str.substr(str.find(".") + 1, str.length() - nameoftable.length());
	for (Table t : this->ListOfTables)
	{
		if (t.GetnameOfTable() == nameoftable)
		{
			for (Attribute a : t.GetAttributes())
			{
				if (a.name == nameofAtt)
				{
					flag = true;
					kind = a.kind;
				}
			}
		}
	}
}

string QueryValidator::cutStr(int first, int size)
{
	string substr;
	substr = queryString.substr(first, size);
	return substr;
}

void QueryValidator::isFromAsNeeded()
{
	if (isCorrectQuery)
	{
		string str = cutStr(queryString.find("FROM") + 4, queryString.find("WHERE") - (queryString.find("FROM") + 4));
		fromStrChecker(str);
	}
}

string QueryValidator::removeCharFromString(string str, char toRemove)
{
	string newstr;
	int i = 0;
	while (str[i])
	{
		if (str[i] != toRemove)
		{
			newstr+= str[i];
		}
		i++;
	}
	return newstr;
}

void QueryValidator::fromStrChecker(string str)
{
	str = removeCharFromString(str,' ');
	size_t s = str.find(",");
	if (s != std::string::npos)
	{
		checkTable(str.substr(0, s));
		fromStrChecker(str.substr(s + 1, str.length() - s));
	}
	else
	{
		checkTable(str);
	}
}

void QueryValidator::checkTable(string str)
{
	bool flag = false;
	int x = 0;
	if (str[0] == ' ')
		x = 1;
	string nameoftable = str.substr(x, str.find(".") - x);
	for (Table t : this->ListOfTables)
	{
		if (t.GetnameOfTable() == nameoftable)
			flag = true;
	}
	if (!flag)
	{
		isCorrectQuery = flag;
		whatWrongMsg += "Parsing <table_list> failed\n";
	}
}

void QueryValidator::isWhereAsNeeded()
{
	if (isCorrectQuery)
	{
		string str = cutStr(queryString.find("WHERE") + 5, queryString.length() - (queryString.find("WHERE") + 5));
		str = removeCharFromString(str, ' ');
		str = removeCharFromString(str, ';');
		whereStrChecker(str);
	}

}

void QueryValidator::whereStrChecker(string str)
{
	int lastLeftBracket = 0;
	int firstRightBracket = str.length();
	int pos = 0;
	bool flag = true;
	while (str[pos] != '\0')
	{
		if (str[pos] == '(')
			lastLeftBracket = pos;
		pos++;
	}
	pos = 0;
	while (str[pos] != '\0' && flag)
	{
		if (str[pos] == ')' && pos > lastLeftBracket)
		{
			firstRightBracket = pos;
			flag = false;
		}
		pos++;
	}
	string strToCheck;
	if (str[lastLeftBracket] == '(' && str[firstRightBracket] == ')')
	{
		strToCheck = str.substr(lastLeftBracket+1, firstRightBracket - lastLeftBracket-1);
	}
	else
	{
		 strToCheck = str.substr(lastLeftBracket, firstRightBracket - lastLeftBracket);
	}
	
	checkByANDandOR(strToCheck);
	if ( firstRightBracket != str.length())
	{
		string left = str.substr(0, lastLeftBracket);
		string middle = str.substr(lastLeftBracket + 1, firstRightBracket - lastLeftBracket - 1);
		string right = str.substr(firstRightBracket + 1, str.length() - firstRightBracket - 1);
		string newstr = left + middle + right;

		size_t found1 = newstr.find("(");
		size_t found2 = newstr.find(")");
		if (found1 != std::string::npos && found2 != std::string::npos)
			whereStrChecker(newstr);
		else
			checkByANDandOR(newstr);
	}
}

void QueryValidator::checkByANDandOR(string str)
{
	size_t ANDFOUND = str.find("AND");
	size_t ORFOUND = str.find("OR");
	if (ANDFOUND != std::string::npos || ORFOUND != std::string::npos)
	{
		if (ANDFOUND < ORFOUND)
		{
			checkCondition(str.substr(0, ANDFOUND));
			checkByANDandOR(str.substr(ANDFOUND + 3, str.length() - ANDFOUND));
		}
		else
		{
			checkCondition(str.substr(0, ORFOUND));
			checkByANDandOR(str.substr(ORFOUND + 2, str.length() - ORFOUND));
		}
	}
	else
	{
		checkCondition(str);
	}
}

void QueryValidator::checkCondition(string str)
{
	string opp ;
	bool isOppExist = checkIsOppExist(str, opp);
	bool isCorrectAttLeft = false;
	bool isCorrectAttRight = false;
	string kindLeft;
	string kindRight;
	if (isOppExist)
	{
		string left = str.substr(0, str.find(opp));
		checkAttribute(left, isCorrectAttLeft, kindLeft);
		string right = str.substr(str.find(opp) + 1, str.length() - str.find(opp));
		checkAttribute(right, isCorrectAttRight, kindRight);
		if (isCorrectAttLeft && isCorrectAttRight)
		{
			if (kindLeft != kindRight)
			{
				isCorrectQuery = false;
				whatWrongMsg = "Parsing <condition> failed";
			}
		}
		else if (isCorrectAttLeft)
		{
			kindRight = getKindByString(right);
			if (kindLeft != kindRight)
			{
				isCorrectQuery = false;
				whatWrongMsg = "Parsing <condition> failed";
			}
		}
		else
		{
			isCorrectQuery = false;
			whatWrongMsg = "Parsing <condition> failed";
		}
	}
	else
	{
		isCorrectQuery = false;
		whatWrongMsg = "Parsing <condition> failed";
	}
}

bool QueryValidator::checkIsOppExist(string str, string& opp)
{
	bool flag = false;
	if (str.find("=") != std::string::npos)
	{
		flag = true;
		opp = "=";
	}
	else if (str.find(">=") != std::string::npos)
	{
		flag = true;
		opp = ">=";
	}
	
	else if (str.find("<=") != std::string::npos)
	{
		flag = true;
		opp = "<=";
	}
	else if (str.find("<") != std::string::npos)
	{
		flag = true;
		opp = "<";
	}
	else if (str.find(">") != std::string::npos)
	{
		flag = true;
		opp = ">";
	}
	return flag;
}

string QueryValidator::getKindByString(string str)
{
	if (str[0] == '"' && str[str.length() - 1] == '"')
		return "STRING";
	if (checkIfINTEGER(str))
		return "INTEGER";
	else
		return "UNKNOWN";
}

bool QueryValidator::checkIfINTEGER(string str)
{
	bool flag = true;
	int pos = 0;
	while (str[pos] != '\0')
	{
		if (str[pos] < '0' || str[pos] > '9')
			flag = false;
		pos++;
	}
	return flag;
 }

void QueryValidator::getFileContentAndInterpretTables()
{
	ifstream myFile("C:\\scheme.txt");
	string line;
	while (getline(myFile, line))
	{
		Table table(line);
		ListOfTables.push_back(table);
	}
	myFile.close();
}

