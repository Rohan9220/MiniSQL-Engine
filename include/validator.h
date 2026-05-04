// #ifndef VALIDATOR_H
// #define VALIDATOR_H

// #include <vector>
// #include <string>
// #include "metadata.h"

// using namespace std;

// bool validateInsert(vector<string> values, vector<Column> schema);

// #endif

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <vector>
#include <string>
#include "metadata.h"
#include "parser.h"

using namespace std;

// Utility
string cleanValue(string val);
bool isInteger(string s);

// INSERT
bool validateInsert(vector<string> values, vector<Column> schema);

// UPDATE
bool validateValue(string value, Column col);
bool validateUpdate(ParsedQuery pq, vector<Column> schema);

// DELETE
bool validateDelete(ParsedQuery pq, vector<Column> schema);

#endif