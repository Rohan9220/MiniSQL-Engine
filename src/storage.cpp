#include "../include/storage.h"
#include "../include/metadata.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

bool insertRecord(string tableName, vector<string> values)
{
    string filePath = "data/" + tableName + ".tbl";

    vector<Column> schema = loadMetadata(tableName);

    ifstream readFile(filePath);

    string line;

    while(getline(readFile, line))
    {
        stringstream ss(line);
        vector<string> record;
        string value;

        while(getline(ss, value, ','))
        {
            record.push_back(value);
        }

        for(int i = 0; i < schema.size(); i++)
        {
            if(i >= record.size()) 
                continue;

            string val = values[i];

            // remove quotes safely
            if(!val.empty() && val.front() == '"' && val.back() == '"')
            {
                val = val.substr(1, val.size() - 2);
            }

            // PRIMARY KEY check
            if(schema[i].constraint == "PRIMARY_KEY")
            {
                if(record[i] == val)
                {
                    cout << "Error: PRIMARY KEY violation\n";
                    readFile.close();
                    return false;
                }
            }

            // UNIQUE check
            if(schema[i].constraint == "UNIQUE")
            {
                if(record[i] == val)
                {
                    cout << "Error: UNIQUE constraint violation for "
                         << schema[i].name << endl;

                    readFile.close();
                    return false;
                }
            }
        }
    }

    readFile.close();

    ofstream writeFile(filePath, ios::app);

    for(int i = 0; i < values.size(); i++)
    {
        string val = values[i];

        if(!val.empty() && val.front() == '"' && val.back() == '"')
        {
            val = val.substr(1, val.size() - 2);
        }

        writeFile << val;

        if(i != values.size() - 1)
            writeFile << ",";
    }

    writeFile << endl;

    writeFile.close();

    cout << "Record inserted successfully\n";

    return true;
}