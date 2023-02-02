#include "QueryResult.h"

vector<int> QueryResult::getStatementList() {
    return statementList;
}

vector<string> QueryResult::getEntityList() {
    return entityList;
}

string QueryResult::getError() {
    return error;
}
