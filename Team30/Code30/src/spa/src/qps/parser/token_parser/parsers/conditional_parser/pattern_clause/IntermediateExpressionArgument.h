#pragma once

#include <string>
#include <memory>
#include "common/parser/ISourceExpressionParser.h"
#include "qps/clauses/arguments/ExpressionArgument.h"

using std::string, std::unique_ptr;

class IntermediateExpressionArgument {
 private:
  string literalPattern;
  bool isPartial;

 public:
  IntermediateExpressionArgument(const string &literalPattern,
                                 const bool isPartial);
  bool isWildcard();
  bool allowsPartial();
  IASTPtr parse(ISourceExpressionParser *parser);
};

typedef unique_ptr<IntermediateExpressionArgument>
    IntermediateExpressionArgumentPtr;
