#pragma once

#include <string>
#include <vector>

using std::string, std::vector;

class QueryResult {
    vector<int> statementList;
    vector<string> entityList;
    string error;

 public:
    vector<int> getStatementList();
    vector<string> getEntityList();
    string getError();
};
