#include "DataCell.h"
#include <sstream>
#include <iostream>

using namespace std;

constexpr auto DATACELL_INTEGER = 1;
constexpr auto DATACELL_FLOAT = 2;
constexpr auto DATACELL_TEXT = 3;
constexpr auto DATACELL_BLOB = 4;
constexpr auto DATACELL_NULL = 5;
constexpr auto DATACELL_BOOL = 6;

namespace DBAbstraction
{

DataCell::DataCell()
{
	value = 0;
	type = DATACELL_NULL;
}

DataCell::DataCell(int op_value)
{
	value = op_value;
	type = DATACELL_INTEGER;
}

DataCell::DataCell(const char* op_text): text(op_text)
{
	type = DATACELL_TEXT;
	value=0;
}


DataCell::~DataCell()
{
}

bool DataCell::hasValue()
{
	return (type != DATACELL_NULL);
}

int DataCell::getInt()
{
	if(type!=DATACELL_INTEGER)
	{
		dbe.setMessage("Wrong datatype on column");
		throw dbe;
	}
	return value;
}

std::string DataCell::getString()
{
	if(type!=DATACELL_TEXT)
	{
		dbe.setMessage("Wrong datatype on column");
		throw dbe;
	}
	return text;
}

bool DataCell::getBool()
{
	if(type!=DATACELL_INTEGER)
	{
		dbe.setMessage("Wrong datatype on column");
		throw dbe;
	}
	return (value!=0);
}

}


