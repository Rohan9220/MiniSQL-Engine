#include "../include/metadata.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

vector<Column> loadMetadata(string tableName)
{
    vector<Column> schema;

    string filePath = "data/" + tableName + ".meta";

    ifstream file(filePath);

    if(!file)
    {
        cout << "Error: Metadata file not found\n";
        return schema;
    }

    int columnCount;
    file >> columnCount;

    file.ignore(); // remove newline after number

    for(int i = 0; i < columnCount; i++)
    {
        string line;

        getline(file, line);

        if(line.empty())   // skip empty line
        {
            i--;
            continue;
        }

        stringstream ss(line);

        Column col;

        ss >> col.name >> col.type;

        if(!(ss >> col.constraint))
            col.constraint = "NONE";

        schema.push_back(col);
    }

    file.close();

    return schema;
}