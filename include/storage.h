// #ifndef STORAGE_H
// #define STORAGE_H

// #include <vector>
// #include <string>

// using namespace std;

// bool insertRecord(string tableName, vector<string> values);

// #endif

#ifndef STORAGE_H
#define STORAGE_H

#include <vector>
#include <string>
#include "metadata.h"
#include "parser.h"

using namespace std;

// Utility
string cleanValue(string val);
int getColumnIndex(string colName, vector<Column> schema);

// INSERT
bool insertRecord(string tableName, vector<string> values);

// UPDATE
bool updateRecords(ParsedQuery pq);

// DELETE
bool deleteRecords(ParsedQuery pq);

#endif