

#include <iostream>
#include <vector>
#include "../include/parser.h"
#include "../include/metadata.h"
#include "../include/validator.h"
#include "../include/storage.h"

using namespace std;

int main()
{
    string query;

    cout << "MiniSQL Engine Started" << endl;

    while(true)
    {
        cout << "MiniSQL> ";
        getline(cin, query);

        if(query == "EXIT")
            break;

        if(query.empty())
            continue;

        // Step 1: Parse
        ParsedQuery pq = parseQuery(query);

        // ❗ Invalid query check
        if(pq.queryType.empty())
        {
            cout << "Invalid Query\n";
            continue;
        }

        // Step 2: Load schema
        vector<Column> schema = loadMetadata(pq.tableName);

        if(schema.empty())
        {
            cout << "Error: Table not found\n";
            continue;
        }

        // ---------------- INSERT ----------------
        if(pq.queryType == "INSERT")
        {
            if(!validateInsert(pq.values, schema))
            {
                cout << "Insert Failed\n";
                continue;
            }

            if(!insertRecord(pq.tableName, pq.values))
            {
                cout << "Insert Failed\n";
                continue;
            }

            cout << "Insert Successful\n";
        }

        // ---------------- UPDATE ----------------
        else if(pq.queryType == "UPDATE")
        {
            if(!validateUpdate(pq, schema))
            {
                cout << "Update Failed\n";
                continue;
            }

            if(!updateRecords(pq))
            {
                cout << "Update Failed\n";
                continue;
            }

            cout << "Update Successful\n";
        }

        // ---------------- DELETE ----------------
        else if(pq.queryType == "DELETE")
        {
            if(!validateDelete(pq, schema))
            {
                cout << "Delete Failed\n";
                continue;
            }

            if(!deleteRecords(pq))
            {
                cout << "Delete Failed\n";
                continue;
            }

            cout << "Delete Successful\n";
        }

        else
        {
            cout << "Unsupported Query\n";
        }
    }

    return 0;
}