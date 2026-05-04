
#include "../include/parser.h"
#include <sstream>
#include <iostream>

using namespace std;

// ---------------- INSERT (ADD) ----------------
ParsedQuery parseInsertQuery(string query)
{
    ParsedQuery result;
    result.queryType = "INSERT";

    int intoPos = query.find("INTO");
    int valuesPos = query.find("VALUES");

    // 🔥 SAFETY CHECK
    if(intoPos == string::npos || valuesPos == string::npos)
    {
        cout << "Error: Invalid ADD query\n";
        return result;
    }

    // Extract table name
    result.tableName = query.substr(intoPos + 5, valuesPos - (intoPos + 5));

    result.tableName.erase(0, result.tableName.find_first_not_of(" "));
    result.tableName.erase(result.tableName.find_last_not_of(" ") + 1);

    // Extract values
    int start = query.find("(");
    int end = query.find(")");

    if(start == string::npos || end == string::npos)
    {
        cout << "Error: Invalid VALUES format\n";
        return result;
    }

    string valuesStr = query.substr(start + 1, end - start - 1);

    stringstream ss(valuesStr);
    string value;

    while(getline(ss, value, ','))
    {
        value.erase(0, value.find_first_not_of(" "));
        value.erase(value.find_last_not_of(" ") + 1);

        result.values.push_back(value);
    }

    return result;
}

// ---------------- MAIN PARSER ----------------
ParsedQuery parseQuery(string query)
{
    ParsedQuery result;

    // ADD → INSERT
    if(query.find("ADD") != string::npos)
    {
        return parseInsertQuery(query);
    }

    // UPDATE
    else if(query.find("UPDATE") != string::npos)
    {
        result.queryType = "UPDATE";

        int updatePos = query.find("UPDATE");
        int setPos = query.find("SET");
        int wherePos = query.find("WHERE");

        result.tableName = query.substr(updatePos + 7, setPos - (updatePos + 7));
        result.tableName.erase(0, result.tableName.find_first_not_of(" "));
        result.tableName.erase(result.tableName.find_last_not_of(" ") + 1);

        string setPart;
        if(wherePos != string::npos)
            setPart = query.substr(setPos + 4, wherePos - (setPos + 4));
        else
            setPart = query.substr(setPos + 4);

        int eqPos = setPart.find("=");

        result.columnName = setPart.substr(0, eqPos);
        result.newValue = setPart.substr(eqPos + 1);

        result.columnName.erase(0, result.columnName.find_first_not_of(" "));
        result.columnName.erase(result.columnName.find_last_not_of(" ") + 1);

        result.newValue.erase(0, result.newValue.find_first_not_of(" "));
        result.newValue.erase(result.newValue.find_last_not_of(" ") + 1);

        if(wherePos != string::npos)
        {
            string wherePart = query.substr(wherePos + 6);

            int eqPos2 = wherePart.find("=");

            result.conditionColumn = wherePart.substr(0, eqPos2);
            result.conditionValue = wherePart.substr(eqPos2 + 1);

            result.conditionColumn.erase(0, result.conditionColumn.find_first_not_of(" "));
            result.conditionColumn.erase(result.conditionColumn.find_last_not_of(" ") + 1);

            result.conditionValue.erase(0, result.conditionValue.find_first_not_of(" "));
            result.conditionValue.erase(result.conditionValue.find_last_not_of(" ") + 1);
        }

        return result;
    }

    // REMOVE → DELETE
    else if(query.find("REMOVE") != string::npos)
    {
        result.queryType = "DELETE";

        int fromPos = query.find("FROM");
        int wherePos = query.find("WHERE");

        if(fromPos == string::npos)
        {
            cout << "Error: Invalid REMOVE query\n";
            return result;
        }

        if(wherePos != string::npos)
            result.tableName = query.substr(fromPos + 5, wherePos - (fromPos + 5));
        else
            result.tableName = query.substr(fromPos + 5);

        result.tableName.erase(0, result.tableName.find_first_not_of(" "));
        result.tableName.erase(result.tableName.find_last_not_of(" ") + 1);

        if(wherePos != string::npos)
        {
            string wherePart = query.substr(wherePos + 6);

            int eqPos = wherePart.find("=");

            result.conditionColumn = wherePart.substr(0, eqPos);
            result.conditionValue = wherePart.substr(eqPos + 1);

            result.conditionColumn.erase(0, result.conditionColumn.find_first_not_of(" "));
            result.conditionColumn.erase(result.conditionColumn.find_last_not_of(" ") + 1);

            result.conditionValue.erase(0, result.conditionValue.find_first_not_of(" "));
            result.conditionValue.erase(result.conditionValue.find_last_not_of(" ") + 1);
        }

        return result;
    }

    cout << "Error: Unsupported Query\n";
    return result;
}