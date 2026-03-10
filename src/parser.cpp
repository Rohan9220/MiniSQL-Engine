#include "../include/parser.h"
#include <sstream>
#include <iostream>

using namespace std;

ParsedQuery parseInsertQuery(string query)
{
    ParsedQuery result;

    // find INTO
    int intoPos = query.find("INTO");
    int valuesPos = query.find("VALUES");

    // extract table name
    result.tableName = query.substr(intoPos + 5, valuesPos - (intoPos + 5));

    // remove spaces
    result.tableName.erase(0, result.tableName.find_first_not_of(" "));
    result.tableName.erase(result.tableName.find_last_not_of(" ") + 1);

    // extract values section
    int start = query.find("(");
    int end = query.find(")");

    string valuesStr = query.substr(start + 1, end - start - 1);

    stringstream ss(valuesStr);
    string value;

    while(getline(ss, value, ','))
    {
        result.values.push_back(value);
    }

    return result;
}