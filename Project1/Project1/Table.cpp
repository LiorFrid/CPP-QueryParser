
#include "Table.h"

Table::Table(string line)
{
	nameOfTable = line.substr(0,line.find("("));
	extractAttributes(line.substr(line.find("(") + 1, line.length() - nameOfTable.length() - 2));
	
}

vector<Attribute> Table::GetAttributes()
{
	return Attributes;
}

string Table::GetnameOfTable()
{
	return nameOfTable;
}

void Table::extractAttributes(string line)
{
	try
	{
		int currentposition = 0;
		int lastposition = 0;
		int length = 0;
		while (line[currentposition] != '\0')
		{
			if (line[currentposition] == ',' || line[currentposition + 1] == '\0')
			{
				length = currentposition - lastposition;
				if (line[currentposition + 1] == '\0')
					length++;
				string attributeString = line.substr(lastposition, length);
				lastposition = currentposition + 2;
				Attribute attribute;
				attribute.name = attributeString.substr(0, attributeString.find(":"));
				attribute.kind = attributeString.substr(attributeString.find(":") + 1, attributeString.length() - attribute.name.length());
				Attributes.push_back(attribute);
			}
			currentposition++;
		}
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}
}