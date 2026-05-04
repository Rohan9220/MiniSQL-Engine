

// #include "../include/storage.h"
// #include "../include/metadata.h"
// #include "../include/validator.h"
// #include <fstream>
// #include <iostream>
// #include <sstream>

// using namespace std;

// // ---------------- COLUMN INDEX ----------------
// int getColumnIndex(string colName, vector<Column> schema)
// {
//     for(int i = 0; i < schema.size(); i++)
//     {
//         if(schema[i].name == colName)
//             return i;
//     }
//     return -1;
// }

// // ---------------- INSERT ----------------
// bool insertRecord(string tableName, vector<string> values)
// {
//     string filePath = "data/" + tableName + ".tbl";
//     vector<Column> schema = loadMetadata(tableName);

//     // 🔥 SAFETY CHECK
//     if(values.size() != schema.size())
//     {
//         cout << "Error: Column count mismatch\n";
//         return false;
//     }

//     ifstream readFile(filePath);
//     string line;

//     while(getline(readFile, line))
//     {
//         stringstream ss(line);
//         vector<string> record;
//         string value;

//         while(getline(ss, value, ','))
//         {
//             record.push_back(cleanValue(value));
//         }

//         // 🔥 SAFE LOOP
//         for(int i = 0; i < values.size(); i++)
//         {
//             string val = cleanValue(values[i]);

//             if(i >= record.size()) continue;

//             if(schema[i].constraint == "PRIMARY_KEY" && record[i] == val)
//             {
//                 cout << "Error: PRIMARY KEY violation\n";
//                 return false;
//             }

//             if(schema[i].constraint == "UNIQUE" && record[i] == val)
//             {
//                 cout << "Error: UNIQUE constraint violation\n";
//                 return false;
//             }
//         }
//     }

//     readFile.close();

//     ofstream writeFile(filePath, ios::app);

//     for(int i = 0; i < values.size(); i++)
//     {
//         writeFile << cleanValue(values[i]);

//         if(i != values.size() - 1)
//             writeFile << ",";
//     }

//     writeFile << endl;
//     writeFile.close();

//     cout << "Record inserted successfully\n";
//     return true;
// }

// // ---------------- UPDATE ----------------
// bool updateRecords(ParsedQuery pq)
// {
//     string filePath = "data/" + pq.tableName + ".tbl";
//     vector<Column> schema = loadMetadata(pq.tableName);

//     int colIndex = getColumnIndex(pq.columnName, schema);
//     int condIndex = getColumnIndex(pq.conditionColumn, schema);

//     if(colIndex == -1)
//     {
//         cout << "Error: Column not found\n";
//         return false;
//     }

//     vector<vector<string>> allRecords;
//     ifstream file(filePath);
//     string line;

//     while(getline(file, line))
//     {
//         stringstream ss(line);
//         vector<string> row;
//         string val;

//         while(getline(ss, val, ','))
//             row.push_back(cleanValue(val));

//         allRecords.push_back(row);
//     }
//     file.close();

//     if(!validateValue(pq.newValue, schema[colIndex]))
//         return false;

//     bool updated = false;

//     for(auto &row : allRecords)
//     {
//         // 🔥 HANDLE NO WHERE
//         if(pq.conditionColumn.empty())
//         {
//             row[colIndex] = cleanValue(pq.newValue);
//             updated = true;
//         }
//         else
//         {
//             if(condIndex == -1 || condIndex >= row.size())
//                 continue;

//             if(cleanValue(row[condIndex]) == cleanValue(pq.conditionValue))
//             {
//                 row[colIndex] = cleanValue(pq.newValue);
//                 updated = true;
//             }
//         }
//     }

//     if(!updated)
//     {
//         cout << "No matching records found\n";
//         return false;
//     }

//     ofstream out(filePath);

//     for(auto row : allRecords)
//     {
//         for(int i = 0; i < row.size(); i++)
//         {
//             out << row[i];
//             if(i != row.size() - 1)
//                 out << ",";
//         }
//         out << endl;
//     }

//     out.close();
//     return true;
// }

// // ---------------- DELETE ----------------
// bool deleteRecords(ParsedQuery pq)
// {
//     string filePath = "data/" + pq.tableName + ".tbl";
//     vector<Column> schema = loadMetadata(pq.tableName);

//     int condIndex = getColumnIndex(pq.conditionColumn, schema);

//     vector<vector<string>> newRecords;
//     ifstream file(filePath);
//     string line;

//     bool deleted = false; // 🔥 track deletion

//     while(getline(file, line))
//     {
//         stringstream ss(line);
//         vector<string> row;
//         string val;

//         while(getline(ss, val, ','))
//             row.push_back(cleanValue(val));

//         // DELETE ALL (no WHERE)
//         if(pq.conditionColumn.empty())
//         {
//             deleted = true;
//             continue;
//         }

//         if(condIndex == -1 || condIndex >= row.size())
//         {
//             newRecords.push_back(row);
//             continue;
//         }

//         if(cleanValue(row[condIndex]) == cleanValue(pq.conditionValue))
//         {
//             deleted = true; // 🔥 mark deletion
//             continue;
//         }

//         newRecords.push_back(row);
//     }

//     file.close();

//     // 🔥 If nothing deleted
//     if(!deleted)
//     {
//         cout << "No matching records found\n";
//         return false;
//     }

//     ofstream out(filePath);

//     for(auto row : newRecords)
//     {
//         for(int i = 0; i < row.size(); i++)
//         {
//             out << row[i];
//             if(i != row.size() - 1)
//                 out << ",";
//         }
//         out << endl;
//     }

//     out.close();
//     return true;
// }

#include "../include/storage.h"
#include "../include/metadata.h"
#include "../include/validator.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// ---------------- COLUMN INDEX ----------------
int getColumnIndex(string colName, vector<Column> schema)
{
    for(int i = 0; i < schema.size(); i++)
    {
        if(schema[i].name == colName)
            return i;
    }
    return -1;
}

// ---------------- UNIQUE / PK CHECK FOR UPDATE ----------------
bool violatesUniqueOrPK(vector<vector<string>> &records,
                        vector<Column> &schema,
                        int colIndex,
                        string newValue,
                        int currentRow)
{
    string val = cleanValue(newValue);

    if(schema[colIndex].constraint != "UNIQUE" &&
       schema[colIndex].constraint != "PRIMARY_KEY")
        return false;

    for(int i = 0; i < records.size(); i++)
    {
        if(i == currentRow) continue;

        if(colIndex < records[i].size())
        {
            if(cleanValue(records[i][colIndex]) == val)
                return true;
        }
    }

    return false;
}

// ---------------- INSERT ----------------
bool insertRecord(string tableName, vector<string> values)
{
    string filePath = "data/" + tableName + ".tbl";
    vector<Column> schema = loadMetadata(tableName);

    if(values.size() != schema.size())
    {
        cout << "Error: Column count mismatch\n";
        return false;
    }

    ifstream readFile(filePath);
    string line;

    while(getline(readFile, line))
    {
        stringstream ss(line);
        vector<string> record;
        string value;

        while(getline(ss, value, ','))
        {
            record.push_back(cleanValue(value));
        }

        for(int i = 0; i < values.size(); i++)
        {
            string val = cleanValue(values[i]);

            if(i >= record.size()) continue;

            if(schema[i].constraint == "PRIMARY_KEY" && record[i] == val)
            {
                cout << "Error: PRIMARY KEY violation\n";
                return false;
            }

            if(schema[i].constraint == "UNIQUE" && record[i] == val)
            {
                cout << "Error: UNIQUE constraint violation\n";
                return false;
            }
        }
    }

    readFile.close();

    ofstream writeFile(filePath, ios::app);

    for(int i = 0; i < values.size(); i++)
    {
        writeFile << cleanValue(values[i]);

        if(i != values.size() - 1)
            writeFile << ",";
    }

    writeFile << endl;
    writeFile.close();

    cout << "Record inserted successfully\n";
    return true;
}

// ---------------- UPDATE ----------------
bool updateRecords(ParsedQuery pq)
{
    string filePath = "data/" + pq.tableName + ".tbl";
    vector<Column> schema = loadMetadata(pq.tableName);

    int colIndex = getColumnIndex(pq.columnName, schema);
    int condIndex = getColumnIndex(pq.conditionColumn, schema);

    if(colIndex == -1)
    {
        cout << "Error: Column not found\n";
        return false;
    }

    // ❌ Prevent updating PRIMARY KEY (optional but strong)
    if(schema[colIndex].constraint == "PRIMARY_KEY")
    {
        cout << "Error: Cannot update PRIMARY KEY\n";
        return false;
    }

    vector<vector<string>> allRecords;
    ifstream file(filePath);
    string line;

    while(getline(file, line))
    {
        stringstream ss(line);
        vector<string> row;
        string val;

        while(getline(ss, val, ','))
            row.push_back(cleanValue(val));

        allRecords.push_back(row);
    }
    file.close();

    if(!validateValue(pq.newValue, schema[colIndex]))
        return false;

    bool updated = false;

    for(int r = 0; r < allRecords.size(); r++)
    {
        auto &row = allRecords[r];
        bool match = false;

        if(pq.conditionColumn.empty())
            match = true;
        else if(condIndex != -1 && condIndex < row.size() &&
                cleanValue(row[condIndex]) == cleanValue(pq.conditionValue))
            match = true;

        if(match)
        {
            // 🔥 UNIQUE / PRIMARY KEY check
            if(violatesUniqueOrPK(allRecords, schema, colIndex, pq.newValue, r))
            {
                cout << "Error: " << schema[colIndex].constraint
                     << " constraint violation for "
                     << schema[colIndex].name << endl;
                return false;
            }

            row[colIndex] = cleanValue(pq.newValue);
            updated = true;
        }
    }

    if(!updated)
    {
        cout << "No matching records found\n";
        return false;
    }

    ofstream out(filePath);

    for(auto row : allRecords)
    {
        for(int i = 0; i < row.size(); i++)
        {
            out << row[i];
            if(i != row.size() - 1)
                out << ",";
        }
        out << endl;
    }

    out.close();
    return true;
}

// ---------------- DELETE ----------------
bool deleteRecords(ParsedQuery pq)
{
    string filePath = "data/" + pq.tableName + ".tbl";
    vector<Column> schema = loadMetadata(pq.tableName);

    int condIndex = getColumnIndex(pq.conditionColumn, schema);

    vector<vector<string>> newRecords;
    ifstream file(filePath);
    string line;

    bool deleted = false;

    while(getline(file, line))
    {
        stringstream ss(line);
        vector<string> row;
        string val;

        while(getline(ss, val, ','))
            row.push_back(cleanValue(val));

        if(pq.conditionColumn.empty())
        {
            deleted = true;
            continue;
        }

        if(condIndex == -1 || condIndex >= row.size())
        {
            newRecords.push_back(row);
            continue;
        }

        if(cleanValue(row[condIndex]) == cleanValue(pq.conditionValue))
        {
            deleted = true;
            continue;
        }

        newRecords.push_back(row);
    }

    file.close();

    if(!deleted)
    {
        cout << "No matching records found\n";
        return false;
    }

    ofstream out(filePath);

    for(auto row : newRecords)
    {
        for(int i = 0; i < row.size(); i++)
        {
            out << row[i];
            if(i != row.size() - 1)
                out << ",";
        }
        out << endl;
    }

    out.close();
    return true;
}