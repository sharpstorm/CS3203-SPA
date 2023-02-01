#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../../common/PQLQuery.h"
#include "TokenParseState.h"
#include "context/PQLGrammarContextProvider.h"

using std::string, std::vector;

class TokenParser {
 private:
  vector<PQLToken> tokens;
  PQLGrammarContextProvider contextProvider;

 public:
  explicit TokenParser(vector<PQLToken> tokens);
  unique_ptr<PQLQuery> build();
};
