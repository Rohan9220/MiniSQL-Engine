// #ifndef PARSER_H
// #define PARSER_H

// #include <string>
// #include <vector>

// using namespace std;

// struct ParsedQuery
// {
//     string tableName;
//     vector<string> values;
// };

// ParsedQuery parseInsertQuery(string query);

// #endif


#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

using namespace std;

struct ParsedQuery
{
    string queryType;   // INSERT / UPDATE / DELETE
    string tableName;

    // INSERT
    vector<string> values;

    // UPDATE
    string columnName;
    string newValue;

    // WHERE (UPDATE + DELETE)
    string conditionColumn;
    string conditionValue;
};

ParsedQuery parseQuery(string query);
ParsedQuery parseInsertQuery(string query);

#endif