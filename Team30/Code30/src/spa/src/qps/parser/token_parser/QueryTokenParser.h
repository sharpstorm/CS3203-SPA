#pragma once

#include "../../common/PQLQuery.h"
#include "QueryTokenParseState.h"
#include "common/parser/ISourceExpressionParser.h"
#include "parsers/select_parser/PQLSelectParser.h"
#include "parsers/declaration_parser/PQLDeclarationParser.h"
#include "qps/parser/token_parser/parsers/conditional_parser/PQLConditionalParser.h"

class QueryTokenParser {
 private:
  PQLTokenList *tokens;

  PQLSelectParser selectParser;
  PQLDeclarationParser declarationParser;
  PQLConditionalParser conditionalParser;

 public:
  explicit QueryTokenParser(ISourceExpressionParser *exprParser,
                            PQLTokenList *tokens);
  PQLQueryPtr build();
};
