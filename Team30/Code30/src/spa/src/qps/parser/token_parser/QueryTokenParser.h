#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../../common/PQLQuery.h"
#include "QueryTokenParseState.h"
#include "context/PQLGrammarContextProvider.h"
#include "common/parser/ISourceExpressionParser.h"

using std::string, std::vector;

class QueryTokenParser {
 private:
  vector<PQLToken>* tokens;
  PQLGrammarContextProvider contextProvider;

 public:
  explicit QueryTokenParser(ISourceExpressionParser* exprParser,
                            vector<PQLToken>* tokens);
  unique_ptr<PQLQuery> build();
};
