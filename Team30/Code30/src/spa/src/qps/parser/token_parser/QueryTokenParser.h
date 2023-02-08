#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../../common/PQLQuery.h"
#include "QueryTokenParseState.h"
#include "context/PQLGrammarContextProvider.h"

using std::string, std::vector;

class QueryTokenParser {
 private:
  vector<PQLToken> tokens;
  PQLGrammarContextProvider contextProvider;

 public:
  explicit QueryTokenParser(vector<PQLToken> tokens);
  unique_ptr<PQLQuery> build();
};