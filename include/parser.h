#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

using namespace std;

struct ParsedQuery
{
    string tableName;
    vector<string> values;
};

ParsedQuery parseInsertQuery(string query);

#endif