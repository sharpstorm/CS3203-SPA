#pragma once

#include <utility>
#include <string>
#include <unordered_map>
#include <vector>

#include "PQLTypes.h"

using std::pair, std::string, std::unordered_map, std::vector;

// Temporary Results
// TODO(KwanHW): Integrate when PKB comes in
struct StatementResult {
   vector<int> lines;
   vector<pair<int,int>> linePairs;
};

struct EntityResult {
    vector<int> lines;
    vector<string> entities;
    vector<pair<int,string>> enitityPairs;
};

class QueryResult {
    unordered_map<PQL_VAR_NAME, StatementResult> statementMap;
    unordered_map<PQL_VAR_NAME, EntityResult> entityMap;
//    vector<int> statementList;
//    vector<string> entityList;
    string error;

 public:
//    vector<int> getStatementList();
//    vector<string> getEntityList();
    bool isStatementMapEmpty();
    bool isEntityMapEmpty();
    StatementResult* getFromStatementMap(PQL_VAR_NAME var);
    EntityResult* getFromEntityMap(PQL_VAR_NAME var);
    string getError();
};
