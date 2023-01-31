#pragma once

#include <string>
#include <vector>

using std::string, std::vector;

class QueryLexer {
 public:
  vector<string>* getTokenStream(string* query);
};
