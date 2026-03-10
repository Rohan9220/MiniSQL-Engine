#include "../include/validator.h"
#include <iostream>
#include <cctype>

using namespace std;

bool isInteger(string s)
{
    for(char c : s)
    {
        if(!isdigit(c))
            return false;
    }
    return true;
}

bool validateInsert(vector<string> values, vector<Column> schema)
{
    if(values.size() != schema.size())
    {
        cout << "Error: Column count mismatch\n";
        return false;
    }

    for(int i = 0; i < values.size(); i++)
    {
        string val = values[i];

        // remove quotes
        if(val.front() == '"' && val.back() == '"')
        {
            val = val.substr(1, val.size()-2);
        }

        // datatype check
        if(schema[i].type == "INT")
        {
            if(!isInteger(val))
            {
                cout << "Error: Datatype mismatch for "
                     << schema[i].name << endl;
                return false;
            }
        }

        // NOT NULL check
        if(schema[i].constraint == "NOT_NULL")
        {
            if(val == "NULL" || val.empty())
            {
                cout << "Error: NOT NULL constraint violated for "
                     << schema[i].name << endl;
                return false;
            }
        }
    }

    return true;
}