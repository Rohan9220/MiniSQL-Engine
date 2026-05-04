

// #ifndef METADATA_H
// #define METADATA_H

// #include <vector>
// #include <string>

// using namespace std;

// struct Column
// {
//     string name;
//     string type;
//     string constraint;
// };

// vector<Column> loadMetadata(string tableName);

// #endif


#ifndef METADATA_H
#define METADATA_H

#include <vector>
#include <string>

using namespace std;

struct Column
{
    string name;
    string type;
    string constraint;

    // 🔥 NEW (for FOREIGN KEY support)
    string refTable;
    string refColumn;
};

vector<Column> loadMetadata(string tableName);

#endif