

// #include "../include/validator.h"
// #include <iostream>
// #include <cctype>

// using namespace std;

// // ---------------- CLEAN VALUE ----------------
// string cleanValue(string val)
// {
//     // trim spaces
//     val.erase(0, val.find_first_not_of(" "));
//     val.erase(val.find_last_not_of(" ") + 1);

//     // remove quotes (" or ')
//     if(!val.empty())
//     {
//         if((val.front() == '"' && val.back() == '"') ||
//            (val.front() == '\'' && val.back() == '\''))
//         {
//             val = val.substr(1, val.size() - 2);
//         }
//     }

//     return val;
// }

// // ---------------- INTEGER CHECK ----------------
// bool isInteger(string s)
// {
//     if(s.empty())
//         return false;

//     for(char c : s)
//     {
//         if(!isdigit(c))
//             return false;
//     }
//     return true;
// }

// // ---------------- VALIDATE SINGLE VALUE ----------------
// // Used in UPDATE
// bool validateValue(string value, Column col)
// {
//     string val = cleanValue(value);

//     // NOT NULL constraint
//     if(col.constraint == "NOT_NULL")
//     {
//         if(val == "NULL" || val.empty())
//         {
//             cout << "Error: NOT NULL constraint violated for "
//                  << col.name << endl;
//             return false;
//         }
//     }

//     // INT datatype
//     if(col.type == "INT")
//     {
//         if(val == "NULL") return true; // allowed if not NOT_NULL

//         if(!isInteger(val))
//         {
//             cout << "Error: Datatype mismatch for "
//                  << col.name << endl;
//             return false;
//         }
//     }

//     // STRING datatype
//     if(col.type == "STRING")
//     {
//         if(val == "NULL") return true;

//         if(val.empty())
//         {
//             cout << "Error: Invalid string for "
//                  << col.name << endl;
//             return false;
//         }
//     }

//     return true;
// }

// // ---------------- VALIDATE INSERT ----------------
// bool validateInsert(vector<string> values, vector<Column> schema)
// {
//     // Column count check
//     if(values.size() != schema.size())
//     {
//         cout << "Error: Column count mismatch\n";
//         return false;
//     }

//     for(int i = 0; i < values.size(); i++)
//     {
//         if(!validateValue(values[i], schema[i]))
//             return false;
//     }

//     return true;
// }

// // ---------------- VALIDATE UPDATE ----------------
// bool validateUpdate(ParsedQuery pq, vector<Column> schema)
// {
//     // find column index
//     int colIndex = -1;

//     for(int i = 0; i < schema.size(); i++)
//     {
//         if(schema[i].name == pq.columnName)
//         {
//             colIndex = i;
//             break;
//         }
//     }

//     if(colIndex == -1)
//     {
//         cout << "Error: Column not found in UPDATE\n";
//         return false;
//     }

//     // validate new value
//     return validateValue(pq.newValue, schema[colIndex]);
// }

// // ---------------- VALIDATE DELETE ----------------
// // No heavy validation needed
// bool validateDelete(ParsedQuery pq, vector<Column> schema)
// {
//     // if WHERE is present → check column exists
//     if(!pq.conditionColumn.empty())
//     {
//         bool found = false;

//         for(auto col : schema)
//         {
//             if(col.name == pq.conditionColumn)
//             {
//                 found = true;
//                 break;
//             }
//         }

//         if(!found)
//         {
//             cout << "Error: Column not found in DELETE condition\n";
//             return false;
//         }
//     }

//     return true;
// }


#include "../include/validator.h"
#include <iostream>
#include <cctype>

using namespace std;

// ---------------- CLEAN VALUE ----------------
string cleanValue(string val)
{
    // handle empty string safely
    if(val.empty()) return val;

    // trim spaces
    val.erase(0, val.find_first_not_of(" "));
    val.erase(val.find_last_not_of(" ") + 1);

    // remove quotes (" or ')
    if(!val.empty())
    {
        if((val.front() == '"' && val.back() == '"') ||
           (val.front() == '\'' && val.back() == '\''))
        {
            val = val.substr(1, val.size() - 2);
        }
    }

    return val;
}

// ---------------- INTEGER CHECK ----------------
bool isInteger(string s)
{
    if(s.empty())
        return false;

    for(char c : s)
    {
        if(!isdigit(c))
            return false;
    }
    return true;
}

// ---------------- VALIDATE SINGLE VALUE ----------------
bool validateValue(string value, Column col)
{
    string val = cleanValue(value);

    // NOT NULL constraint
    if(col.constraint == "NOT_NULL")
    {
        if(val == "NULL" || val.empty())
        {
            cout << "Error: NOT NULL constraint violated for "
                 << col.name << endl;
            return false;
        }
    }

    // INT datatype
    if(col.type == "INT")
    {
        if(val == "NULL") return true;

        if(!isInteger(val))
        {
            cout << "Error: Datatype mismatch for "
                 << col.name << endl;
            return false;
        }
    }

    // STRING datatype
    if(col.type == "STRING")
    {
        if(val == "NULL") return true;

        if(val.empty())
        {
            cout << "Error: Invalid string for "
                 << col.name << endl;
            return false;
        }
    }

    return true;
}

// ---------------- VALIDATE INSERT ----------------
bool validateInsert(vector<string> values, vector<Column> schema)
{
    if(values.size() != schema.size())
    {
        cout << "Error: Column count mismatch\n";
        return false;
    }

    for(int i = 0; i < values.size(); i++)
    {
        if(!validateValue(values[i], schema[i]))
            return false;
    }

    return true;
}

// ---------------- VALIDATE UPDATE ----------------
bool validateUpdate(ParsedQuery pq, vector<Column> schema)
{
    int colIndex = -1;

    for(int i = 0; i < schema.size(); i++)
    {
        if(schema[i].name == pq.columnName)
        {
            colIndex = i;
            break;
        }
    }

    if(colIndex == -1)
    {
        cout << "Error: Column not found in UPDATE\n";
        return false;
    }

    // prevent updating PRIMARY KEY here also (extra safety)
    if(schema[colIndex].constraint == "PRIMARY_KEY")
    {
        cout << "Error: Cannot update PRIMARY KEY\n";
        return false;
    }

    return validateValue(pq.newValue, schema[colIndex]);
}

// ---------------- VALIDATE DELETE ----------------
bool validateDelete(ParsedQuery pq, vector<Column> schema)
{
    if(!pq.conditionColumn.empty())
    {
        bool found = false;

        for(auto col : schema)
        {
            if(col.name == pq.conditionColumn)
            {
                found = true;
                break;
            }
        }

        if(!found)
        {
            cout << "Error: Column not found in DELETE condition\n";
            return false;
        }
    }

    return true;
}