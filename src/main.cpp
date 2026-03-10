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

        // Step 1: Parse Query
        ParsedQuery pq = parseInsertQuery(query);

        // Step 2: Load Metadata
        vector<Column> schema = loadMetadata(pq.tableName);

        // Step 3: Validate Insert
        if(!validateInsert(pq.values, schema))
        {
            cout << "Insert Failed\n";
            continue;
        }

        // Step 4: Insert Record into table file
        if(!insertRecord(pq.tableName, pq.values))
        {
            cout << "Insert Failed\n";
            continue;
        }

        cout << "Insert Successful\n";
    }

    return 0;
}