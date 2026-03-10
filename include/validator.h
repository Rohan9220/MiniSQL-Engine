#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <vector>
#include <string>
#include "metadata.h"

using namespace std;

bool validateInsert(vector<string> values, vector<Column> schema);

#endif