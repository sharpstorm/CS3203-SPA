#pragma once

#include <string>

using std::string;

class QueryLexer {
 public:
  string* getTokenStream(string* query);
};
