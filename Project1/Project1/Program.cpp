#include "Program.h"

void Program::Run()
{
	string queryString;
	cout << "Make sure that the path C:\\scheme.txt exist\n";
	cout << "Please enter a query\n";
	getline(cin, queryString);
	QueryValidator query(queryString);
	query.getFileContentAndInterpretTables();
	query.isKeyWordExist();
	query.isSelectAsNeeded();
	query.isFromAsNeeded();
	query.isWhereAsNeeded();
	if (query.getIsCorrectQuery())
		cout << "\nValid query\n";
	else
		cout << "\nInvalid query:\n" << query.getWhatWrong() << endl;
}