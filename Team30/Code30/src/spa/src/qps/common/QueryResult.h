#pragma once

#include <string>
#include <vector>

using std::string, std::vector;

class QueryResult {
  vector<int> statementList;
  vector<string> entityList;
  string error;
};
