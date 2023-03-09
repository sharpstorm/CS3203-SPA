#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../../common/PQLQuery.h"
#include "QueryTokenParseState.h"
#include "common/parser/ISourceExpressionParser.h"
#include "parsers/select_parser/PQLSelectParser.h"
#include "parsers/declaration_parser/PQLDeclarationParser.h"
#include "qps/parser/token_parser/parsers/conditional_parser/PQLConditionalParser.h"

using std::string, std::vector;

class QueryTokenParser {
 private:
  vector<PQLToken>* tokens;

  PQLSelectParser selectParser;
  PQLDeclarationParser declarationParser;
  PQLConditionalParser conditionalParser;

 public:
  explicit QueryTokenParser(ISourceExpressionParser* exprParser,
                            vector<PQLToken>* tokens);
  unique_ptr<PQLQuery> build();
};
