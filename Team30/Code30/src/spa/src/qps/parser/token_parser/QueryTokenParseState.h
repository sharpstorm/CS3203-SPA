#pragma once

#include <vector>
#include <unordered_set>
#include <string>

#include "token_stream/QueryTokenStream.h"
#include "../../errors/QPSParserSyntaxError.h"
#include "QueryExpectationAgent.h"

using std::vector, std::unordered_set, std::string;

class QueryTokenParseState:
    public QueryExpectationAgent {
 private:
  QueryTokenStream tokenStream;

 public:
  explicit QueryTokenParseState(vector<PQLToken>* tokens);
  bool isTokenStreamEnd();
  void advanceToken();
  PQLTokenType getCurrentTokenType();
  bool isCurrentTokenType(PQLTokenType type);
  bool isCurrentTokenCategory(PQLTokenCategory category);
};
