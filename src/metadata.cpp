
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
        cout << "Error: Metadata file not found for table: " 
             << tableName << endl;
        return schema;
    }

    int columnCount;
    file >> columnCount;

    file.ignore(); // skip newline after number

    for(int i = 0; i < columnCount; i++)
    {
        string line;
        getline(file, line);

        // skip empty lines safely
        if(line.empty())
        {
            i--;
            continue;
        }

        stringstream ss(line);

        Column col;

        ss >> col.name >> col.type;

        // constraint optional
        if(!(ss >> col.constraint))
            col.constraint = "NONE";

        schema.push_back(col);
    }

    file.close();

    if(schema.size() != columnCount)
    {
        cout << "Warning: Metadata mismatch in table "
             << tableName << endl;
    }

    return schema;
}