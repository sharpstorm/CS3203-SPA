#include "ResultProjector.h"

using std::to_string;

string *ResultProjector::project(QueryResult* queryResult) {
    if (queryResult->getError() != "") {
        return new string(queryResult->getError());
    }

    string result;
    if (queryResult->getEntityList().size() == 0) {
        // Convert statement list to a list of strings
        for (int stmt : queryResult->getStatementList()) {
            result += to_string(stmt);
        }
    } else {
        for (string &entity : queryResult->getEntityList()) {
            result += entity;
        }
    }

    return &result;
}
