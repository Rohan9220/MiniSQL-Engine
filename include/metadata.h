#ifndef METADATA_H
#define METADATA_H

#include <string>
#include <vector>

using namespace std;

struct Column
{
    string name;
    string type;
    string constraint;
};

vector<Column> loadMetadata(string tableName);

#endif